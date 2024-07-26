#include "SwapChainUtility.h"



SwapChainDetails VulkanUtility::getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
    // ����̕����f�o�C�X�ƃT�[�t�F�X�ɑ΂���\�ʂ̃L���p�r���e�B���擾����
    swapChainDetails.surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// �t�H�[�}�b�g���Ԃ��ꂽ�ꍇ�A�t�H�[�}�b�g�̃��X�g���擾����
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
        swapChainDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// �v���[���e�[�V�������[�h���Ԃ��ꂽ�ꍇ�A�v���[���e�[�V�������[�h�̃��X�g���擾����
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
        swapChainDetails.presentationModes = physicalDevice.getSurfacePresentModesKHR(surface);
	}

	return swapChainDetails;
}


// �X���b�v�`�F�C���̃t�H�[�}�b�g��I������֐�
vk::SurfaceFormatKHR VulkanUtility::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
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


// �X���b�v�`�F�C���̃v���[���g���[�h��I������֐�
vk::PresentModeKHR VulkanUtility::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }

    return vk::PresentModeKHR::eFifo; // �t�B�t�H�L���[���g�p����i�K���T�|�[�g����Ă���j
}


// �X���b�v�`�F�C���̃G�N�X�e���g��I������֐�
vk::Extent2D VulkanUtility::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
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

vk::SurfaceCapabilitiesKHR VulkanUtility::getSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceCapabilitiesKHR(surface);
}

std::vector<vk::SurfaceFormatKHR> VulkanUtility::getSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceFormatsKHR(surface);
}

std::vector<vk::PresentModeKHR> VulkanUtility::getPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfacePresentModesKHR(surface);
}

std::vector<SwapchainImage> VulkanUtility::createSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain)
{
    // �X���b�v�`�F�[�����\������C���[�W�̃x�N�^�[���擾
    std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain);


    // Swapchain�̉摜���i�[����x�N�^�[���쐬
    std::vector<SwapchainImage> swapChainImages;
    swapChainImages.reserve(images.size());

    // �X���b�v�`�F�C���쐬���Ɏ擾�����̂Ɠ�����񂪗~����
    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(getSurfaceFormats(physicalDevice, surface));

    for (vk::Image image : images)
    {
        // �摜�n���h����ۑ�����
        SwapchainImage swapChainImage = {};
        swapChainImage.image = image;
        //�t�H�[�}�b�g��chooseSwapSurfaceFormat�Ŏg�p�����̂Ɠ������̂łȂ���΂Ȃ�Ȃ�
        swapChainImage.imageView = createImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor);
        
        // Swapchain�摜���X�g�ɒǉ�����
        swapChainImages.push_back(swapChainImage);
    }
    return swapChainImages;
}

vk::ImageView VulkanUtility::createImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
    // �摜�r���[�쐬���̏�����
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.image = image;                                            // View ���쐬���邽�߂� Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
    imageViewCreateInfo.format = format;                                          // Image �f�[�^�̃t�H�[�}�b�g
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
    
    // Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;                         // �\�����J�n���� Mipmap ���x��
    imageViewCreateInfo.subresourceRange.levelCount = 1;                           // �\������ Mipmap ���x���̐�
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // �\�����J�n����z�񃌃x��
    imageViewCreateInfo.subresourceRange.layerCount = 1;                           // �\������z�񃌃x���̐�

    // vkCreateImageView �֐����g�p���� Image View ���쐬���܂�
    // mainDevice.logicalDevice: Image View ���쐬���邽�߂̘_���f�o�C�X
    // &viewCreateInfo: Image View �̍쐬�ɕK�v�ȏ�񂪊i�[���ꂽ�\���̂ւ̃|�C���^
    // nullptr: �J�X�^���̃A���P�[�^�[���g�p���Ȃ����߂̃I�v�V���� (�ʏ�� nullptr ���w�肵�܂�)
    // &imageView: �쐬���ꂽ Image View �̃n���h�����󂯎��ϐ��ւ̃|�C���^
    vk::ImageView imageView = logicalDevice.createImageView(imageViewCreateInfo);
    // vkCreateImageView �̌��ʂ������ł͂Ȃ��ꍇ�A�G���[���X���[���܂�
    if (!imageView)
    {
        throw std::runtime_error("Failed to create an Image View!");
    }

    // �쐬���� Image View �̃n���h����Ԃ��܂�
    return imageView;
}

