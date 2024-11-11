#include "SwapchainImage.h"

SwapChainImage::SwapChainImage()
{
}

SwapChainImage::~SwapChainImage()
{
}

void SwapChainImage::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    CreateColor(logicalDevice, swapchain, m_SwapchainInfo);
    CreateDepth(logicalDevice, physicalDevice, m_SwapchainInfo);
}

void SwapChainImage::CreateColor(vk::Device logicalDevice, vk::SwapchainKHR swapchain, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    m_LogicalDevice = logicalDevice;
    m_Size                      = m_SwapchainInfo.minImageCount;
    vk::Extent2D imageExtent    = m_SwapchainInfo.imageExtent;

    // ��:�X���b�v�`�F�C���̉摜��getSwapchainImagesKHR����
    // �擾�������̂��g�p���Ȃ���΂Ȃ�Ȃ��B
    m_ColorImages = logicalDevice.getSwapchainImagesKHR(swapchain);

    //m_ImageMemory.resize(m_Size);
    m_ColorImageViews.resize(m_Size);

    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(m_Size);

    vk::Format fomat = m_SwapchainInfo.imageFormat;
    for (uint32_t i = 0; i < m_Size; i++)
    {
        //�摜�������ۂ̏���ݒ�
        auto viewInfo = CreateImageViewInfo(m_ColorImages[i], fomat, vk::ImageAspectFlagBits::eColor);
        m_ColorImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::CreateDepth(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR m_SwapchainInfo)
{
    m_LogicalDevice = logicalDevice;
    m_Size = m_SwapchainInfo.minImageCount;

    m_DepthImages.resize(m_Size);
    m_DepthImageMemory.resize(m_Size);
    m_DepthImageViews.resize(m_Size);


    std::vector<vk::ImageViewCreateInfo> viewInfo;
    viewInfo.resize(m_Size);

    
    vk::Extent2D imageExtent = m_SwapchainInfo.imageExtent;
    vk::Format fomat = m_SwapchainInfo.imageFormat;
    auto imageInfo = CreateImageInfo(imageExtent, fomat, vk::ImageUsageFlagBits::eDepthStencilAttachment);
    for (uint32_t i = 0; i < m_Size; i++)
    {
        //�摜���쐬
        m_DepthImages[i] = m_LogicalDevice.createImage(imageInfo);

        //�摜�̃��������m��
        auto allocInfo = AllocateImageMemory(logicalDevice, physicalDevice, m_DepthImages[i], vk::MemoryPropertyFlagBits::eDeviceLocal);
        m_DepthImageMemory[i] = m_LogicalDevice.allocateMemory(allocInfo);
        m_LogicalDevice.bindImageMemory(m_DepthImages[i], m_DepthImageMemory[i], 0);  // �o�C���h

        //�摜�������ۂ̏���ݒ�
        auto viewInfo = CreateImageViewInfo(m_DepthImages[i], fomat, vk::ImageAspectFlagBits::eDepth);
        m_DepthImageViews[i] = m_LogicalDevice.createImageView(viewInfo);
    }
}

void SwapChainImage::Destroy()
{
	// �C���[�W�r���[�̉��
	for (auto& imageView : m_ColorImageViews)
	{
        m_LogicalDevice.destroyImageView(imageView);
	}

    // �C���[�W�̉��
    for (auto& image : m_ColorImages)
    {
        m_LogicalDevice.destroyImage(image);
    }

    // �������[�̉��
    for (auto& memory : m_ColorImageMemory)
    {
        m_LogicalDevice.freeMemory(memory);
    }
}

std::vector<vk::Image> SwapChainImage::GetColorImages()
{
	return m_ColorImages;
}

std::vector<vk::ImageView> SwapChainImage::GetColorImageViews()
{
    return m_ColorImageViews;
}

uint32_t SwapChainImage::GetSize()
{
    return m_Size;
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
