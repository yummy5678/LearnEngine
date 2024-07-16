#include "Renderer.h"

VulkanRenderer::VulkanRenderer()
{
}

int VulkanRenderer::init(GameWindow renderWindow)
{
	window = renderWindow.getWindowPointer(); //�E�B���h�E�̃|�C���^�̃Z�b�g

	try {
		createInstance();
		createDebugCallback();
		//createSurface();
		surface			= SurfaceUtilities::createUnique(instance.get(), window);
		physicalDevice	= DeviceUtility::getPhysicalDevice(instance.get(), surface.get());
		logicalDevice   = DeviceUtility::createLogicalDevice(physicalDevice, surface.get());
		swapchain		= SwapChainUtility::createSwapchain(logicalDevice.get(), physicalDevice, surface.get());
		
		renderPass		= RenderPassUtility::createRenderPass(logicalDevice.get(), physicalDevice, surface.get());
		pipelineLayout  = GraphicsPipelineUtility::createPipelineLayout(logicalDevice.get());
		graphicsPipeline = GraphicsPipelineUtility::createGraphicsPipeline(logicalDevice.get(), swapChainExtent, pipelineLayout.get(), renderPass.get());

		swapChainImages = ;//swapChainImages���쐬���Ă���
		swapChainFramebuffers = SwapChainUtility::createFramebuffers(logicalDevice.get(), swapChainImages, renderPass.get(), swapChainExtent);
		createCommandPool();
		createCommandBuffers();
		recordCommands();
		createSynchronisation();
	}
	catch (const std::runtime_error& e) {
		//�G���[���b�Z�[�W�󂯎��
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//void VulkanRenderer::draw()
//{
//	// -- GET NEXT IMAGE --
//// Wait for given fence to signal (open) from last draw before continuing
//	vkWaitForFences(mainDevice.logicalDevice.get(), 1, &drawFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
//	// Manually reset (close) fences
//	vkResetFences(mainDevice.logicalDevice.get(), 1, &drawFences[currentFrame]);
//
//	// Get index of next image to be drawn to, and signal semaphore when ready to be drawn to
//	uint32_t imageIndex;
//	vkAcquireNextImageKHR(mainDevice.logicalDevice.get(), swapchain.get(), std::numeric_limits<uint64_t>::max(), imageAvailable[currentFrame], VK_NULL_HANDLE, &imageIndex);
//
//	// -- SUBMIT COMMAND BUFFER TO RENDER --
//	// Queue submission information
//	VkSubmitInfo submitInfo = {};
//	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//	submitInfo.waitSemaphoreCount = 1;										// Number of semaphores to wait on
//	submitInfo.pWaitSemaphores = &imageAvailable[currentFrame];				// List of semaphores to wait on
//	VkPipelineStageFlags waitStages[] = {
//		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
//	};
//	submitInfo.pWaitDstStageMask = waitStages;						// Stages to check semaphores at
//	submitInfo.commandBufferCount = 1;								// Number of command buffers to submit
//	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];		// Command buffer to submit
//	submitInfo.signalSemaphoreCount = 1;							// Number of semaphores to signal
//	submitInfo.pSignalSemaphores = &renderFinished[currentFrame];	// Semaphores to signal when command buffer finishes
//
//	// Submit command buffer to queue
//	VkResult result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, drawFences[currentFrame]);
//	if (result != VK_SUCCESS)
//	{
//		throw std::runtime_error("Failed to submit Command Buffer to Queue!");
//	}
//
//
//	// -- PRESENT RENDERED IMAGE TO SCREEN --
//	VkPresentInfoKHR presentInfo = {};
//	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//	presentInfo.waitSemaphoreCount = 1;										// Number of semaphores to wait on
//	presentInfo.pWaitSemaphores = &renderFinished[currentFrame];			// Semaphores to wait on
//	presentInfo.swapchainCount = 1;											// Number of swapchains to present to
//	presentInfo.pSwapchains = &swapchain;									// Swapchains to present images to
//	presentInfo.pImageIndices = &imageIndex;								// Index of images in swapchains to present
//
//	// Present image
//	result = vkQueuePresentKHR(presentationQueue, &presentInfo);
//	if (result != VK_SUCCESS)
//	{
//		throw std::runtime_error("Failed to present Image!");
//	}
//
//	// Get next frame (use % MAX_FRAME_DRAWS to keep value below MAX_FRAME_DRAWS)
//	currentFrame = (currentFrame + 1) % MAX_FRAME_DRAWS;
//}

void VulkanRenderer::cleanup()
{
	//// Wait until no actions being run on device before destroying
	//vkDeviceWaitIdle(mainDevice.logicalDevice);

	//for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	//{
	//	vkDestroySemaphore(mainDevice.logicalDevice, renderFinished[i], nullptr);
	//	vkDestroySemaphore(mainDevice.logicalDevice, imageAvailable[i], nullptr);
	//	vkDestroyFence(mainDevice.logicalDevice, drawFences[i], nullptr);
	//}
	//vkDestroyCommandPool(mainDevice.logicalDevice, graphicsCommandPool, nullptr);
	//for (auto framebuffer : swapChainFramebuffers)
	//{
	//	vkDestroyFramebuffer(mainDevice.logicalDevice, framebuffer, nullptr);
	//}
	//vkDestroyPipeline(mainDevice.logicalDevice, graphicsPipeline, nullptr);
	//vkDestroyPipelineLayout(mainDevice.logicalDevice, pipelineLayout, nullptr);
	//vkDestroyRenderPass(mainDevice.logicalDevice, renderPass, nullptr);
	//for (auto image : swapChainImages)
	//{
	//	vkDestroyImageView(mainDevice.logicalDevice, image.imageView, nullptr);
	//}
	//vkDestroySwapchainKHR(mainDevice.logicalDevice, swapchain, nullptr);
	//vkDestroySurfaceKHR(instance, surface, nullptr);
	//vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	//if (validationEnabled)
	//{
	//	DestroyDebugReportCallbackEXT(instance, callback, nullptr);
	//}
	//vkDestroyInstance(instance, nullptr);
}



VulkanRenderer::~VulkanRenderer()
{
}

void VulkanRenderer::createInstance()
{

	// �G���[�`�F�b�N
	if (validationEnabled && !checkValidationLayerSupport())
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}

	// �A�v���P�[�V�����̏���������
	// �����͍D���Ȓl����������
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";                     // �A�v���P�[�V�����̖��O
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);       // �A�v���P�[�V�����̃o�[�W������ 
	appInfo.pEngineName = "No Engine";                           // �G���W���̖��O
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);            // �G���W���̃o�[�W������
	appInfo.apiVersion = VK_API_VERSION_1_0;                     // Vulkan API�̃o�[�W����

	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	// �C���X�^���X�̃^�C�v
	createInfo.pNext;											// �g���@�\�̏�� 
	createInfo.flags;											// �C���X�^���X�̍쐬�t���O 
	createInfo.pApplicationInfo = &appInfo;						// �A�v���P�[�V�������ւ̃|�C���^
	createInfo.enabledLayerCount = 0;							// �L���ɂ��郌�C���[�̐� 
	createInfo.ppEnabledLayerNames = nullptr;					// �L���ɂ��郌�C���[�̖��O�̔z�� 
	createInfo.enabledExtensionCount;							// �L���ɂ���g���@�\�̐� 
	createInfo.ppEnabledExtensionNames;							// �L���ɂ���g���@�\�̖��O�̔z�� 	

	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	// �C���X�^���X���g�p����g���@�\��ݒ肷��
	uint32_t glfwExtensionCount = 0;	// GLFW�͕����̊g���@�\��v������ꍇ������
	const char** glfwExtensions;		// �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^(�z��)�̃|�C���^(C������)���K�v

	// GLFW�̊g���@�\���擾����
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFW�̊g���@�\�����X�g�ɒǉ�����
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	// �o���f�[�V�������L���ȏꍇ�A���ؗp�̃f�o�b�O���g���@�\��ǉ�����
	if (validationEnabled)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// �C���X�^���X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�m�F����
	if (!checkInstanceExtensionSupport(&instanceExtensions))
	{
		throw std::runtime_error("VkInstance �͕K�v�Ȋg���@�\���T�|�[�g���Ă��܂���I");
	}

	// �L���Ȋg���@�\�̐��Ɩ��O�̔z���ݒ肷��
	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// �o���f�[�V�������L���ȏꍇ�A�L���ȃ��C���[�̐��Ɩ��O�̔z���ݒ肷��
	if (validationEnabled)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}	

	// �C���X�^���X���쐬����
	instance = vk::createInstanceUnique(createInfo);

	// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ�̃G���[���b�Z�[�W
	if (!instance) {
		throw std::runtime_error("Vulkan�C���X�^���X�̍쐬�Ɏ��s���܂����I");
	}
}