std::vector<vk::Framebuffer> VulkanUtility::createFramebuffers(vk::Device logicalDevice,std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent)
{
    // logicalDevice���L���ł��邩�m�F
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDevice�������ł��I");
    }

    // renderPass���L���ł��邩�m�F
    if (!renderPass)
    {
        throw std::runtime_error("renderPass�������ł��I");
    }

    // swapChainImages����łȂ����Ƃ��m�F
    if (swapChainImages.empty())
    {
        throw std::runtime_error("swapChainImages����ł��I");
    }

    std::vector<vk::Framebuffer> framebuffers;
    framebuffers.reserve(swapChainImages.size());

    for (const auto& swapChainImage : swapChainImages)
    {
        if (!swapChainImage.imageView)
        {
            throw std::runtime_error("ImageView�������ł��I");
        }

        std::array<vk::ImageView, 1> attachments = { swapChainImage.imageView };

        vk::FramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = extent.width;
        framebufferCreateInfo.height = extent.height;
        framebufferCreateInfo.layers = 1;

        // vk::Framebuffer���쐬����
        vk::Framebuffer framebuffer = logicalDevice.createFramebuffer(framebufferCreateInfo);
        if (!framebuffer)
        {
            throw std::runtime_error("�t���[���o�b�t�@�̍쐬�Ɏ��s���܂����I");
        }

        // vk::Framebuffer��vk::UniqueFramebuffer�ɕϊ�����
        framebuffers.push_back(framebuffer);
    }

    return framebuffers;
}


















SwapchainGenerator::SwapchainGenerator()
{
    m_ClassName = "SwapchainGenerator";
}

SwapchainGenerator::~SwapchainGenerator()
{
    Destroy(m_LogicalDevice);
}

void SwapchainGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = logicalDevice.createSwapchainKHR(m_SwapchainInfo);

    m_Images = CreateSwapChainImages(logicalDevice, physicalDevice, surface, m_Swapchain);
}

void SwapchainGenerator::Destroy(vk::Device logicalDevice)
{
    //m_Images
    for (auto image : m_Images)
	{
		vkDestroyImageView(logicalDevice, image.imageView, nullptr);
	}

    //m_Swapchain
   	vkDestroySwapchainKHR(logicalDevice, m_Swapchain, nullptr);
}

vk::SwapchainKHR SwapchainGenerator::GetSwapchain()
{
    CheckCreated();
    return m_Swapchain;
}

vk::Extent2D SwapchainGenerator::Get2DExtent()
{
    CheckCreated();
    return m_Extent;
}

vk::SurfaceFormatKHR SwapchainGenerator::GetSurfaceFormat()
{
    CheckCreated();
    return m_SurfaceFormat;
}

vk::SwapchainCreateInfoKHR SwapchainGenerator::GetSwapchainInfo()
{
    CheckCreated();
    return m_SwapchainInfo;
}

std::vector<SwapchainImage> SwapchainGenerator::GetSwapChainImages()
{
    CheckCreated();
    return m_Images;
}


