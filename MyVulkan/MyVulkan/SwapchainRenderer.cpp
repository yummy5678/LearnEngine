#include "SwapChainRenderer.h"



SwapchainRenderer::SwapchainRenderer(VulkanInitializer& initializer) : 
    m_pAllocator(nullptr),
    m_AllocatorInfo(),
    m_Surface(VK_NULL_HANDLE),
    m_SwapchainInfo(),
    m_Swapchain(VK_NULL_HANDLE),
    m_ColorFormat(vk::Format::eUndefined),
    m_DepthFormat(vk::Format::eUndefined),
    m_QueueFamily(),
    m_ImageAvailableSemaphores(),
    m_PresentCommandPool(VK_NULL_HANDLE),
    m_PresentCommandBuffers(),
    //m_Fences(),
    //m_PresentationCommand(), 
    m_ImageIndex(0)
{    
    initializer.GetPDeviceExtension()->UseSwapchain();

    // �A���P�[�^�̓��e�̍\���̂����������Ă���
    m_AllocatorInfo.instance = VK_NULL_HANDLE;
    m_AllocatorInfo.physicalDevice = VK_NULL_HANDLE;
    m_AllocatorInfo.device = VK_NULL_HANDLE;
}

SwapchainRenderer::~SwapchainRenderer()
{

}

void SwapchainRenderer::Create(VmaAllocator* allocator, vk::SurfaceKHR surface)
{
    m_pAllocator = allocator;

    // VMA�ɕR�Â����Ă���I�u�W�F�N�g�̏����擾
    vmaGetAllocatorInfo(*allocator, &m_AllocatorInfo);
	vk::PhysicalDevice physicalDevice = m_AllocatorInfo.physicalDevice;
	vk::Device logicalDevice = m_AllocatorInfo.device;
    m_QueueFamily.Initialize(physicalDevice);
    m_Surface = surface;


    
    // �X���b�v�`�F�C���̍쐬
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = logicalDevice.createSwapchainKHR(m_SwapchainInfo);

    // �X���b�v�`�F�C���p�̃t���[���o�b�t�@�̍쐬
    CreateSwapchainImage();

    // �X���b�v�`�F�C���̕`��R�}���h���쐬
    CreatePresentationCommands();

    // �Z�}�t�H���摜�̐������쐬
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphoreInfo.flags;
    m_ImageAvailableSemaphores.resize(m_SwapchainInfo.minImageCount);
    for (uint32_t i = 0; i < m_SwapchainInfo.minImageCount; i++)
    {
        m_ImageAvailableSemaphores[i] = logicalDevice.createSemaphore(semaphoreInfo);
    }

    // �C���[�W�C���f�b�N�X��������
    m_ImageIndex = 0;
    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[m_ImageIndex]);
}

void SwapchainRenderer::Cleanup()
{
    // NULL�`�F�b�N
    vk::Device logicalDevice = m_AllocatorInfo.device;
    vk::Instance instance = m_AllocatorInfo.instance;
    if (logicalDevice == VK_NULL_HANDLE ||
        instance == VK_NULL_HANDLE) return;

    // �Z�}�t�H�̉��
    for (auto& semaphore : m_ImageAvailableSemaphores)
    {
        logicalDevice.destroySemaphore(semaphore);
    }

    // 
    m_QueueFamily.Cleanup();
    
    // �R�}���h�̉��
    logicalDevice.destroyCommandPool(m_PresentCommandPool);
    m_PresentCommandBuffers.clear();

    // �t���[���o�b�t�@�̉��
    for (uint32_t i = 0;i < m_DepthImageAllocation.size();i++)
    {
        vmaDestroyImage(*m_pAllocator, m_ImageSets[i].depth.buffer, m_DepthImageAllocation[i]);
    }
    m_DepthFormat = vk::Format::eUndefined;
    m_ColorFormat = vk::Format::eUndefined;
    m_ImageSets.clear();

    // �T�[�t�F�X�̉��
    instance.destroySurfaceKHR(m_Surface);

    //�X���b�v�`�F�[���̉��
    logicalDevice.destroySwapchainKHR(m_Swapchain);

    // �t���[���o�b�t�@�̃J�E���g��������
    m_ImageIndex = 0;

    // �A���P�[�^�̓��e�̍\���̂�������
    m_AllocatorInfo.instance = VK_NULL_HANDLE;
    m_AllocatorInfo.physicalDevice = VK_NULL_HANDLE;
    m_AllocatorInfo.device = VK_NULL_HANDLE;

}