void VulkanRenderer::createDebugCallback()
{
	// �o���f�[�V�������L���ȏꍇ�̂݃R�[���o�b�N���쐬����
	if (!validationEnabled) return;

	// �f�o�b�O���|�[�g�R�[���o�b�N�̍쐬��������������
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;				// �f�o�b�O���|�[�g�R�[���o�b�N�̍쐬���̍\���̂̃^�C�v���w��
	callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;		// �Ăяo�����J�n����o���f�[�V�������|�[�g�̎��
	callbackCreateInfo.pfnCallback = debugCallback;													// �R�[���o�b�N�֐��ւ̃|�C���^

	// �J�X�^���̍쐬�֐����g�p���ăf�o�b�O�R�[���o�b�N���쐬����
	VkResult result = CreateDebugReportCallbackEXT(instance.get(), &callbackCreateInfo, nullptr, &callback);
	if (result != VK_SUCCESS)
	{
		//throw std::runtime_error("Failed to create Debug Callback!");
		throw std::runtime_error("�f�o�b�O�R�[���o�b�N�̍쐬�Ɏ��s���܂����I");
	}
}

/// <summary>
/// �_���f�o�C�X�̍쐬
/// </summary>
void VulkanRenderer::createLogicalDevice()
{
	//�_���f�o�C�X�̍쐬�ɕK�v�Ȃ���
	//1,�g�p����f�o�C�X�̊g��
	//2,�g�p����f�o�C�X�̃��C���[
	//3,�f�o�C�X�̂ǂ̃L���[���g�p���邩

	// �I�����������f�o�C�X�̃L���[�t�@�~���[�C���f�b�N�X���擾����
	QueueFamilyIndices queueIndex = getQueueFamilies(physicalDevice);

	// �L���[�쐬���p�̃x�N�^�[
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	//�t�@�~���[�C���f�b�N�X�p�̃Z�b�g
	std::set<int> queueFamilyIndices = { queueIndex.graphicsFamily, queueIndex.presentationFamily };

	// �_���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;                        // �L���[���쐬����t�@�~���[�̃C���f�b�N�X
		queueCreateInfo.queueCount = 1;                                             // �쐬����L���[�̐�
		float priority = 1.0f;														// �D��x
		queueCreateInfo.pQueuePriorities = &priority;                               // Vulkan�͕����̃L���[���ǂ̂悤�Ɉ�������m��K�v�����邽�߁A�D��x���w�肷�� (1���ō��D��x)

		queueCreateInfos.push_back(queueCreateInfo);
	}

	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // �L���[�쐬���̐�
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g

	// �_���f�o�C�X���g�p���镨���f�o�C�X�̋@�\
	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;            // ���W�J���f�o�C�X���g�p���镨���f�o�C�X�̋@�\

	// �w�肳�ꂽ�����f�o�C�X�ɑ΂��ă��W�J���f�o�C�X���쐬����
	logicalDevice = physicalDevice.createDeviceUnique(deviceCreateInfo);
	if (!logicalDevice)
	{
		throw std::runtime_error("���W�J���f�o�C�X�̍쐬�Ɏ��s���܂����I");
	}

	// �L���[�̓f�o�C�X�Ɠ����ɍ쐬����邽��
	// �L���[�ւ̃n���h�����擾����
	// �w�肳�ꂽ���W�J���f�o�C�X����A�w�肳�ꂽ�L���[�t�@�~���[�̎w�肳�ꂽ�L���[�C���f�b�N�X�i0�͗B��̃L���[�Ȃ̂�0�j�A�w�肳�ꂽVkQueue�ւ̎Q�Ƃ�u��
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.presentationFamily, 0, &presentationQueue);
}

//void VulkanRenderer::createSurface()
//{
//
//	VkSurfaceKHR c_surface;
//	// �T�[�t�F�X���쐬����i�T�[�t�F�X�쐬���\���̂��쐬���A�T�[�t�F�X�쐬�֐������s���A���ʂ�Ԃ��j
//	VkResult result = glfwCreateWindowSurface(instance.get(), window, nullptr, &c_surface);
//
//	if (result != VK_SUCCESS)
//	{
//		throw std::runtime_error("�T�[�t�F�X�̍쐬�Ɏ��s���܂����I");
//	}
//
//	// �R���X�g���N�^�̑������Őe�ƂȂ�C���X�^���X�̏���n��
//	surface = vk::UniqueSurfaceKHR{ c_surface, instance.get() };
//}


