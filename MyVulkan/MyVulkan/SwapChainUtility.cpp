#include "SwapChainUtility.h"


SwapChainDetails SwapChainUtility::getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// ����̕����f�o�C�X�ƃT�[�t�F�X�ɑ΂���\�ʂ̃L���r�e�B���擾����
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainDetails.surfaceCapabilities);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// �t�H�[�}�b�g���Ԃ��ꂽ�ꍇ�A�t�H�[�}�b�g�̃��X�g���擾����
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainDetails.formats.data());
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// �v���[���e�[�V�������[�h���Ԃ��ꂽ�ꍇ�A�v���[���e�[�V�������[�h�̃��X�g���擾����
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}

	return swapChainDetails;
}


/// <summary>
/// �X���b�v�`�F�C���̍쐬
/// </summary>
vk::UniqueSwapchainKHR SwapChainUtility::createSwapchain(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // �X���b�v�`�F�C���̐ݒ���s��
    // �T�[�t�F�X�̋@�\���擾
    vk::SurfaceCapabilitiesKHR surfaceCapabilities = getSurfaceCapabilities(physicalDevice, surface);

    // �X���b�v�`�F�C���̐ݒ�
    // �K�؂ȃT�[�t�F�X�t�H�[�}�b�g��I��
    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(getSurfaceFormats(physicalDevice, surface));
    // �K�؂ȃv���[���e�[�V�������[�h��I��
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(getPresentModes(physicalDevice, surface));
    // �X���b�v�`�F�C���̃G�N�X�e���g�i���ƍ����j��I��
    vk::Extent2D extent = chooseSwapExtent(surfaceCapabilities);

    // �X���b�v�`�F�C���ɕK�v�ȃC���[�W�̐�������
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // �X���b�v�`�F�C���쐬���̐ݒ�
    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.pNext;                       // �g���`�F�[���ւ̃|�C���^�i�K�v�ɉ����Đݒ�j
    createInfo.flags;                       // �쐬�t���O�i�K�v�ɉ����Đݒ�j
    createInfo.surface = surface;           // �X���b�v�`�F�C�����^�[�Q�b�g�Ƃ���T�[�t�F�X
    createInfo.minImageCount = imageCount;  // �X���b�v�`�F�C���Ɋ܂܂��ŏ��C���[�W��
    createInfo.imageFormat = surfaceFormat.format;          // �C���[�W�t�H�[�}�b�g
    createInfo.imageColorSpace = surfaceFormat.colorSpace;  // �J���[�X�y�[�X
    createInfo.imageExtent = extent;        // �X���b�v�`�F�C���̕��ƍ���
    createInfo.imageArrayLayers = 1;        // �C���[�W�̔z��w��
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // �C���[�W�̎g�p�p�r
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;          // �C���[�W�̋��L���[�h�i�����ݒ�͔r���I���[�h�j
    createInfo.queueFamilyIndexCount;                                   // �L���[�t�@�~���[�C���f�b�N�X�̐�
    createInfo.pQueueFamilyIndices;                                     // �L���[�t�@�~���[�C���f�b�N�X�̔z��ւ̃|�C���^
    createInfo.preTransform = surfaceCapabilities.currentTransform;     // �T�[�t�F�X�̃g�����X�t�H�[��
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // �A���t�@�������[�h
    createInfo.presentMode = presentMode;   // �v���[���e�[�V�������[�h
    createInfo.clipped = VK_TRUE;           // �N���b�v����邩�ǂ���
    createInfo.oldSwapchain = nullptr;      // �Â��X���b�v�`�F�C���ւ̃|�C���^

    // �O���t�B�b�N�X�L���[�t�@�~���̃C���f�b�N�X���擾����
    // �L���[�t�@�~���̃C���f�b�N�X���擾
    QueueFamilyIndices indices = QueueUtility::getQueueFamilies(physicalDevice, surface);
    // �L���[�t�@�~���[�C���f�b�N�X�̔z����쐬
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentationFamily };

    // �L���[�t�@�~�����قȂ�ꍇ�͋��L���[�h��ݒ�
    if (indices.graphicsFamily != indices.presentationFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent; // ���s���[�h�ɐݒ�
        createInfo.queueFamilyIndexCount = 2;                       // �L���[�t�@�~���[�C���f�b�N�X�̐���ݒ�
        createInfo.pQueueFamilyIndices = queueFamilyIndices;        // �L���[�t�@�~���[�C���f�b�N�X�̔z���ݒ�
    }

    // �X���b�v�`�F�C�����쐬����
    // �X���b�v�`�F�C�����쐬
    vk::UniqueSwapchainKHR swapchain = device.createSwapchainKHRUnique(createInfo);
    // �G���[�`�F�b�N���s��
    if (!swapchain)
    {
        throw std::runtime_error("�X���b�v�`�F�C���̍쐬�Ɏ��s���܂���!");  // �G���[�����������ꍇ�͗�O�𓊂���
    }

    return swapchain;  // �쐬�����X���b�v�`�F�C����Ԃ�
}



// �X���b�v�`�F�C���̃t�H�[�}�b�g��I������֐�
vk::SurfaceFormatKHR SwapChainUtility::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
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
vk::PresentModeKHR SwapChainUtility::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
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
vk::Extent2D SwapChainUtility::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
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

vk::SurfaceCapabilitiesKHR SwapChainUtility::getSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceCapabilitiesKHR(surface);
}

std::vector<vk::SurfaceFormatKHR> SwapChainUtility::getSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceFormatsKHR(surface);
}

std::vector<vk::PresentModeKHR> SwapChainUtility::getPresentModes(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfacePresentModesKHR(surface);
}
