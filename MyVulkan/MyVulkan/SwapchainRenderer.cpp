#include "SwapChainRenderer.h"


SwapchainRenderer::SwapchainRenderer(VulkanInitializer& initializer) : 
    m_pAllocator(nullptr),
    m_LogicalDevice(VK_NULL_HANDLE),
    m_Surface(VK_NULL_HANDLE),
    m_SwapchainInfo(),
    m_Swapchain(VK_NULL_HANDLE),
    m_SwapChainImages(),
    m_QueueFamily(),
    m_ImageAvailableSemaphores(),
    m_PresentCommandPool(VK_NULL_HANDLE),
    m_PresentCommandBuffers(),
    //m_Fences(),
    //m_PresentationCommand(), 
    m_ImageIndex(0)
{    
    initializer.GetPDeviceExtension()->UseSwapchain();
}

SwapchainRenderer::~SwapchainRenderer()
{

}

void SwapchainRenderer::Create(VmaAllocator* allocator, vk::SurfaceKHR surface)
{
    m_pAllocator = allocator;

    // VMA�ɕR�Â����Ă���I�u�W�F�N�g�̏����擾
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo);
    m_LogicalDevice = allocatorInfo.device;
	vk::PhysicalDevice physicalDevice = allocatorInfo.physicalDevice;
    m_QueueFamily.Initialize(physicalDevice);
    m_Surface = surface;


    
    // �X���b�v�`�F�C���̍쐬
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = m_LogicalDevice.createSwapchainKHR(m_SwapchainInfo);

    // �X���b�v�`�F�C���p�̃t���[���o�b�t�@�̍쐬
    m_SwapChainImages.Create(allocator, m_Swapchain, m_SwapchainInfo);

    // �X���b�v�`�F�C���̕`��R�}���h���쐬
    CreatePresentationCommands();

    //// �t�F���X���摜�̐������쐬
    //vk::FenceCreateInfo fenceInfo;
    //fenceInfo.pNext;
    //fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    //m_Fences.resize(m_SwapchainInfo.minImageCount);
    //for (uint32_t i = 0; i < m_SwapchainInfo.minImageCount; i++)
    //{
    //    m_Fences[i] = m_LogicalDevice.createFence(fenceInfo);
    //}



    // �Z�}�t�H���摜�̐������쐬
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphoreInfo.flags;
    m_ImageAvailableSemaphores.resize(m_SwapchainInfo.minImageCount);
    for (uint32_t i = 0; i < m_SwapchainInfo.minImageCount; i++)
    {
        m_ImageAvailableSemaphores[i] = m_LogicalDevice.createSemaphore(semaphoreInfo);
    }

    // �C���[�W�C���f�b�N�X��������
    m_ImageIndex = 0;
    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[m_ImageIndex]);
}

void SwapchainRenderer::Destroy()
{
    //�X���b�v�`�F�[���̉��
    m_LogicalDevice.destroySwapchainKHR(m_Swapchain);
}

vk::SwapchainKHR SwapchainRenderer::GetSwapchain()
{
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainRenderer::GetSwapchainInfo()
{
    return m_SwapchainInfo;
}

SwapChainImage SwapchainRenderer::GetImages()
{
    return m_SwapChainImages;
}

vk::Extent2D SwapchainRenderer::GetExtent()
{
    return m_SwapchainInfo.imageExtent;
}

std::vector<RenderingImageSet> SwapchainRenderer::GetImageSets()
{
    return m_SwapChainImages.GetImageSets();
}

vk::Format SwapchainRenderer::GetColorFormat()
{
    return m_SwapChainImages.GetColorFormat();
}

vk::Format SwapchainRenderer::GetDepthFormat()
{
    return m_SwapChainImages.GetDepthFormat();
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


    // �X���b�v�`�F�[�����玟�ɕ`�悷��C���[�W�i�t���[���o�b�t�@�̂悤�Ȃ��́j�̃C���f�b�N�X���擾���܂��B
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
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

void SwapchainRenderer::CreatePresentationCommands()
{


#pragma region �R�}���h�v�[���̍쐬
    // �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = m_QueueFamily.GetPresentationIndex(m_Surface);

    // �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
    m_PresentCommandPool = m_LogicalDevice.createCommandPool(poolInfo);
#pragma endregion

#pragma region �R�}���h�̍쐬
    // �R�}���h�o�b�t�@���A���P�[�g(���蓖�Ă�)���߂̏���ݒ肷��
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = m_PresentCommandPool;                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;   // �R�}���h�o�b�t�@�̎��(PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
    allocateInfo.commandBufferCount = m_SwapchainInfo.minImageCount;           // ���蓖�Ă�R�}���h�o�b�t�@�̐�

    // �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
    m_PresentCommandBuffers = m_LogicalDevice.allocateCommandBuffers(allocateInfo); //�z��ŏ�������肷��
#pragma endregion
}

void SwapchainRenderer::Presentation(vk::SurfaceKHR surface, vk::Semaphore imageAvailableSemaphore)
{
    //vk::SemaphoreCreateInfo semaphoreInfo;
    //semaphoreInfo.pNext;
    //m_Semaphores.push_back(m_LogicalDevice.createSemaphore(semaphoreInfo));
    //m_SwapChainIndex = AcquiredSwapChainIndex;
    //m_SwapChainIndex = 0;
    //m_SwapChainIndex = 0;

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
    auto presentQueue = m_LogicalDevice.getQueue(m_QueueFamily.GetPresentationIndex(surface), 0);
    presentQueue.presentKHR(presentInfo);
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("�X���b�v�`�F�[���̉摜�̕\���Ɏ��s���܂����I");
    }
}

void SwapchainRenderer::UpdateFrame()
{
    //vk::ResultValue acquire = m_LogicalDevice.acquireNextImageKHR(
    //    m_Swapchain, std::numeric_limits<uint64_t>::max(), m_ImageAvailableSemaphores, m_Fences);
    //if (acquire.result != vk::Result::eSuccess) std::cerr << "���t���[���̎擾�Ɏ��s���܂����B" << std::endl;

    /*m_PresentationCommand.PresentFrame(m_Swapchain, acquire.value);*/
    uint32_t index = m_ImageIndex;
    //m_PresentationCommand.RunningCommand(index, m_Surface, m_ImageAvailableSemaphores[index]);
    Presentation(m_Surface, m_ImageAvailableSemaphores[index]);

    // ���̃C���f�b�N�X�ɐ؂�ւ�
    index = (index + 1) % m_SwapchainInfo.minImageCount;



    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[index]);

    printf("%d :�ԍ����X�V\n", index);
    printf("%d\n", m_ImageAvailableSemaphores[index]);
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