vk::SwapchainKHR SwapchainRenderer::GetSwapchain()
{
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainRenderer::GetSwapchainInfo()
{
    return m_SwapchainInfo;
}

vk::Extent2D SwapchainRenderer::GetExtent()
{
    return m_SwapchainInfo.imageExtent;
}

uint32_t SwapchainRenderer::GetFrameCount()
{
    return m_SwapchainInfo.minImageCount;
}

RenderingImageSet SwapchainRenderer::GetRenderingImageSet()
{
    return m_ImageSets[m_ImageIndex];
}

vk::Format SwapchainRenderer::GetColorFormat()
{
    return m_ColorFormat;
}

vk::Format SwapchainRenderer::GetDepthFormat()
{
    return m_DepthFormat;
}

vk::Semaphore SwapchainRenderer::GetImageAvailableSemaphore()
{
    return m_ImageAvailableSemaphores[m_ImageIndex];
}

//vk::Fence SwapchainRenderer::GetFence()
//{
//    return m_Fences[m_ImageIndex];
//}

const uint32_t SwapchainRenderer::GetUseImageIndex()
{
    return m_ImageIndex;
}

void SwapchainRenderer::AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore)
{
    //// ���̃C���f�b�N�X
    //m_LogicalDevice.waitForFences(
    //    { m_Fences[m_ImageIndex] },							// ���p����t�F���X�B
    //    VK_TRUE,								// �t�F���X���S�ăV�O�i����ԂɂȂ�܂ő҂�
    //    UINT64_MAX);							// �ő�ҋ@����

    //m_LogicalDevice.resetFences(m_Fences[m_ImageIndex]);	// �t�F���X���V�O�i����Ԃɂ���
    vk::Device logicalDevice = m_AllocatorInfo.device;

    // �X���b�v�`�F�[�����玟�ɕ`�悷��C���[�W�i�t���[���o�b�t�@�̂悤�Ȃ��́j�̃C���f�b�N�X���擾���܂��B
    vk::Result result = logicalDevice.acquireNextImageKHR(
        m_Swapchain,                            // �X���b�v�`�F�[��
        std::numeric_limits<uint64_t>::max(),   // �^�C���A�E�g�̐ݒ�(�����ł͖����ҋ@)
        imageAvailableSemaphore,                // �C���[�W���g�p�\�ɂȂ�̂�ʒm����Z�}�t�H
        nullptr,                                // �t�F���X(�����ł͎g�p���Ȃ��̂�nullptr)
        &m_ImageIndex                           // �C���[�W�̃C���f�b�N�X���i�[�����
    );

    // �C���[�W�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W�𓊂���
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[������C���[�W���擾�ł��܂���ł����I");
    }

}

vk::ImageCreateInfo SwapchainRenderer::CreateImageInfo(vk::Extent2D extent, vk::Format fomat, vk::ImageUsageFlags usage)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = fomat;                                 // �摜�̃t�H�[�}�b�g
    imageCreateInfo.extent.width = extent.width;                    // �摜�̕�
    imageCreateInfo.extent.height = extent.height;                  // �摜�̍���
    imageCreateInfo.extent.depth = 1;                               // �摜�̉��s��
    imageCreateInfo.mipLevels = 1;                                  // �~�b�v�}�b�v���x��
    imageCreateInfo.arrayLayers = 1;                                // ���C���[��
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;          // �T���v����
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;             // �^�C�����O����
    imageCreateInfo.usage = usage;                                  // �g�p���@
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;      // ���L���[�h
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;    // �������C�A�E�g
    // imageCreateInfo.queueFamilyIndexCount = 0;                     // �L���[�t�@�~���[�C���f�b�N�X�̐�
    // imageCreateInfo.pQueueFamilyIndices;                           // �L���[�t�@�~���[�C���f�b�N�X�̔z��
    return imageCreateInfo;

}

vk::ImageViewCreateInfo SwapchainRenderer::CreateImageViewInfo(vk::Image image, vk::Format fomat, vk::ImageAspectFlags aspectFlag)
{
    // �摜�r���[�쐬���̏�����
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.pNext;
    imageViewCreateInfo.flags;
    imageViewCreateInfo.image = image;                                  // View ���쐬���邽�߂� Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;              // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
    imageViewCreateInfo.format = fomat;                                 // Image �f�[�^�̃t�H�[�}�b�g(image�Ɠ������̂łȂ���΂Ȃ�Ȃ�)
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity; // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
    //VK_IMAGE_ASPECT_COLOR_BIT
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlag;   // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // �\�����J�n���� Mipmap ���x��
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // �\������ Mipmap ���x���̐�
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // �\�����J�n����z�񃌃x��
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // �\������z�񃌃x���̐�

    return imageViewCreateInfo;

}

