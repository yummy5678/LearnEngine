#include "SwapChainUtility.h"


SwapchainGenerator::SwapchainGenerator(CDeviceExtensionManager& deviceExtensionManager)
{
    m_ClassName = "SwapchainGenerator";

    deviceExtensionManager.UseSwapchain();
}

SwapchainGenerator::~SwapchainGenerator()
{

}

void SwapchainGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = logicalDevice.createSwapchainKHR(m_SwapchainInfo);

    //m_Images = CreateSwapChainImages(logicalDevice, physicalDevice, surface, m_Swapchain);
}

void SwapchainGenerator::Destroy(vk::Device logicalDevice)
{
    //���g���쐬����Ă��Ȃ��Ȃ����������s��Ȃ�
    if (m_bCreated == false) return;
    m_bCreated = false;
    
    //�X���b�v�`�F�C���̉��
    m_LogicalDevice.destroySwapchainKHR(m_Swapchain);
}

vk::SwapchainKHR SwapchainGenerator::GetSwapchain()
{
    CheckCreated();
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainGenerator::GetSwapchainInfo()
{
    CheckCreated();
    return m_SwapchainInfo;
}

//std::vector<SwapchainImage> SwapchainGenerator::GetSwapChainImages()
//{
//    CheckCreated();
//    return m_Images;
//}


/// <summary>
/// �X���b�v�`�F�C���C���t�H�̍쐬
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainGenerator::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // �X���b�v�`�F�C���̐ݒ���s��
    // �T�[�t�F�X�̋@�\���擾
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
    std::vector<vk::PresentModeKHR> surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);

    vk::SurfaceFormatKHR swapchainFormat = surfaceFormats[0];
    vk::PresentModeKHR swapchainPresentMode = surfacePresentModes[0];

    QueueFamilySelector queueFamilySelector(physicalDevice);



    // �X���b�v�`�F�C���ɕK�v�ȃC���[�W�̐�������
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // �X���b�v�`�F�C���쐬���̐ݒ�
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.pNext;                       // �g���`�F�[���ւ̃|�C���^
    swapchainInfo.flags;                       // �쐬�t���O
    swapchainInfo.surface = surface;           // �X���b�v�`�F�C�����^�[�Q�b�g�Ƃ���T�[�t�F�X
    swapchainInfo.minImageCount = imageCount;  // �X���b�v�`�F�C���Ɋ܂܂��ŏ��C���[�W��
    swapchainInfo.imageFormat = swapchainFormat.format;          // �C���[�W�t�H�[�}�b�g
    swapchainInfo.imageColorSpace = swapchainFormat.colorSpace;  // �J���[�X�y�[�X
    swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;      // �X���b�v�`�F�C���̕��ƍ���
    swapchainInfo.imageArrayLayers = 1;        // �C���[�W�̔z��w��
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // �C���[�W�̎g�p�p�r
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;          // �C���[�W�̋��L���[�h�i�����ݒ�͔r���I���[�h�j
    swapchainInfo.queueFamilyIndexCount;                                   // �L���[�t�@�~���[�C���f�b�N�X�̐�
    swapchainInfo.pQueueFamilyIndices;                                     // �L���[�t�@�~���[�C���f�b�N�X�̔z��ւ̃|�C���^
    swapchainInfo.preTransform = surfaceCapabilities.currentTransform;     // �T�[�t�F�X�̃g�����X�t�H�[��
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // �A���t�@�������[�h
    swapchainInfo.presentMode = swapchainPresentMode;   // �v���[���e�[�V�������[�h
    swapchainInfo.clipped = VK_TRUE;                    // �N���b�v����邩�ǂ���
    swapchainInfo.oldSwapchain = nullptr;               // �Â��X���b�v�`�F�C���ւ̃|�C���^

    // �O���t�B�b�N�X�L���[�t�@�~���̃C���f�b�N�X���擾����
    // �L���[�t�@�~���̃C���f�b�N�X���擾
    //QueueFamilyIndices indices = VulkanUtility::GetQueueFamilies(physicalDevice, surface);
    // �L���[�t�@�~���[�C���f�b�N�X�̔z����쐬
    //uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamilyIndex, indices.presentationFamily };
    
    uint32_t queueFamilyIndices[] = { queueFamilySelector.GetGraphicIndex(), queueFamilySelector.GetPresentationIndex(surface) };

    // �L���[�t�@�~�����قȂ�ꍇ�͋��L���[�h��ݒ�
    //if (indices.m_GraphicsFamilyIndex != indices.presentationFamily)
    if (queueFamilySelector.GetGraphicIndex() != queueFamilySelector.GetPresentationIndex(surface))
    {
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent; // ���s���[�h�ɐݒ�
        swapchainInfo.queueFamilyIndexCount = 2;                       // �L���[�t�@�~���[�C���f�b�N�X�̐���ݒ�
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;        // �L���[�t�@�~���[�C���f�b�N�X�̔z���ݒ�
    }

    return swapchainInfo;
}