//void VulkanRenderer::createSwapChain()
//{
//	// Swap Chain�̏ڍׂ��擾���čœK�Ȑݒ��I������
//	SwapChainDetails swapChainDetails = getSwapChainDetails(physicalDevice);
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
//	swapChainCreateInfo.surface = surface.get();												// �X���b�v�`�F�C���̑ΏۂƂȂ�Surface
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
//	QueueFamilyIndices indices = getQueueFamilies(physicalDevice);
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
//	swapchain = logicalDevice->createSwapchainKHRUnique(swapChainCreateInfo);
//	if (!swapchain)
//	{
//		throw std::runtime_error("Swapchain�̍쐬�Ɏ��s���܂����I");
//	}
//
//	// ��ŎQ�Ƃ��邽�߂ɕۑ�����
//	swapChainImageFormat = surfaceFormat.format;
//	swapChainExtent = extent;
//
//	// Swapchain�̉摜���擾����i�����擾���A�l���擾����j
//	uint32_t swapChainImageCount;	//�X���b�v�`�F�C���̉摜��
//	vkGetSwapchainImagesKHR(logicalDevice.get(), swapchain.get(), &swapChainImageCount, nullptr);
//	std::vector<VkImage> images(swapChainImageCount);
//	vkGetSwapchainImagesKHR(logicalDevice.get(), swapchain.get(), &swapChainImageCount, images.data());
//
//	for (VkImage image : images)
//	{
//		// �摜�n���h����ۑ�����
//		SwapchainImage swapChainImage = {};
//		swapChainImage.image = image;
//		swapChainImage.imageView = createImageView(image, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
//
//		// Swapchain�摜���X�g�ɒǉ�����
//		swapChainImages.push_back(swapChainImage);
//	}
//}