void SwapchainRenderer::CreateDepthImage(vk::Image& setImage, VmaAllocation& allocation, VmaAllocator* allocator, vk::ImageCreateInfo createInfo)
{
    VmaAllocationCreateInfo allocInfo;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // �����ōœK�ȃ�������I��
    allocInfo.pool = VK_NULL_HANDLE;
    allocInfo.memoryTypeBits = 0;
    allocInfo.preferredFlags = 0;
    allocInfo.priority = 1;
    allocInfo.requiredFlags = 0;
    allocInfo.pUserData = nullptr;
    VkImage image = nullptr;
    auto imageInfo = (VkImageCreateInfo)createInfo;


    VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocInfo, &image, &allocation, nullptr);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("VMA�ɂ��C���[�W�̍쐬�Ɏ��s���܂���!");
    }

    setImage = vk::Image(image);  // VkImage��vk::Image�ɃL���X�g

}

void SwapchainRenderer::CreatePresentationCommands()
{
    vk::Device logicalDevice = m_AllocatorInfo.device;

#pragma region �R�}���h�v�[���̍쐬
    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = m_QueueFamily.GetPresentationIndex(m_Surface);

    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    m_PresentCommandPool = logicalDevice.createCommandPool(poolInfo);
#pragma endregion

#pragma region �R�}���h�̍쐬
    // �R�}���h�o�b�t�@���A���P�[�g(���蓖�Ă�)���߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = m_PresentCommandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    allocateInfo.commandBufferCount = m_SwapchainInfo.minImageCount;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    m_PresentCommandBuffers = logicalDevice.allocateCommandBuffers(allocateInfo); //�z��ŏ�������肷��
#pragma endregion
}

void SwapchainRenderer::CreateSwapchainImage()
{
    if (m_pAllocator == nullptr) throw std::runtime_error("�X���b�v�`�F�C���摜�̍쐬����VMA��NULL�ł���!");

    uint32_t        imageCount  = m_SwapchainInfo.minImageCount;    // �X���b�v�`�F�[���̃t���[�������擾
    vk::Extent2D    imageExtent = m_SwapchainInfo.imageExtent;      // �摜�T�C�Y���擾



    // �[�x�C���[�W�̓X���b�v�`�F�[���I�u�W�F�N�g�̂ق��ŃT�|�[�g����Ă��Ȃ��̂Ŏ��O�ŗp��
    m_DepthImageAllocation.resize(imageCount);  

    // �J���[�C���[�W�����̓X���b�v�`�F�C���쐬���Ɉꏏ�ɍ쐬�����̂ŁA������g��
    vk::Device logicalDevice = m_AllocatorInfo.device;
    std::vector<vk::Image> colorImage = logicalDevice.getSwapchainImagesKHR(m_Swapchain);

    // �Ǘ�����摜�̐����Z�b�g
    m_ImageSets.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        // �J���[�C���[�W�̍쐬
        m_ImageSets[i].color.buffer = colorImage[i];
        //�C���[�W�r���[�A�摜�������ۂ̏���ݒ�
        m_ColorFormat = m_SwapchainInfo.imageFormat;
        vk::ImageViewCreateInfo colorImageViewInfo = CreateImageViewInfo(m_ImageSets[i].color.buffer, m_ColorFormat, vk::ImageAspectFlagBits::eColor);
        m_ImageSets[i].color.view = logicalDevice.createImageView(colorImageViewInfo);


        // �[�x�C���[�W�̍쐬
        m_DepthFormat = vk::Format::eD24UnormS8Uint;    //�[�x�C���[�W�̃t�H�[�}�b�g
        vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        auto imageInfo = CreateImageInfo(imageExtent, m_DepthFormat, usage);
        CreateDepthImage(m_ImageSets[i].depth.buffer, m_DepthImageAllocation[i], m_pAllocator, imageInfo);
        //�C���[�W�r���[�A�摜�������ۂ̏���ݒ�
        vk::ImageViewCreateInfo depthImageViewInfo = CreateImageViewInfo(m_ImageSets[i].depth.buffer, m_DepthFormat, vk::ImageAspectFlagBits::eDepth);
        m_ImageSets[i].depth.view = logicalDevice.createImageView(depthImageViewInfo);
    }
}

void SwapchainRenderer::Presentation(vk::SurfaceKHR surface, vk::Semaphore imageAvailableSemaphore)
{
    vk::Result result;

    vk::PresentInfoKHR presentInfo;
    presentInfo.pNext;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &m_ImageIndex;
    presentInfo.pResults = &result;
    presentInfo.pWaitSemaphores = &imageAvailableSemaphore;
    presentInfo.waitSemaphoreCount = 1;

    // �g�p����L���[(�O���t�B�b�N�L���[��v���[���g�L���[�Ȃ�)�̃C���f�b�N�X���擾
    vk::Device logicalDevice = m_AllocatorInfo.device;
    auto presentQueue = logicalDevice.getQueue(m_QueueFamily.GetPresentationIndex(surface), 0);
    presentQueue.presentKHR(presentInfo);
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[���̉摜�̕\���Ɏ��s���܂����I");
    }
}