//std::vector<SwapchainImage> SwapchainGenerator::CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain)
//{
//    // �X���b�v�`�F�[�����\������C���[�W�̃x�N�^�[���擾
//    std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain);
//
//
//    // Swapchain�̉摜���i�[����x�N�^�[���쐬
//    std::vector<SwapchainImage> swapChainImages;
//    swapChainImages.reserve(images.size());
//
//
//    // �X���b�v�`�F�C���쐬���Ɏ擾�����̂Ɠ�����񂪗~����
//    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(GetSurfaceFormats(physicalDevice, surface));
//
//    for (vk::Image image : images)
//    {
//        // �摜�n���h����ۑ�����
//        SwapchainImage swapChainImage;
//        swapChainImage.image = image;
//        //�t�H�[�}�b�g��chooseSwapSurfaceFormat�Ŏg�p�����̂Ɠ������̂łȂ���΂Ȃ�Ȃ�
//        swapChainImage.imageView = CreateImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor);
//
//        // Swapchain�摜���X�g�ɒǉ�����
//        swapChainImages.push_back(swapChainImage);
//    }
//
//    return swapChainImages;
//}

//vk::ImageView SwapchainGenerator::CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
//{
//    // �摜�r���[�쐬���̏�����
//    vk::ImageViewCreateInfo imageViewCreateInfo;
//    imageViewCreateInfo.image = image;                                            // View ���쐬���邽�߂� Image
//    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
//    imageViewCreateInfo.format = format;                                          // Image �f�[�^�̃t�H�[�}�b�g
//    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
//    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
//    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
//    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
//
//    // Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
//    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
//    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;                         // �\�����J�n���� Mipmap ���x��
//    imageViewCreateInfo.subresourceRange.levelCount = 1;                           // �\������ Mipmap ���x���̐�
//    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // �\�����J�n����z�񃌃x��
//    imageViewCreateInfo.subresourceRange.layerCount = 1;                           // �\������z�񃌃x���̐�
//
//    //�C���[�W�r���[���쐬
//    vk::ImageView imageView = logicalDevice.createImageView(imageViewCreateInfo);
//    // vkCreateImageView �̌��ʂ������ł͂Ȃ��ꍇ�A�G���[���X���[���܂�
//    if (!imageView)
//    {
//        throw std::runtime_error("Failed to create an Image View!");
//    }
//
//    // �쐬���� Image View �̃n���h����Ԃ��܂�
//    return imageView;
//
//}


//vk::SurfaceCapabilitiesKHR SwapchainGenerator::GetSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
//{
//    return physicalDevice.getSurfaceCapabilitiesKHR(surface);
//}

vk::SurfaceFormatKHR SwapchainGenerator::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
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

vk::PresentModeKHR SwapchainGenerator::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }

    return vk::PresentModeKHR::eFifo; // �t�@�C�t�H�L���[���g�p����i�K���T�|�[�g����Ă���j
}

vk::Extent2D SwapchainGenerator::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        vk::Extent2D actualExtent = { windowWidth, windowHeight }; // �����̃E�B���h�E�̃T�C�Y��ݒ肷��
        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

std::vector<vk::SurfaceFormatKHR> SwapchainGenerator::GetSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceFormatsKHR(surface);
}

std::vector<vk::PresentModeKHR> SwapchainGenerator::GetPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfacePresentModesKHR(surface);
}