void VulkanRenderer::createGraphicsPipeline()
{
	//// �V�F�[�_�[��SPIR-V�R�[�h��ǂݍ���
	//auto vertexShaderCode = readFile("Shaders/vert.spv");
	//auto fragmentShaderCode = readFile("Shaders/frag.spv");

	//// �V�F�[�_�[���W���[�����쐬����
	//VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
	//VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

	//// -- �V�F�[�_�[�X�e�[�W�쐬��� --
	//// ���_�V�F�[�_�[�X�e�[�W�̍쐬���
	//VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	//vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;				// �V�F�[�_�[�X�e�[�W�̎��
	//vertexShaderCreateInfo.module = vertexShaderModule;						// �g�p����V�F�[�_�[���W���[��
	//vertexShaderCreateInfo.pName = "main";									// �G���g���[�|�C���g

	//// �t���O�����g�V�F�[�_�[�X�e�[�W�̍쐬���
	//VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	//fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;				// �V�F�[�_�[�X�e�[�W�̎��
	//fragmentShaderCreateInfo.module = fragmentShaderModule;						// �g�p����V�F�[�_�[���W���[��
	//fragmentShaderCreateInfo.pName = "main";									// �G���g���[�|�C���g

	//// �V�F�[�_�[�X�e�[�W�쐬����z��Ɋi�[
	//VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };


	//// -- ���_���� (TODO: ���\�[�X���쐬���ꂽ�璸�_�̐�����ǉ�����) --
	//VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
	//vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	//vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// ���_�o�C���f�B���O�̐��� (�f�[�^�̊Ԋu��X�g���C�h���)
	//vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	//vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// ���_�����̐��� (�f�[�^�t�H�[�}�b�g��o�C���h��/��)


	//// -- ���̓A�Z���u�� --
	//VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	//inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	//inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;		// ���_���A�Z���u�������{�}�`�̎��
	//inputAssembly.primitiveRestartEnable = VK_FALSE;					// "strip" �g�|���W�[���㏑�����ĐV�����v���~�e�B�u���J�n���邱�Ƃ������邩


	//// -- �r���[�|�[�g & �V�U�[ --
	//// �r���[�|�[�g���̍\���̂��쐬
	//VkViewport viewport = {};
	//viewport.x = 0.0f;									// x �J�n���W
	//viewport.y = 0.0f;									// y �J�n���W
	//viewport.width = (float)swapChainExtent.width;		// �r���[�|�[�g�̕�
	//viewport.height = (float)swapChainExtent.height;	// �r���[�|�[�g�̍���
	//viewport.minDepth = 0.0f;							// �t���[���o�b�t�@�̍ŏ��[�x
	//viewport.maxDepth = 1.0f;							// �t���[���o�b�t�@�̍ő�[�x

	//// �V�U�[���̍\���̂��쐬
	//VkRect2D scissor = {};
	//scissor.offset = { 0,0 };							// �g�p����̈�̃I�t�Z�b�g
	//scissor.extent = swapChainExtent;					// �g�p����̈�͈̔͂ƃI�t�Z�b�g����J�n

	//VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	//viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	//viewportStateCreateInfo.viewportCount = 1;
	//viewportStateCreateInfo.pViewports = &viewport;
	//viewportStateCreateInfo.scissorCount = 1;
	//viewportStateCreateInfo.pScissors = &scissor;


	//// -- �_�C�i�~�b�N�X�e�[�g --
	//// �L���ɂ���_�C�i�~�b�N�X�e�[�g
	////std::vector<VkDynamicState> dynamicStateEnables;
	////dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	// �_�C�i�~�b�N�r���[�|�[�g: vkCmdSetViewport(commandbuffer, 0, 1, &viewport); �ŃR�}���h�o�b�t�@���Ń��T�C�Y�\
	////dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	// �_�C�i�~�b�N�V�U�[: vkCmdSetScissor(commandbuffer, 0, 1, &scissor); �ŃR�}���h�o�b�t�@���Ń��T�C�Y�\

	////// �_�C�i�~�b�N�X�e�[�g�̍쐬���
	////VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	////dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	////dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	////dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();


	//// -- ���X�^���C�U --
	//VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
	//rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	//rasterizerCreateInfo.depthClampEnable = VK_FALSE;			// �t���O�����g���ߐ�/���u���ʂ𒴂����ꍇ�ɃN���b�v (�f�t�H���g) �܂��͕��ʂɃN�����v���邩��ύX
	//rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;	// �f�[�^��j�������X�^���C�U���X�L�b�v���邩�ǂ����B�t���[���o�b�t�@�o�͂Ȃ��̃p�C�v���C���ɂ̂ݓK���Ă���
	//rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;	// ���_�Ԃ̃|�C���g�̓h��Ԃ����@
	//rasterizerCreateInfo.lineWidth = 1.0f;						// �`�掞�̐��̑���
	//rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;		// �O�p�`�̂ǂ̖ʂ��J�����O���邩
	//rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;	// �O�ʂ����肷�邽�߂̊�������
	//rasterizerCreateInfo.depthBiasEnable = VK_FALSE;			// �t���O�����g�ɐ[�x�o�C�A�X��ǉ����邩 (�V���h�E�}�b�s���O�� "�e�A�N�l" ��h���̂ɗL��)


	//// -- �}���`�T���v�����O --
	//VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = {};
	//multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	//multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;					// �}���`�T���v���V�F�[�f�B���O��L���ɂ��邩�ǂ���
	//multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;	// �t���O�����g���ƂɎg�p����T���v����


	//// -- �u�����f�B���O --
	//// �u�����f�B���O�́A�������܂��V�����F�ƌÂ��l���ǂ̂悤�Ƀu�����h���邩�����肷��

	//// �u�����h�A�^�b�`�����g�X�e�[�g (�u�����f�B���O�̏������@)
	//VkPipelineColorBlendAttachmentState colourState = {};
	//colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT	// �u�����f�B���O��K�p����F
	//	| VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	//colourState.blendEnable = VK_TRUE;													// �u�����f�B���O��L���ɂ��邩�ǂ���

	//// �u�����f�B���O�͕��������g�p: (srcColorBlendFactor * �V�����F) colorBlendOp (dstColorBlendFactor * �Â��F)
	//colourState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//colourState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//colourState.colorBlendOp = VK_BLEND_OP_ADD;

	//// �v��: (VK_BLEND_FACTOR_SRC_ALPHA * �V�����F) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * �Â��F)
	//// (�V�����F�̃A���t�@ * �V�����F) + ((1 - �V�����F�̃A���t�@) * �Â��F)

	//colourState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//colourState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//colourState.alphaBlendOp = VK_BLEND_OP_ADD;
	//// �v��: (1 * �V�����A���t�@) + (0 * �Â��A���t�@) = �V�����A���t�@

	//VkPipelineColorBlendStateCreateInfo colourBlendingCreateInfo = {};
	//colourBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	//colourBlendingCreateInfo.logicOpEnable = VK_FALSE;				// �v�Z�̑���ɘ_�����Z���g�p���邩�ǂ���
	//colourBlendingCreateInfo.attachmentCount = 1;
	//colourBlendingCreateInfo.pAttachments = &colourState;


	//// -- �p�C�v���C�����C�A�E�g (TODO: �����̃f�B�X�N���v�^�Z�b�g���C�A�E�g��K�p����) --
	//VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	//pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	//pipelineLayoutCreateInfo.setLayoutCount = 0;
	//pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	//pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	//pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	//// �p�C�v���C�����C�A�E�g���쐬
	//VkResult result = vkCreatePipelineLayout(logicalDevice.get(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout.get());
	//if (result != VK_SUCCESS)
	//{
	//	throw std::runtime_error("�p�C�v���C�����C�A�E�g�̍쐬�Ɏ��s���܂����I");
	//}


	//// -- �[�x�X�e���V���e�X�g --
	//// TODO: �[�x�X�e���V���e�X�g�̐ݒ�


	//// -- �O���t�B�b�N�X�p�C�v���C���̍쐬 --
	//VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	//pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//pipelineCreateInfo.stageCount = 2;									// �V�F�[�_�[�X�e�[�W�̐�
	//pipelineCreateInfo.pStages = shaderStages;							// �V�F�[�_�[�X�e�[�W�̃��X�g
	//pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;		// ���ׂĂ̌Œ�@�\�p�C�v���C���X�e�[�g
	//pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	//pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	//pipelineCreateInfo.pDynamicState = nullptr;
	//pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	//pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	//pipelineCreateInfo.pColorBlendState = &colourBlendingCreateInfo;
	//pipelineCreateInfo.pDepthStencilState = nullptr;
	//pipelineCreateInfo.layout = pipelineLayout.get();							// �p�C�v���C�����g�p����p�C�v���C�����C�A�E�g
	//pipelineCreateInfo.renderPass = renderPass.get();							// �p�C�v���C�����݊����̂��郌���_�[�p�X�̐���
	//pipelineCreateInfo.subpass = 0;										// �p�C�v���C���Ŏg�p����T�u�p�X

	//// �p�C�v���C���̔h��: �œK���̂��߂ɑ��݂ɔh�����镡���̃p�C�v���C�����쐬�ł���
	//pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;              // �h�����̊����̃p�C�v���C��...
	//pipelineCreateInfo.basePipelineIndex = -1;                           // �܂��͍쐬���̃p�C�v���C���̃C���f�b�N�X (�����쐬����ꍇ)

	//// �O���t�B�b�N�X�p�C�v���C�����쐬
	//result = vkCreateGraphicsPipelines(logicalDevice.get(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline.get());
	//if (result != VK_SUCCESS)
	//{
	//	throw std::runtime_error("�O���t�B�b�N�X�p�C�v���C���̍쐬�Ɏ��s���܂����I");
	//}

	////�p�C�v���C���̍쐬��ɕs�v�ɂȂ����V�F�[�_�[���W���[����j��
	//vkDestroyShaderModule(logicalDevice.get(), fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice.get(), vertexShaderModule, nullptr);
}


void VulkanRenderer::createFramebuffers()
{
	//// �X���b�v�`�F�C���̉摜���ɍ��킹�ăt���[���o�b�t�@�̐������T�C�Y����
	//swapChainFramebuffers.resize(swapChainImages.size());

	//// �e�X���b�v�`�F�C���摜�ɑ΂��ăt���[���o�b�t�@���쐬����
	//for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	//{
	//	// VkImageView�̊m�F
	//	if (swapChainImages[i].imageView == VK_NULL_HANDLE)
	//	{
	//		throw std::runtime_error("ImageView�������ł��I");
	//	}

	//	// �t���[���o�b�t�@�ɓY�t����C���[�W�r���[�̔z����쐬����
	//	std::array<VkImageView, 1> attachments = {
	//		swapChainImages[i].imageView
	//	};

	//	// �t���[���o�b�t�@�̍쐬�ɕK�v�ȏ���ݒ肷��
	//	VkFramebufferCreateInfo framebufferCreateInfo = {};
	//	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//	framebufferCreateInfo.renderPass = renderPass.get();                                     // �t���[���o�b�t�@���g�p����郌���_�[�p�X�̃��C�A�E�g
	//	framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	//	framebufferCreateInfo.pAttachments = attachments.data();                           // �Y�t����C���[�W�r���[�̃��X�g (�����_�[�p�X��1:1�Ή�)
	//	framebufferCreateInfo.width = swapChainExtent.width;                                // �t���[���o�b�t�@�̕�
	//	framebufferCreateInfo.height = swapChainExtent.height;                              // �t���[���o�b�t�@�̍���
	//	framebufferCreateInfo.layers = 1;                                                    // �t���[���o�b�t�@�̃��C���[��

	//	// �t���[���o�b�t�@���쐬����
	//	VkResult result = vkCreateFramebuffer(logicalDevice.get(), &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
	//	if (result != VK_SUCCESS)
	//	{
	//		throw std::runtime_error("�t���[���o�b�t�@�̍쐬�Ɏ��s���܂����I");
	//	}
	//}
}

