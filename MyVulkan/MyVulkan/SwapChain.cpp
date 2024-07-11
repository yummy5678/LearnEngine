#include "SwapChain.h"


//vk::UniqueSwapchainKHR SwapChain::create()
//{
//	// Swap Chain�̏ڍׂ��擾���čœK�Ȑݒ��I������
//	SwapChainDetails swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);
//
//	// Swap Chain�̍œK��Surface�t�H�[�}�b�g��I������
//	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapChainDetails.formats);
//
//	// Swap Chain�̍œK�ȃv���[���e�[�V�������[�h��I������
//	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentationModes);
//
//	// Swap Chain�̍œK��Extent�i�T�C�Y�j��I������
//	VkExtent2D extent = chooseSwapExtent(swapChainDetails.surfaceCapabilities);
//
//	// Swap Chain�Ɋ܂܂��摜�̐������肷��
//	uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;
//
//	// ����imageCount��max�𒴂��Ă�����max�ɐ�������
//	if (swapChainDetails.surfaceCapabilities.maxImageCount > 0
//		&& swapChainDetails.surfaceCapabilities.maxImageCount < imageCount)
//	{
//		imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;
//	}
//
//
//	/*//////////////////////////
//	* �X���b�v�`�F�C���̍쐬
//	*///////////////////////////
//	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
//	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;					// �X���b�v�`�F�C���̃^�C�v
//	swapChainCreateInfo.surface = surface;														// �X���b�v�`�F�C���̑ΏۂƂȂ�Surface
//	swapChainCreateInfo.imageFormat = surfaceFormat.format;										// �X���b�v�`�F�C���̉摜�t�H�[�}�b�g
//	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;								// �X���b�v�`�F�C���̃J���[�X�y�[�X
//	swapChainCreateInfo.presentMode = presentMode;												// �X���b�v�`�F�C���̃v���[���e�[�V�������[�h
//	swapChainCreateInfo.imageExtent = extent;													// �X���b�v�`�F�C���̉摜�̃T�C�Y
//	swapChainCreateInfo.minImageCount = imageCount;												// �X���b�v�`�F�C�����̍ŏ��摜��
//	swapChainCreateInfo.imageArrayLayers = 1;													// �摜�̔z�񃌃C���[��
//	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;						// �摜���ǂ̂悤�Ɏg�p����邩
//	swapChainCreateInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;	// Swapchain�摜�ɑ΂��čs���ϊ�
//	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;						// �O���O���t�B�b�N�X�Ƃ̃u�����f�B���O�̈�����
//	swapChainCreateInfo.clipped = VK_TRUE;														// ��ʊO�̕������N���b�v���邩�ǂ���
//
//	// �L���[�t�@�~���[�C���f�b�N�X���擾����
//	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);
//
//	// �O���t�B�N�X�t�@�~���[�ƃv���[���e�[�V�����t�@�~���[���قȂ�ꍇ�A�摜���t�@�~���[�Ԃŋ��L����K�v������
//	if (indices.graphicsFamily != indices.presentationFamily)
//	{
//		// �قȂ�ꍇ
//		// ���L����L���[�̎w��
//		uint32_t queueFamilyIndices[] = {
//			static_cast<uint32_t>(indices.graphicsFamily),
//			static_cast<uint32_t>(indices.presentationFamily)
//		};
//
//		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;		// �摜���L�̃��[�h
//		swapChainCreateInfo.queueFamilyIndexCount = 2;							// �摜�����L����L���[�̐�
//		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;			// �摜�����L����L���[�̔z��
//	}
//	else
//	{
//		// �����������ꍇ
//		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;		// �摜���L�̃��[�h
//		swapChainCreateInfo.queueFamilyIndexCount = 0;							// �摜�����L����L���[�̐�
//		swapChainCreateInfo.pQueueFamilyIndices = nullptr;						// �摜�����L����L���[�̔z��
//	}
//
//	// �Â�Swapchain���j������A�V����Swapchain���u����������ꍇ�A��Swapchain�������N���ĐӔC��v���Ɉ����p��
//	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
//
//	// Swapchain���쐬����
//	swapchain = mainDevice.logicalDevice->createSwapchainKHRUnique(swapChainCreateInfo);
//	if (!swapchain)
//	{
//		throw std::runtime_error("Swapchain�̍쐬�Ɏ��s���܂����I");
//	}
//
//	// ��ŎQ�Ƃ��邽�߂ɕۑ�����
//	swapChainImageFormat = surfaceFormat.format;
//	swapChainExtent = extent;
//
//}