void SwapchainRenderer::UpdateFrame()
{
    uint32_t index = m_ImageIndex;
    //m_PresentationCommand.RunningCommand(index, m_Surface, m_ImageAvailableSemaphores[index]);
    Presentation(m_Surface, m_ImageAvailableSemaphores[index]);

    // ���̃C���f�b�N�X�ɐ؂�ւ�
    index = (index + 1) % m_SwapchainInfo.minImageCount;

    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[index]);
}

/// <summary>
/// �X���b�v�`�F�[���C���t�H�̍쐬
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainRenderer::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // �X���b�v�`�F�[���̐ݒ���s��
    // �T�[�t�F�X�̋@�\���擾
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
    std::vector<vk::PresentModeKHR> surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);

    // �T�[�t�F�X����擾�������̒�����œK�Ȃ��̂�I��
    vk::SurfaceFormatKHR format = SelectSurfaceFormat(surfaceFormats);
    vk::PresentModeKHR presentMode = SelectPresentMode(surfacePresentModes);

    // �X���b�v�`�F�[���ɕK�v�ȃC���[�W�̐�������
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // �X���b�v�`�F�[���쐬���̐ݒ�
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.pNext;                       // �g���`�F�[���ւ̃|�C���^
    swapchainInfo.flags;                       // �쐬�t���O
    swapchainInfo.surface = surface;           // �X���b�v�`�F�[�����^�[�Q�b�g�Ƃ���T�[�t�F�X
    swapchainInfo.minImageCount = imageCount;  // �X���b�v�`�F�[���Ɋ܂܂��ŏ��摜��
    swapchainInfo.imageFormat = format.format;          // �摜�t�H�[�}�b�g
    swapchainInfo.imageColorSpace = format.colorSpace;  // �J���[�X�y�[�X
    swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;         // �摜�̕��ƍ���
    swapchainInfo.imageArrayLayers = 1;                                    // �摜�̔z��w��(���ʉ摜�Ȃ�"1"�ɂ���)
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // �摜�̎g�p�p�r
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;          // �摜�̋��L���[�h�i�����ݒ�͔r���I���[�h�j
    swapchainInfo.queueFamilyIndexCount;                                   // �L���[�t�@�~���[�C���f�b�N�X�̐�
    swapchainInfo.pQueueFamilyIndices;                                     // �L���[�t�@�~���[�C���f�b�N�X�̔z��ւ̃|�C���^
    swapchainInfo.preTransform = surfaceCapabilities.currentTransform;     // �T�[�t�F�X�̃g�����X�t�H�[��(�ό`���)
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // �A���t�@�������[�h
    swapchainInfo.presentMode = presentMode;        // �v���[���e�[�V�������[�h
    swapchainInfo.clipped = VK_TRUE;                // �N���b�v����邩�ǂ���
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;    // �Â��X���b�v�`�F�[���ւ̃|�C���^

    //////////////////////////////
    // �L���[�̋��L�ݒ�
    //////////////////////////////
    // �L���[�t�@�~�����قȂ�ꍇ�͋��L���[�h��ݒ�
    QueueFamilySelector queueFamilySelector;
    queueFamilySelector.Initialize(physicalDevice);
    if (queueFamilySelector.GetGraphicIndex() != queueFamilySelector.GetPresentationIndex(surface))
    {
        std::vector<uint32_t> queueFamilyIndices = { queueFamilySelector.GetGraphicIndex(), queueFamilySelector.GetPresentationIndex(surface) };
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;      // ���s���[�h�ɐݒ�
        swapchainInfo.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();    // �L���[�t�@�~���[�C���f�b�N�X�̐���ݒ�
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();      // �L���[�t�@�~���[�C���f�b�N�X�̔z���ݒ�
    }

    return swapchainInfo;
}

vk::SurfaceFormatKHR SwapchainRenderer::SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&             
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)    
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR SwapchainRenderer::SelectPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        // �T�[�t�F�X�̎g�p�\�ȃ��[�h�̒���
        // ���[���{�b�N�X���[�h������΂���ɂ���
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {

            return availablePresentMode;
        }
    }

    // �������FIFO�L���[���g�p����(�K���T�|�[�g����Ă���)
    return vk::PresentModeKHR::eFifo; 
}