void VulkanRenderer::createCommandPool()
{
	// �f�o�C�X����L���[�t�@�~���[�̃C���f�b�N�X���擾����
	QueueFamilyIndices queueFamilyIndices = getQueueFamilies(physicalDevice);

	// �R�}���h�v�[���̍쐬�ɕK�v�ȏ���ݒ肷��
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;   // ���̃R�}���h�v�[�����g�p����L���[�t�@�~���[�^�C�v

	// �O���t�B�b�N�X�L���[�t�@�~���[�p�̃R�}���h�v�[�����쐬����
	VkResult result = vkCreateCommandPool(logicalDevice.get(), &poolInfo, nullptr, &graphicsCommandPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�R�}���h�v�[���̍쐬�Ɏ��s���܂����I");
	}
}

void VulkanRenderer::createCommandBuffers()
{
	// �R�}���h�o�b�t�@�̐����t���[���o�b�t�@���ƂɃ��T�C�Y����
	commandBuffers.resize(swapChainFramebuffers.size());

	// �R�}���h�o�b�t�@�����蓖�Ă邽�߂̏���ݒ肷��
	VkCommandBufferAllocateInfo cbAllocInfo = {};
	cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbAllocInfo.commandPool = graphicsCommandPool;                                  // �R�}���h�o�b�t�@�����蓖�Ă�R�}���h�v�[��
	cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                            // �R�}���h�o�b�t�@�̃��x�� (PRIMARY: ���ڃL���[�ɑ��M����o�b�t�@)
	cbAllocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());  // ���蓖�Ă�R�}���h�o�b�t�@�̐�

	// �R�}���h�o�b�t�@�����蓖�ĂāA���̃n���h�����o�b�t�@�̔z��Ɋi�[����
	VkResult result = vkAllocateCommandBuffers(logicalDevice.get(), &cbAllocInfo, commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�R�}���h�o�b�t�@�̊��蓖�ĂɎ��s���܂����I");
	}
}

void VulkanRenderer::createSynchronisation()
{
	// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X�����T�C�Y����
	imageAvailable.resize(MAX_FRAME_DRAWS);
	renderFinished.resize(MAX_FRAME_DRAWS);
	drawFences.resize(MAX_FRAME_DRAWS);

	// �Z�}�t�H�̍쐬����ݒ肷��
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// �t�F���X�̍쐬����ݒ肷��
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // ������ԂŃV�O�i�����ꂽ��Ԃ̃t�F���X���쐬����

	// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X���쐬���郋�[�v
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		// �Z�}�t�H�ƃt�F���X���쐬����
		if (vkCreateSemaphore(logicalDevice.get(), &semaphoreCreateInfo, nullptr, &imageAvailable[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice.get(), &semaphoreCreateInfo, nullptr, &renderFinished[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice.get(), &fenceCreateInfo, nullptr, &drawFences[i]) != VK_SUCCESS)
		{
			// �쐬�Ɏ��s�����ꍇ�͗�O�𓊂���
			throw std::runtime_error("Failed to create a Semaphore and/or Fence!");
		}
	}
}



void VulkanRenderer::recordCommands()
{
	// �e�R�}���h�o�b�t�@�̊J�n���@�Ɋւ�����
	VkCommandBufferBeginInfo bufferBeginInfo = {};
	bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;   // �o�b�t�@���Ďg�p�\�ł��邱�Ƃ������t���O

	// �����_�[�p�X���J�n���邽�߂̏�� (�O���t�B�J���ȃA�v���P�[�V�����̏ꍇ�̂ݕK�v)
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass.get();                             // �J�n���郌���_�[�p�X
	renderPassBeginInfo.renderArea.offset = { 0, 0 };                        // �����_�[�p�X�̊J�n�ʒu (�s�N�Z���P��)
	renderPassBeginInfo.renderArea.extent = swapChainExtent;                 // �����_�[�p�X�����s����̈�̃T�C�Y (offset����n�܂�)
	VkClearValue clearValues[] = {
		{0.6f, 0.65f, 0.4, 1.0f}                                            // �N���A����l�̃��X�g (TODO: �[�x�A�^�b�`�����g�̃N���A�l)
	};
	renderPassBeginInfo.pClearValues = clearValues;                          // �N���A����l�̃��X�g
	renderPassBeginInfo.clearValueCount = 1;

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		renderPassBeginInfo.framebuffer = swapChainFramebuffers[i].get();          // �g�p����t���[���o�b�t�@��ݒ肷��

		// �R�}���h�o�b�t�@�̋L�^���J�n����
		VkResult result = vkBeginCommandBuffer(commandBuffers[i], &bufferBeginInfo);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̊J�n�Ɏ��s���܂����I");
		}

		// �����_�[�p�X���J�n����
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		// �g�p����p�C�v���C�����o�C���h����
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.get());	//�����ŃG���[

		// �p�C�v���C�������s����
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		// �����_�[�p�X���I������
		vkCmdEndRenderPass(commandBuffers[i]);

		// �R�}���h�o�b�t�@�̋L�^���I������
		result = vkEndCommandBuffer(commandBuffers[i]);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("�R�}���h�o�b�t�@�̋L�^�̏I���Ɏ��s���܂����I");
		}
	}
}

/// <summary>
/// �����f�o�C�X(GPU)�̎擾
/// </summary>
void VulkanRenderer::getPhysicalDevice()
{
	//�C���X�^���X���畨���f�o�C�X���擾
	std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : physicalDevices)
	{
		if (checkDeviceSuitable(device))
		{
			// �K�؂ȃf�o�C�X����������
			physicalDevice = device;
			return;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");

}


bool VulkanRenderer::checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	// Vulkan�̃C���X�^���X���T�|�[�g���闘�p�\�Ȋg���@�\�̐����擾����
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// ���p�\�Ȋg���@�\�̏���ێ����邽�߂�VkExtensionProperties�̃��X�g���쐬����
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// �w�肳�ꂽ�g���@�\�����p�\�Ȋg���@�\���X�g�Ɋ܂܂�Ă��邩�`�F�b�N����
	for (const auto& checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// �g���@�\������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(checkExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// �w�肳�ꂽ�g���@�\��������Ȃ��ꍇ��false��Ԃ�
		if (!hasExtension)
		{
			return false;
		}
	}

	// ���ׂĂ̎w�肳�ꂽ�g���@�\�����������ꍇ��true��Ԃ�
	return true;
}


bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// �f�o�C�X���T�|�[�g����g���@�\�̐����擾����
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// �����g���@�\��������Ȃ��ꍇ�A���s�Ƃ���false��Ԃ�
	if (extensionCount == 0)
	{
		return false;
	}

	// �g���@�\�̏���ێ����邽�߂�VkExtensionProperties�̃��X�g���쐬����
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�`�F�b�N����
	for (const auto& deviceExtension : deviceExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// �g���@�\������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// �K�v�Ȋg���@�\��������Ȃ��ꍇ��false��Ԃ�
		if (!hasExtension)
		{
			return false;
		}
	}

	// ���ׂĂ̕K�v�Ȋg���@�\�����������ꍇ��true��Ԃ�
	return true;
}


bool VulkanRenderer::checkValidationLayerSupport()
{
	// �o���f�[�V�������C���[�̐����擾���A�K�؂ȃT�C�Y�̃x�N�^�[���쐬����
	uint32_t validationLayerCount;
	vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

	// �����o���f�[�V�������C���[��������Ȃ��ꍇ�����Ȃ��Ƃ�1�̃��C���[���K�v�ȏꍇ��false��Ԃ�
	if (validationLayerCount == 0 && validationLayers.size() > 0)
	{
		return false;
	}

	// ���p�\�ȃ��C���[�̏���ێ����邽�߂�VkLayerProperties�̃��X�g���쐬����
	std::vector<VkLayerProperties> availableLayers(validationLayerCount);
	vkEnumerateInstanceLayerProperties(&validationLayerCount, availableLayers.data());

	// �w�肳�ꂽ�o���f�[�V�������C���[�����p�\�ȃ��C���[���X�g�Ɋ܂܂�Ă��邩�`�F�b�N����
	for (const auto& validationLayer : validationLayers)
	{
		bool hasLayer = false;
		for (const auto& availableLayer : availableLayers)
		{
			// ���C���[������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(validationLayer, availableLayer.layerName) == 0)
			{
				hasLayer = true;
				break;
			}
		}

		// �w�肳�ꂽ�o���f�[�V�������C���[��������Ȃ��ꍇ��false��Ԃ�
		if (!hasLayer)
		{
			return false;
		}
	}

	// ���ׂĂ̎w�肳�ꂽ�o���f�[�V�������C���[�����������ꍇ��true��Ԃ�
	return true;
}

/// <summary>
/// �w�肳�ꂽ�����f�o�C�X�� Vulkan �����_���[�ɓK���Ă��邩�ǂ������m�F����
/// </summary>
//bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
//{
	///*
	//// �f�o�C�X���̂Ɋւ����� (ID�A���O�A�^�C�v�A�x���_�[�Ȃ�)
	//VkPhysicalDeviceProperties deviceProperties;
	//vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//// �f�o�C�X�̋@�\�Ɋւ����� (�W�I���g���V�F�[�_�[�A�e�b�Z���[�V�����V�F�[�_�[�A���C�h���C���Ȃ�)
	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	//*/

	//// �L���[�t�@�~���[�̃C���f�b�N�X���擾����
	//QueueFamilyIndices indices = getQueueFamilies(device);

	//// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g���Ă��邩�m�F����
	//bool extensionsSupported = checkDeviceExtensionSupport(device);

	//// Swapchain���L�����ǂ������m�F����
	//bool swapChainValid = false;
	//if (extensionsSupported)
	//{
	//	// ����̕����f�o�C�X�ɑ΂���Swap Chain�̏ڍׂ��擾����
	//	SwapChainDetails swapChainDetails = getSwapChainDetails(device);

	//	// Swap Chain�̗L�������m�F����B�v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
	//	swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	//}

	//// �f�o�C�X���K�����Ă��邩���m�F����
	//return indices.isValid() && extensionsSupported && swapChainValid;
//}


//QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
//{
	//QueueFamilyIndices indices;

	//// �����f�o�C�X�ɑ΂��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	//uint32_t queueFamilyCount = 0;
	//vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	//std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	//vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	//// �e�L���[�t�@�~���[�𒲂ׁA�K�v�ȃ^�C�v�̃L���[�����Ȃ��Ƃ�1�����Ă��邩�ǂ������m�F����
	//int i = 0;
	//for (const auto& queueFamily : queueFamilyList)
	//{
	//	// �܂��A�L���[�t�@�~���[�����Ȃ��Ƃ�1�̃L���[�������Ă��邩�m�F����i�L���[���Ȃ��\��������j
	//	// �L���[�̓r�b�g�t�B�[���h�ŕ����̃^�C�v���`���邱�Ƃ��ł���BVK_QUEUE_*_BIT�ƃr�b�g���Ƃ�AND���Z���s���A�K�v�ȃ^�C�v�������Ă��邩�m�F����
	//	if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
	//	{
	//		indices.graphicsFamily = i;		// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
	//	}

	//	// �L���[�t�@�~���[���v���[���e�[�V�������T�|�[�g���Ă��邩�m�F����
	//	VkBool32 presentationSupport = false;
	//	vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.get(), &presentationSupport);
	//	// �L���[���v���[���e�[�V�����^�C�v�ł��邩�ǂ������m�F����i�O���t�B�b�N�X�ƃv���[���e�[�V�����̗����ɂȂ蓾��j
	//	if (queueFamily.queueCount > 0 && presentationSupport)
	//	{
	//		indices.presentationFamily = i;
	//	}

	//	// �L���[�t�@�~���[�̃C���f�b�N�X���L���ȏ�Ԃɂ��邩�ǂ������m�F���A�����ł���Ό������~����
	//	if (indices.isValid())
	//	{
	//		break;
	//	}

	//	i++;
	//}

	//return indices;
//}


