#include "SwapchainImage.h"


SwapChainImage::SwapChainImage() :
    m_ColorImages(),
    m_ColorFormat(vk::Format::eUndefined),
    m_ColorImageViews(),
    m_DepthImages(),
    m_DepthFormat(vk::Format::eUndefined)



{
}

SwapChainImage::~SwapChainImage()
{
}

void SwapChainImage::Create(VmaAllocator* allocator, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    // VMAM�ɕR�Â����Ă���I�u�W�F�N�g�̏����擾
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo);
    m_Size = m_SwapchainInfo.minImageCount;
    try
    {
        CreateColor(allocatorInfo.device, swapchain, m_SwapchainInfo);
        CreateDepth(allocator, m_SwapchainInfo);
    }
    catch (const std::runtime_error& e)
    {
        //�G���[���b�Z�[�W�󂯎��
        printf("�G���[: %s\n", e.what());
    }


    //m_ColorDescriptor.CreateSingleDescriptorSet();

}

void SwapChainImage::CreateColor(vk::Device logicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
	uint32_t imageCount = m_SwapchainInfo.minImageCount;
    vk::Extent2D imageExtent    = m_SwapchainInfo.imageExtent;

    // ��:�X���b�v�`�F�C���̉摜��getSwapchainImagesKHR����
    // �擾�������̂��g�p���Ȃ���΂Ȃ�Ȃ��B
    m_ColorImages = logicalDevice.getSwapchainImagesKHR(swapchain);

   
    m_ColorImageViews.resize(imageCount);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(imageCount);

    m_ColorFormat = m_SwapchainInfo.imageFormat;
    for (uint32_t i = 0; i < imageCount; i++)
    {
        //�摜�������ۂ̏���ݒ�
        auto viewInfo = CreateImageViewInfo(m_ColorImages[i], m_ColorFormat, vk::ImageAspectFlagBits::eColor);
        m_ColorImageViews[i] = logicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::CreateDepth(VmaAllocator* allocator, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    uint32_t imageCount = m_SwapchainInfo.minImageCount;
    m_DepthImages.resize(imageCount);
    
    vk::Extent2D imageExtent = m_SwapchainInfo.imageExtent;
    m_DepthFormat = vk::Format::eD24UnormS8Uint;
    vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
    auto imageInfo = CreateImageInfo(imageExtent, m_DepthFormat, usage);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        // �摜���쐬
		m_DepthImages[i].Create(allocator, imageInfo, vk::ImageAspectFlagBits::eDepth);

        //�摜�������ۂ̏���ݒ�
        //auto viewInfo = CreateImageViewInfo(m_DepthImages[i], m_DepthFormat, vk::ImageAspectFlagBits::eDepth);
        //m_DepthImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::Destroy()
{
    // VMAM�ɕR�Â����Ă���I�u�W�F�N�g�̏����擾
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*m_pAllocator, &allocatorInfo);

    vk::Device logicalDevice = allocatorInfo.device;

    // �C���[�W�r���[�̉��
    for (auto& imageView : m_ColorImageViews)
    {
        logicalDevice.destroyImageView(imageView);
    }
}

std::vector<vk::Image> SwapChainImage::GetColorImages()
{
	return m_ColorImages;
}

vk::Format SwapChainImage::GetColorFormat()
{
    return m_ColorFormat;
}

std::vector<vk::ImageView> SwapChainImage::GetColorImageViews()
{
    return m_ColorImageViews;
}


std::vector<VImage> SwapChainImage::GetDepthImages()
{
    return m_DepthImages;
}

vk::Format SwapChainImage::GetDepthFormat()
{
    return m_DepthFormat;
}

std::vector<RenderingImageSet> SwapChainImage::GetImageSets()
{
    std::vector<RenderingImageSet> imageViewSets;
    imageViewSets.resize(m_Size);

    for (uint32_t i = 0; i < m_Size; i++)
    {
        imageViewSets[i].color = m_ColorImageViews[i];
        imageViewSets[i].depth = m_DepthImages[i].GetImageView();
    }


    return imageViewSets;
}

vk::ImageCreateInfo SwapChainImage::CreateImageInfo(vk::Extent2D extent, vk::Format fomat,vk::ImageUsageFlags usage)
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

vk::ImageViewCreateInfo SwapChainImage::CreateImageViewInfo(vk::Image image, vk::Format fomat, vk::ImageAspectFlags aspectFlag)
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

vk::MemoryAllocateInfo SwapChainImage::AllocateImageMemory(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags propertyFlags)
{
    // �C���[�W�̃������v�����擾
    vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);

    // �T���Ă��郁�����v���p�e�B�̃t���O
    vk::MemoryPropertyFlags properties = propertyFlags;
    //vk::MemoryPropertyFlags properties = vk::MemoryProperty::MemoryPropertyFlagBits::eDeviceLocalFlagBits::eHostVisible;

    // �������̊��蓖�ď���ݒ�
    vk::MemoryAllocateInfo allocateInfo;
    allocateInfo.pNext;
    allocateInfo.allocationSize = memoryRequirements.size;  // �摜�̃������T�C�Y
    allocateInfo.memoryTypeIndex = FindMemoryType(device, physicalDevice, image, properties);   // �������^�C�v

    return allocateInfo;
}

uint32_t SwapChainImage::FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags findType)
{
    // �C���[�W�̃������v�����擾
    vk::MemoryRequirements memoryRequirements = logicalDevice.getImageMemoryRequirements(image);
    auto typeFilter = memoryRequirements.memoryTypeBits;

    // �����f�o�C�X(GPU)���烁�����v���p�e�B���擾�B
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        // typeFilter�́A�r�b�g�}�X�N�Ń������^�C�v���t�B���^�����O���邽�߂̂��́B
        // �����ł́AtypeFilter�̃r�b�g��i�Ԗڂ̃������^�C�v�ɑΉ����邩�ǂ������m�F���Ă��܂��B
        // (1 << i)�́Ai�r�b�g�ڂ̃r�b�g��1�ɂ����r�b�g�}�X�N�ł��B
        // typeFilter & (1 << i) �́Ai�r�b�g�ڂ̃r�b�g�������Ă��邩���m�F���܂��B
        if ((typeFilter & (1 << i)) && 
            (memoryProperties.memoryTypes[i].propertyFlags & findType) == findType)
        {
            return i;
        }
    }

    // �G���[���b�Z�[�W
    throw std::runtime_error("�K�؂ȃ������^�C�v���������܂���ł���!");
}