/// <summary>
/// �X���b�v�`�F�C���̍쐬
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainGenerator::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // �X���b�v�`�F�C���̐ݒ���s��
    // �T�[�t�F�X�̋@�\���擾
    m_SurfaceCapabilities = GetSurfaceCapabilities(physicalDevice, surface);


    // �X���b�v�`�F�C���̃G�N�X�e���g�i���ƍ����j��I��
    m_Extent = ChooseSwapExtent(m_SurfaceCapabilities);

    // �X���b�v�`�F�C���̐ݒ�
    // �K�؂ȃT�[�t�F�X�t�H�[�}�b�g��I��
    m_SurfaceFormat = ChooseSwapSurfaceFormat(VulkanUtility::getSurfaceFormats(physicalDevice, surface));
    // �K�؂ȃv���[���e�[�V�������[�h��I��
    m_PresentMode = ChooseSwapPresentMode(VulkanUtility::getPresentModes(physicalDevice, surface));

    // �X���b�v�`�F�C���ɕK�v�ȃC���[�W�̐�������
    uint32_t imageCount = m_SurfaceCapabilities.minImageCount + 1;
    if (m_SurfaceCapabilities.maxImageCount > 0 && imageCount > m_SurfaceCapabilities.maxImageCount)
    {
        imageCount = m_SurfaceCapabilities.maxImageCount;
    }

    // �X���b�v�`�F�C���쐬���̐ݒ�
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.pNext;                       // �g���`�F�[���ւ̃|�C���^
    swapchainInfo.flags;                       // �쐬�t���O
    swapchainInfo.surface = surface;           // �X���b�v�`�F�C�����^�[�Q�b�g�Ƃ���T�[�t�F�X
    swapchainInfo.minImageCount = imageCount;  // �X���b�v�`�F�C���Ɋ܂܂��ŏ��C���[�W��
    swapchainInfo.imageFormat = m_SurfaceFormat.format;          // �C���[�W�t�H�[�}�b�g
    swapchainInfo.imageColorSpace = m_SurfaceFormat.colorSpace;  // �J���[�X�y�[�X
    swapchainInfo.imageExtent = m_Extent;        // �X���b�v�`�F�C���̕��ƍ���
    swapchainInfo.imageArrayLayers = 1;        // �C���[�W�̔z��w��
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // �C���[�W�̎g�p�p�r
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;          // �C���[�W�̋��L���[�h�i�����ݒ�͔r���I���[�h�j
    swapchainInfo.queueFamilyIndexCount;                                   // �L���[�t�@�~���[�C���f�b�N�X�̐�
    swapchainInfo.pQueueFamilyIndices;                                     // �L���[�t�@�~���[�C���f�b�N�X�̔z��ւ̃|�C���^
    swapchainInfo.preTransform = m_SurfaceCapabilities.currentTransform;     // �T�[�t�F�X�̃g�����X�t�H�[��
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // �A���t�@�������[�h
    swapchainInfo.presentMode = m_PresentMode;   // �v���[���e�[�V�������[�h
    swapchainInfo.clipped = VK_TRUE;           // �N���b�v����邩�ǂ���
    swapchainInfo.oldSwapchain = nullptr;      // �Â��X���b�v�`�F�C���ւ̃|�C���^

    // �O���t�B�b�N�X�L���[�t�@�~���̃C���f�b�N�X���擾����
    // �L���[�t�@�~���̃C���f�b�N�X���擾
    QueueFamilyIndices indices = VulkanUtility::GetQueueFamilies(physicalDevice, surface);
    // �L���[�t�@�~���[�C���f�b�N�X�̔z����쐬
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentationFamily };

    // �L���[�t�@�~�����قȂ�ꍇ�͋��L���[�h��ݒ�
    if (indices.graphicsFamily != indices.presentationFamily)
    {
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent; // ���s���[�h�ɐݒ�
        swapchainInfo.queueFamilyIndexCount = 2;                       // �L���[�t�@�~���[�C���f�b�N�X�̐���ݒ�
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;        // �L���[�t�@�~���[�C���f�b�N�X�̔z���ݒ�
    }


    return swapchainInfo;
}

std::vector<SwapchainImage> SwapchainGenerator::CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain)
{
    // �X���b�v�`�F�[�����\������C���[�W�̃x�N�^�[���擾
    std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain);


    // Swapchain�̉摜���i�[����x�N�^�[���쐬
    std::vector<SwapchainImage> swapChainImages;
    swapChainImages.reserve(images.size());


    // �X���b�v�`�F�C���쐬���Ɏ擾�����̂Ɠ�����񂪗~����
    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(GetSurfaceFormats(physicalDevice, surface));

    for (vk::Image image : images)
    {
        // �摜�n���h����ۑ�����
        SwapchainImage swapChainImage;
        swapChainImage.image = image;
        //�t�H�[�}�b�g��chooseSwapSurfaceFormat�Ŏg�p�����̂Ɠ������̂łȂ���΂Ȃ�Ȃ�
        swapChainImage.imageView = CreateImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor);

        // Swapchain�摜���X�g�ɒǉ�����
        swapChainImages.push_back(swapChainImage);
    }

    return swapChainImages;
}

vk::ImageView SwapchainGenerator::CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
    // �摜�r���[�쐬���̏�����
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.image = image;                                            // View ���쐬���邽�߂� Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
    imageViewCreateInfo.format = format;                                          // Image �f�[�^�̃t�H�[�}�b�g
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;                         // �\�����J�n���� Mipmap ���x��
    imageViewCreateInfo.subresourceRange.levelCount = 1;                           // �\������ Mipmap ���x���̐�
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // �\�����J�n����z�񃌃x��
    imageViewCreateInfo.subresourceRange.layerCount = 1;                           // �\������z�񃌃x���̐�

    //�C���[�W�r���[���쐬
    vk::ImageView imageView = logicalDevice.createImageView(imageViewCreateInfo);
    // vkCreateImageView �̌��ʂ������ł͂Ȃ��ꍇ�A�G���[���X���[���܂�
    if (!imageView)
    {
        throw std::runtime_error("Failed to create an Image View!");
    }

    // �쐬���� Image View �̃n���h����Ԃ��܂�
    return imageView;

}


vk::SurfaceCapabilitiesKHR SwapchainGenerator::GetSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceCapabilitiesKHR(surface);
}

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