//SwapChainDetails VulkanRenderer::getSwapChainDetails(VkPhysicalDevice device)
//{
//	SwapChainDetails swapChainDetails;
//
//	// -- CAPABILITIES --
//	// ����̕����f�o�C�X�ƃT�[�t�F�X�ɑ΂���\�ʂ̃L���r�e�B���擾����
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.get(), &swapChainDetails.surfaceCapabilities);
//
//	// -- FORMATS --
//	uint32_t formatCount = 0;
//	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.get(), &formatCount, nullptr);
//
//	// �t�H�[�}�b�g���Ԃ��ꂽ�ꍇ�A�t�H�[�}�b�g�̃��X�g���擾����
//	if (formatCount != 0)
//	{
//		swapChainDetails.formats.resize(formatCount);
//		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.get(), &formatCount, swapChainDetails.formats.data());
//	}
//
//	// -- PRESENTATION MODES --
//	uint32_t presentationCount = 0;
//	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.get(), &presentationCount, nullptr);
//
//	// �v���[���e�[�V�������[�h���Ԃ��ꂽ�ꍇ�A�v���[���e�[�V�������[�h�̃��X�g���擾����
//	if (presentationCount != 0)
//	{
//		swapChainDetails.presentationModes.resize(presentationCount);
//		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.get(), &presentationCount, swapChainDetails.presentationModes.data());
//	}
//
//	return swapChainDetails;
//}
//
//
//// �x�X�g�ȃt�H�[�}�b�g�̑I���͎�ϓI�ł����A�ʏ�ȉ��̂悤�ɂ��܂�:
//// format     :   VK_FORMAT_R8G8B8A8_UNORM (�o�b�N�A�b�v�Ƃ��� VK_FORMAT_B8G8R8A8_UNORM)
//// colorSpace :   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
//VkSurfaceFormatKHR VulkanRenderer::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
//{
//	// �t�H�[�}�b�g��1��������A����`�̏ꍇ�A���ׂẴt�H�[�}�b�g�����p�\�Ƃ݂Ȃ��i�����Ȃ��j
//	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
//	{
//		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
//	}
//
//	// ��������Ă���ꍇ�A�œK�ȃt�H�[�}�b�g��T��
//	for (const auto& format : formats)
//	{
//		if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM)
//			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
//		{
//			return format;
//		}
//	}
//
//	// �œK�ȃt�H�[�}�b�g��������Ȃ��ꍇ�A�ŏ��̃t�H�[�}�b�g��Ԃ�
//	return formats[0];
//}


VkPresentModeKHR VulkanRenderer::chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes)
{
	// ���[���{�b�N�X�iMailbox�j�v���[���e�[�V�������[�h��T��
	// ���[���{�b�N�X���[�h�́A�o�b�t�@�̃X���b�v�������s���ۂɁA�ŐV�̃t���[���݂̂�ێ����A�Â��t���[����j�����邽�߁A
	// �����_�����O�̒x�����ŏ����ɗ}���邱�Ƃ��ł��܂��B
	// �������[���{�b�N�X���[�h�����p�\�ł���΁A�����I�����܂��B
	for (const auto& presentationMode : presentationModes)
	{
		if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentationMode;
		}
	}

	// ���[���{�b�N�X���[�h��������Ȃ��ꍇ�AVulkan �d�l�ɂ�� FIFO ���[�h���g�p���邱�Ƃ��ۏ؂���Ă��܂��B
	// FIFO ���[�h�́A�L���[�ɗ��܂����t���[�������Ԃɏ������邽�߁A�����_�����O�̈��萫���ۂ���܂����A
	// ���C�e���V�͔�r�I�����Ȃ�ꍇ������܂��B
	return VK_PRESENT_MODE_FIFO_KHR;
}

#pragma region VkPresentModeKHR���
// VkPresentModeKHR	: ���ꂼ��̃��[�h�ɂ��Ẳ��(ChatGPT�ō쐬)
// 
//  VK_PRESENT_MODE_IMMEDIATE_KHR		�������[�h: ���������Ȃ��ŉ�ʍX�V�������ɍs����B                
// �����ɔ������̍����`������߂�ꍇ�ɓK���Ă��܂��B
// �Ⴆ�΁A���A���^�C���̑���ɑ΂��Ēx�����ŏ����ɗ}�������ꍇ��A�������x���d�v�ȃA�v���P�[�V�����ɓK���Ă��܂��B
// 
//  VK_PRESENT_MODE_MAILBOX_KHR			���[���{�b�N�X���[�h: �t���[���o�b�t�@�̌������V�����t���[���ɐ؂�ւ�邽�тɍs����B
// �t���[�����[�g�����肵�Ă���A�ŐV�̃t���[���������ɕ`�悷��K�v������ꍇ�ɓK���Ă��܂��B
// �Ⴆ�΁A�Q�[���Ȃǂŕ`��x�����ŏ����ɗ}�������ꍇ�ɗ��p����܂��B
// 
//  VK_PRESENT_MODE_FIFO_KHR			FIFO���[�h: �t���[���o�b�t�@�̌�����V-Sync�Ɠ�������A����V-Blank�܂őҋ@����B
// �ł���ʓI�ȃ��[�h�ŁA�t���[���o�b�t�@�̌�����V-Sync�ɓ�������A��ʂ̃e�B�A�����O�i��ʂ̐؂�ځj��h�����߂Ɏg�p����܂��B
// ���肵���`�悪���߂��鑽���̃A�v���P�[�V�����ŗ��p����܂��B
// 
//  VK_PRESENT_MODE_FIFO_RELAXED_KHR	FIFO�����b�N�X���[�h: V-Sync���L���ȏꍇ�͒ʏ��FIFO���[�h�Ɠ����ŁAV-Sync�������ȏꍇ�͑����Ƀo�b�t�@�����������B
// V-Sync���I�t�̏ꍇ��A�σt���[�����[�g�iVRR�j���T�|�[�g����f�B�X�v���C�ł̎g�p����������܂��B
// ���I�ȃt���[�����[�g�̕ω��ɑΉ�����K�v������ꍇ�ɖ𗧂��܂��B
// 
//	VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR ���v���t���b�V�����L���[�h: ����̃��t���b�V���v���Ɋ�Â��ĉ�ʍX�V���s����B
// ���v�ɉ����ĉ�ʍX�V���s�����ƂŁA�ȓd�̓��[�h�ł̎g�p���z�肳��Ă��܂��B
// �o�b�e���[�쓮�̃f�o�C�X��ȓd�͂��d�����郂�o�C���f�o�C�X�ŗL���ł��B
// 
//  VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR �A�����t���b�V�����L���[�h: �A���I�ȃ��t���b�V���v���Ɋ�Â��ĉ�ʍX�V���s����B
// VR�i���z�����j��AR�i�g�������j�A�v���P�[�V�����ȂǁA�A���I�ȍ����ȃ��t���b�V�����K�v�ȏ�ʂŗ��p����܂��B
// ��x���ł̃��A���^�C���ȕ`�悪���߂�����ɓK���Ă��܂��B
#pragma endregion





VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	// ���݂� extent �����l�̌��E�ɂ���ꍇ�Aextent �͕ω�����\��������܂��B����ȊO�̏ꍇ�̓E�B���h�E�̃T�C�Y�ɂȂ�܂��B
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		// ���l�̌��E�łȂ��ꍇ�A���݂� extent ��Ԃ��܂��B
		return surfaceCapabilities.currentExtent;
	}
	else
	{
		// extent ���ω�����ꍇ�͎蓮�Őݒ肷��K�v������܂��B

		// �E�B���h�E�̃T�C�Y���擾���܂�
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// �E�B���h�E�T�C�Y���g�p���ĐV���� extent ���쐬���܂�
		VkExtent2D newExtent = {};
		newExtent.width = static_cast<uint32_t>(width);
		newExtent.height = static_cast<uint32_t>(height);

		// Surface �͍ő储��эŏ��� extent ���`���Ă��܂��̂ŁA�l�����E���ɂ��邱�Ƃ��m�F���邽�߂ɃN�����v���܂�
		newExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, newExtent.width));
		newExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, newExtent.height));

		return newExtent;
	}
}


VkImageView VulkanRenderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	// �摜�r���[�쐬���̏�����
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;		 // �摜�r���[�쐬���̍\���̃^�C�v��ݒ�
	viewCreateInfo.image = image;                                            // View ���쐬���邽�߂� Image
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;                         // Image �̎�� (1D, 2D, 3D, Cube �Ȃ�)
	viewCreateInfo.format = format;                                          // Image �f�[�^�̃t�H�[�}�b�g
	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;             // RGBA �R���|�[�l���g�𑼂� RGBA �l�Ƀ��}�b�v���邱�Ƃ��ł��܂�
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	// Subresource �� Image �̈ꕔ������\�����邽�߂̐ݒ�ł�
	viewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image �̂ǂ̖ʂ�\�����邩 (��: COLOR_BIT �͐F��\�����邽��)
	viewCreateInfo.subresourceRange.baseMipLevel = 0;                         // �\�����J�n���� Mipmap ���x��
	viewCreateInfo.subresourceRange.levelCount = 1;                           // �\������ Mipmap ���x���̐�
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // �\�����J�n����z�񃌃x��
	viewCreateInfo.subresourceRange.layerCount = 1;                           // �\������z�񃌃x���̐�

	// Image View ���쐬���A���̃n���h����Ԃ��܂�
	VkImageView imageView;  // Image View �̃n���h����ێ�����ϐ���錾���܂�

	// vkCreateImageView �֐����g�p���� Image View ���쐬���܂�
	// mainDevice.logicalDevice: Image View ���쐬���邽�߂̘_���f�o�C�X
	// &viewCreateInfo: Image View �̍쐬�ɕK�v�ȏ�񂪊i�[���ꂽ�\���̂ւ̃|�C���^
	// nullptr: �J�X�^���̃A���P�[�^�[���g�p���Ȃ����߂̃I�v�V���� (�ʏ�� nullptr ���w�肵�܂�)
	// &imageView: �쐬���ꂽ Image View �̃n���h�����󂯎��ϐ��ւ̃|�C���^
	VkResult result = vkCreateImageView(logicalDevice.get(), &viewCreateInfo, nullptr, &imageView);

	// vkCreateImageView �̌��ʂ������ł͂Ȃ��ꍇ�A�G���[���X���[���܂�
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create an Image View!");
	}

	// �쐬���� Image View �̃n���h����Ԃ��܂�
	return imageView;
}

//void VulkanRenderer::createRenderPass()
//{
//	// Colour attachment of render pass
//	VkAttachmentDescription colourAttachment = {};
//	colourAttachment.format = swapChainImageFormat;						// Format to use for attachment
//	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;					// Number of samples to write for multisampling
//	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;				// Describes what to do with attachment before rendering
//	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;			// Describes what to do with attachment after rendering
//	colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	// Describes what to do with stencil before rendering
//	colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;	// Describes what to do with stencil after rendering
//
//	// Framebuffer data will be stored as an image, but images can be given different data layouts
//	// to give optimal use for certain operations
//	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;			// Image data layout before render pass starts
//	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;		// Image data layout after render pass (to change to)
//
//	// Attachment reference uses an attachment index that refers to index in the attachment list passed to renderPassCreateInfo
//	VkAttachmentReference colourAttachmentReference = {};
//	colourAttachmentReference.attachment = 0;
//	colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//	// Information about a particular subpass the Render Pass is using
//	VkSubpassDescription subpass = {};
//	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;		// Pipeline type subpass is to be bound to
//	subpass.colorAttachmentCount = 1;
//	subpass.pColorAttachments = &colourAttachmentReference;
//
//	// Need to determine when layout transitions occur using subpass dependencies
//	std::array<VkSubpassDependency, 2> subpassDependencies;
//
//	// Conversion from VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
//	// Transition must happen after...
//	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;						// Subpass index (VK_SUBPASS_EXTERNAL = Special value meaning outside of renderpass)
//	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;		// Pipeline stage
//	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;				// Stage access mask (memory access)
//	// But must happen before...
//	subpassDependencies[0].dstSubpass = 0;
//	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//	subpassDependencies[0].dependencyFlags = 0;
//
//
//	// Conversion from VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
//	// Transition must happen after...
//	subpassDependencies[1].srcSubpass = 0;
//	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;;
//	// But must happen before...
//	subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
//	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//	subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
//	subpassDependencies[1].dependencyFlags = 0;
//
//	// Create info for Render Pass
//	VkRenderPassCreateInfo renderPassCreateInfo = {};
//	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//	renderPassCreateInfo.attachmentCount = 1;
//	renderPassCreateInfo.pAttachments = &colourAttachment;
//	renderPassCreateInfo.subpassCount = 1;
//	renderPassCreateInfo.pSubpasses = &subpass;
//	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());
//	renderPassCreateInfo.pDependencies = subpassDependencies.data();
//
//	VkResult result = vkCreateRenderPass(logicalDevice.get(), &renderPassCreateInfo, nullptr, &renderPass);
//	if (result != VK_SUCCESS)
//	{
//		throw std::runtime_error("Failed to create a Render Pass!");
//	}
//}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
	// �V�F�[�_�[���W���[���쐬���̐ݒ�
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;		// �\���̂̃^�C�v��ݒ�
	shaderModuleCreateInfo.codeSize = code.size();									// �R�[�h�̃T�C�Y���w��
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());	// �R�[�h�̃|�C���^��uint32_t�|�C���^�ɃL���X�g���Đݒ�

	// �V�F�[�_�[���W���[���̍쐬
	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(logicalDevice.get(), &shaderModuleCreateInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�V�F�[�_�[���W���[���̍쐬�Ɏ��s���܂����I");
	}

	return shaderModule;
}
