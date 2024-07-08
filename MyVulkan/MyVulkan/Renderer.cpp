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
		createSurface();
		getPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createRenderPass();
		createGraphicsPipeline();
	}
	catch (const std::runtime_error& e) {
		//�G���[���b�Z�[�W�󂯎��
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}

	return 0;
}

void VulkanRenderer::cleanup()
{
	vkDestroyPipeline(mainDevice.logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(mainDevice.logicalDevice, pipelineLayout, nullptr);
	vkDestroyRenderPass(mainDevice.logicalDevice, renderPass, nullptr);
	for (auto image : swapChainImages)
	{
		vkDestroyImageView(mainDevice.logicalDevice, image.imageView, nullptr);
	}
	vkDestroySwapchainKHR(mainDevice.logicalDevice, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	if (validationEnabled)
	{
		DestroyDebugReportCallbackEXT(instance, callback, nullptr);
	}
	vkDestroyInstance(instance, nullptr);
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
	VkInstanceCreateInfo instCreateInfo = {};
	instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	// �C���X�^���X�̃^�C�v
	instCreateInfo.pNext;											// �g���@�\�̏�� 
	instCreateInfo.flags;											// �C���X�^���X�̍쐬�t���O 
	instCreateInfo.pApplicationInfo = &appInfo;						// �A�v���P�[�V�������ւ̃|�C���^
	instCreateInfo.enabledLayerCount;								// �L���ɂ��郌�C���[�̐� 
	instCreateInfo.ppEnabledLayerNames;								// �L���ɂ��郌�C���[�̖��O�̔z�� 
	instCreateInfo.enabledExtensionCount;							// �L���ɂ���g���@�\�̐� 
	instCreateInfo.ppEnabledExtensionNames;							// �L���ɂ���g���@�\�̖��O�̔z�� 


	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	// �C���X�^���X���g�p����g���@�\��ݒ肷��
	uint32_t glfwExtensionCount = 0;                    // GLFW�͕����̊g���@�\��v������ꍇ������
	const char** glfwExtensions;                        // �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^�i�z��j�̃|�C���^�iC������j���K�v

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
		//throw std::runtime_error("VkInstance does not support required extensions!");
		throw std::runtime_error("VkInstance �͕K�v�Ȋg���@�\���T�|�[�g���Ă��܂���I");
	}

	// �L���Ȋg���@�\�̐��Ɩ��O�̔z���ݒ肷��
	instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// �o���f�[�V�������L���ȏꍇ�A�L���ȃ��C���[�̐��Ɩ��O�̔z���ݒ肷��
	if (validationEnabled)
	{
		//�L���ȏꍇ
		instCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		//�����ȏꍇ
		instCreateInfo.enabledLayerCount = 0;
		instCreateInfo.ppEnabledLayerNames = nullptr;
	}

	// �C���X�^���X���쐬����
	VkResult result = vkCreateInstance(&instCreateInfo, nullptr, &instance);

	//�C���X�^���X�̍쐬�Ɏ��s�����ꍇ�̃G���[���b�Z�[�W
	if (result != VK_SUCCESS)
	{
		//throw std::runtime_error("Failed to create a Vulkan Instance!");
		throw std::runtime_error("Vulkan �C���X�^���X�̍쐬�Ɏ��s���܂����I");
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
	VkResult result = CreateDebugReportCallbackEXT(instance, &callbackCreateInfo, nullptr, &callback);
	if (result != VK_SUCCESS)
	{
		//throw std::runtime_error("Failed to create Debug Callback!");
		throw std::runtime_error("�f�o�b�O�R�[���o�b�N�̍쐬�Ɏ��s���܂����I");
	}
}

void VulkanRenderer::createLogicalDevice()
{
	// �I�����������f�o�C�X�̃L���[�t�@�~���[�C���f�b�N�X���擾����
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	// �L���[�쐬���p�̃x�N�^�[�ƃt�@�~���[�C���f�b�N�X�p�̃Z�b�g����������
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> queueFamilyIndices = { indices.graphicsFamily, indices.presentationFamily };

	// ���W�J���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
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

	// ���W�J���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // �L���[�쐬���̐�
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g

	// ���W�J���f�o�C�X���g�p���镨���f�o�C�X�̋@�\
	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;            // ���W�J���f�o�C�X���g�p���镨���f�o�C�X�̋@�\

	// �w�肳�ꂽ�����f�o�C�X�ɑ΂��ă��W�J���f�o�C�X���쐬����
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("���W�J���f�o�C�X�̍쐬�Ɏ��s���܂����I");
	}

	// �L���[�̓f�o�C�X�Ɠ����ɍ쐬����邽��
	// �L���[�ւ̃n���h�����擾����
	// �w�肳�ꂽ���W�J���f�o�C�X����A�w�肳�ꂽ�L���[�t�@�~���[�̎w�肳�ꂽ�L���[�C���f�b�N�X�i0�͗B��̃L���[�Ȃ̂�0�j�A�w�肳�ꂽVkQueue�ւ̎Q�Ƃ�u��
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.presentationFamily, 0, &presentationQueue);
}

void VulkanRenderer::createSurface()
{
	// �T�[�t�F�X���쐬����i�T�[�t�F�X�쐬���\���̂��쐬���A�T�[�t�F�X�쐬�֐������s���A���ʂ�Ԃ��j
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�T�[�t�F�X�̍쐬�Ɏ��s���܂����I");
	}
}


void VulkanRenderer::createSwapChain()
{
	// Swap Chain�̏ڍׂ��擾���čœK�Ȑݒ��I������
	SwapChainDetails swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);

	// Swap Chain�̍œK��Surface�t�H�[�}�b�g��I������
	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapChainDetails.formats);

	// Swap Chain�̍œK�ȃv���[���e�[�V�������[�h��I������
	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentationModes);

	// Swap Chain�̍œK��Extent�i�T�C�Y�j��I������
	VkExtent2D extent = chooseSwapExtent(swapChainDetails.surfaceCapabilities);

	// Swap Chain�Ɋ܂܂��摜�̐������肷��
	uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;

	// ����imageCount��max�𒴂��Ă�����max�ɐ�������
	if (swapChainDetails.surfaceCapabilities.maxImageCount > 0
		&& swapChainDetails.surfaceCapabilities.maxImageCount < imageCount)
	{
		imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;
	}


	/*//////////////////////////
	* �X���b�v�`�F�C���̍쐬
	*///////////////////////////
	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;					// �X���b�v�`�F�C���̃^�C�v
	swapChainCreateInfo.surface = surface;														// �X���b�v�`�F�C���̑ΏۂƂȂ�Surface
	swapChainCreateInfo.imageFormat = surfaceFormat.format;										// �X���b�v�`�F�C���̉摜�t�H�[�}�b�g
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;								// �X���b�v�`�F�C���̃J���[�X�y�[�X
	swapChainCreateInfo.presentMode = presentMode;												// �X���b�v�`�F�C���̃v���[���e�[�V�������[�h
	swapChainCreateInfo.imageExtent = extent;													// �X���b�v�`�F�C���̉摜�̃T�C�Y
	swapChainCreateInfo.minImageCount = imageCount;												// �X���b�v�`�F�C�����̍ŏ��摜��
	swapChainCreateInfo.imageArrayLayers = 1;													// �摜�̔z�񃌃C���[��
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;						// �摜���ǂ̂悤�Ɏg�p����邩
	swapChainCreateInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;	// Swapchain�摜�ɑ΂��čs���ϊ�
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;						// �O���O���t�B�b�N�X�Ƃ̃u�����f�B���O�̈�����
	swapChainCreateInfo.clipped = VK_TRUE;														// ��ʊO�̕������N���b�v���邩�ǂ���

	// �L���[�t�@�~���[�C���f�b�N�X���擾����
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	// �O���t�B�N�X�t�@�~���[�ƃv���[���e�[�V�����t�@�~���[���قȂ�ꍇ�A�摜���t�@�~���[�Ԃŋ��L����K�v������
	if (indices.graphicsFamily != indices.presentationFamily)
	{
		// �قȂ�ꍇ
		// ���L����L���[�̎w��
		uint32_t queueFamilyIndices[] = {
			static_cast<uint32_t>(indices.graphicsFamily),
			static_cast<uint32_t>(indices.presentationFamily)
		};

		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;		// �摜���L�̃��[�h
		swapChainCreateInfo.queueFamilyIndexCount = 2;							// �摜�����L����L���[�̐�
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;			// �摜�����L����L���[�̔z��
	}
	else
	{
		// �����������ꍇ
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;		// �摜���L�̃��[�h
		swapChainCreateInfo.queueFamilyIndexCount = 0;							// �摜�����L����L���[�̐�
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;						// �摜�����L����L���[�̔z��
	}

	// �Â�Swapchain���j������A�V����Swapchain���u����������ꍇ�A��Swapchain�������N���ĐӔC��v���Ɉ����p��
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	// Swapchain���쐬����
	VkResult result = vkCreateSwapchainKHR(mainDevice.logicalDevice, &swapChainCreateInfo, nullptr, &swapchain);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Swapchain�̍쐬�Ɏ��s���܂����I");
	}

	// ��ŎQ�Ƃ��邽�߂ɕۑ�����
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	// Swapchain�̉摜���擾����i�����擾���A�l���擾����j
	uint32_t swapChainImageCount;	//�X���b�v�`�F�C���̉摜��
	vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapChainImageCount, nullptr);
	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapChainImageCount, images.data());

	for (VkImage image : images)
	{
		// �摜�n���h����ۑ�����
		SwapchainImage swapChainImage = {};
		swapChainImage.image = image;
		swapChainImage.imageView = createImageView(image, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);

		// Swapchain�摜���X�g�ɒǉ�����
		swapChainImages.push_back(swapChainImage);
	}
}

void VulkanRenderer::createGraphicsPipeline()
{
	// �V�F�[�_�[��SPIR-V�R�[�h��ǂݍ���
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// �V�F�[�_�[���W���[���̍쐬
	VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
	VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

	// -- �V�F�[�_�[�X�e�[�W�̍쐬��� --
	// ���_�V�F�[�_�[�X�e�[�W�̍쐬���
	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;	// ���_�V�F�[�_�[�X�e�[�W�쐬���̍\���̃^�C�v��ݒ�
	vertexShaderCreateInfo.pNext = nullptr;												// ���̍\���̂��g������\���̂ւ̃|�C���^
	vertexShaderCreateInfo.flags;														// �V�F�[�_ �X�e�[�W�̐������@���w�肷��VkPipelineShaderStageCreateFlagBits�̃r�b�g�}�X�N(���ׂ����ǂ悭������Ȃ�����)
	vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;							// �V�F�[�_�[�X�e�[�W�̎��
	vertexShaderCreateInfo.module = vertexShaderModule;									// �g�p����V�F�[�_�[���W���[��
	vertexShaderCreateInfo.pName = "main";												// �V�F�[�_�[�̃G���g���[�|�C���g��

	// �t���O�����g�V�F�[�_�[�X�e�[�W�̍쐬���
	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;	// �t���O�����g�V�F�[�_�[�X�e�[�W�쐬���̍\���̃^�C�v��ݒ�
	fragmentShaderCreateInfo.pNext = nullptr;												// ���̍\���̂��g������\���̂ւ̃|�C���^
	fragmentShaderCreateInfo.flags;															// �V�F�[�_ �X�e�[�W�̐������@���w�肷��VkPipelineShaderStageCreateFlagBits�̃r�b�g�}�X�N
	fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;							// �V�F�[�_�[�X�e�[�W�̎��
	fragmentShaderCreateInfo.module = fragmentShaderModule;									// �g�p����V�F�[�_�[���W���[��
	fragmentShaderCreateInfo.pName = "main";												// �V�F�[�_�[�̃G���g���[�|�C���g��


	// �V�F�[�_�[�X�e�[�W�쐬����z��Ɋi�[
	// �O���t�B�b�N�X�p�C�v���C���̍쐬���ɂ̓V�F�[�_�[�X�e�[�W�쐬���̔z�񂪕K�v
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	// �p�C�v���C���̍쐬

	// �p�C�v���C�����쐬���ꂽ��ɂ̓V�F�[�_�[���W���[���͕s�v�ɂȂ邽�ߔj������
	vkDestroyShaderModule(mainDevice.logicalDevice, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(mainDevice.logicalDevice, vertexShaderModule, nullptr);
}



void VulkanRenderer::getPhysicalDevice()
{
	// vkInstance���A�N�Z�X�ł��镨���f�o�C�X��񋓂���
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	if (deviceCount == 0)
	{
		throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");
	}

	// �����f�o�C�X�̃��X�g���擾����
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : deviceList)
	{
		if (checkDeviceSuitable(device))
		{
			mainDevice.physicalDevice = device;
			break;
		}
	}
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


bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	/*
	// �f�o�C�X���̂Ɋւ����� (ID�A���O�A�^�C�v�A�x���_�[�Ȃ�)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// �f�o�C�X�̋@�\�Ɋւ����� (�W�I���g���V�F�[�_�[�A�e�b�Z���[�V�����V�F�[�_�[�A���C�h���C���Ȃ�)
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	*/

	// �L���[�t�@�~���[�̃C���f�b�N�X���擾����
	QueueFamilyIndices indices = getQueueFamilies(device);

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g���Ă��邩�m�F����
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	// Swapchain���L�����ǂ������m�F����
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// ����̕����f�o�C�X�ɑ΂���Swap Chain�̏ڍׂ��擾����
		SwapChainDetails swapChainDetails = getSwapChainDetails(device);

		// Swap Chain�̗L�������m�F����B�v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// �f�o�C�X���K�����Ă��邩���m�F����
	return indices.isValid() && extensionsSupported && swapChainValid;
}


QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// �����f�o�C�X�ɑ΂��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	// �e�L���[�t�@�~���[�𒲂ׁA�K�v�ȃ^�C�v�̃L���[�����Ȃ��Ƃ�1�����Ă��邩�ǂ������m�F����
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// �܂��A�L���[�t�@�~���[�����Ȃ��Ƃ�1�̃L���[�������Ă��邩�m�F����i�L���[���Ȃ��\��������j
		// �L���[�̓r�b�g�t�B�[���h�ŕ����̃^�C�v���`���邱�Ƃ��ł���BVK_QUEUE_*_BIT�ƃr�b�g���Ƃ�AND���Z���s���A�K�v�ȃ^�C�v�������Ă��邩�m�F����
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
		}

		// �L���[�t�@�~���[���v���[���e�[�V�������T�|�[�g���Ă��邩�m�F����
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
		// �L���[���v���[���e�[�V�����^�C�v�ł��邩�ǂ������m�F����i�O���t�B�b�N�X�ƃv���[���e�[�V�����̗����ɂȂ蓾��j
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		// �L���[�t�@�~���[�̃C���f�b�N�X���L���ȏ�Ԃɂ��邩�ǂ������m�F���A�����ł���Ό������~����
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}


SwapChainDetails VulkanRenderer::getSwapChainDetails(VkPhysicalDevice device)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// ����̕����f�o�C�X�ƃT�[�t�F�X�ɑ΂���\�ʂ̃L���r�e�B���擾����
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails.surfaceCapabilities);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	// �t�H�[�}�b�g���Ԃ��ꂽ�ꍇ�A�t�H�[�}�b�g�̃��X�g���擾����
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);

	// �v���[���e�[�V�������[�h���Ԃ��ꂽ�ꍇ�A�v���[���e�[�V�������[�h�̃��X�g���擾����
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}

	return swapChainDetails;
}


// �x�X�g�ȃt�H�[�}�b�g�̑I���͎�ϓI�ł����A�ʏ�ȉ��̂悤�ɂ��܂�:
// format     :   VK_FORMAT_R8G8B8A8_UNORM (�o�b�N�A�b�v�Ƃ��� VK_FORMAT_B8G8R8A8_UNORM)
// colorSpace :   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
VkSurfaceFormatKHR VulkanRenderer::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	// �t�H�[�}�b�g��1��������A����`�̏ꍇ�A���ׂẴt�H�[�}�b�g�����p�\�Ƃ݂Ȃ��i�����Ȃ��j
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	// ��������Ă���ꍇ�A�œK�ȃt�H�[�}�b�g��T��
	for (const auto& format : formats)
	{
		if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM)
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}

	// �œK�ȃt�H�[�}�b�g��������Ȃ��ꍇ�A�ŏ��̃t�H�[�}�b�g��Ԃ�
	return formats[0];
}


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
	VkResult result = vkCreateImageView(mainDevice.logicalDevice, &viewCreateInfo, nullptr, &imageView);

	// vkCreateImageView �̌��ʂ������ł͂Ȃ��ꍇ�A�G���[���X���[���܂�
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create an Image View!");
	}

	// �쐬���� Image View �̃n���h����Ԃ��܂�
	return imageView;
}

void VulkanRenderer::createRenderPass()
{
	// �����_�[�p�X�̃J���[�A�^�b�`�����g
	VkAttachmentDescription colourAttachment = {};						// �J���[�A�^�b�`�����g�̐ݒ���i�[����\����
	colourAttachment.format = swapChainImageFormat;						// �A�^�b�`�����g�Ɏg�p����t�H�[�}�b�g��ݒ�
	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;					// �}���`�T���v�����O�p�̃T���v������ݒ�
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;				// �����_�����O�O�ɃA�^�b�`�����g���N���A����ݒ�
	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;			// �����_�����O��ɃA�^�b�`�����g��ۑ�����ݒ�
	colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	// �X�e���V���o�b�t�@���g�p���Ȃ����ߖ���
	colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // �X�e���V���o�b�t�@���g�p���Ȃ����ߖ���

	// �t���[���o�b�t�@�f�[�^�̓C���[�W�Ƃ��ĕۑ�����邪�A����̑���ɍœK�Ȃ悤�ɈقȂ�f�[�^���C�A�E�g��^���邱�Ƃ��ł���
	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;		// �����_�[�p�X�J�n�O�̃C���[�W�f�[�^���C�A�E�g�𖢒�`�ɐݒ�
	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // �����_�[�p�X��̃C���[�W�f�[�^���C�A�E�g���v���[���g�p�ɐݒ�

	// �A�^�b�`�����g�Q�Ƃ́ArenderPassCreateInfo�ɓn�����A�^�b�`�����g���X�g���̃C���f�b�N�X���Q�Ƃ���A�^�b�`�����g�C���f�b�N�X���g�p����
	VkAttachmentReference colourAttachmentReference = {};							// �J���[�A�^�b�`�����g�̎Q�Ƃ��i�[����\����
	colourAttachmentReference.attachment = 0;										// �A�^�b�`�����g���X�g�̍ŏ��̃A�^�b�`�����g���w��
	colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;	// �œK�ȃJ���[�A�^�b�`�����g���C�A�E�g��ݒ�

	// �����_�[�p�X���g�p�������̃T�u�p�X�Ɋւ�����
	VkSubpassDescription subpass = {};								// �T�u�p�X�̐ݒ���i�[����\���̂�������
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;	// �O���t�B�b�N�X�p�C�v���C���Ƀo�C���h����T�u�p�X��ݒ�
	subpass.colorAttachmentCount = 1;								// �J���[�A�^�b�`�����g�̐���ݒ�
	subpass.pColorAttachments = &colourAttachmentReference;			// �J���[�A�^�b�`�����g�̎Q�Ƃ�ݒ�

	// �T�u�p�X�ˑ��֌W���g�p���ă��C�A�E�g�J�ڂ���������^�C�~���O�����肷��K�v������
	std::array<VkSubpassDependency, 2> subpassDependencies;			// �T�u�p�X�̈ˑ��֌W���i�[����z���������

	// VK_IMAGE_LAYOUT_UNDEFINED����VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL�ւ̕ϊ�
	// �J�ڂ͈ȉ��̌�ɔ������Ȃ���΂Ȃ�Ȃ�
	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;					// �O���T�u�p�X����̈ˑ��֌W��ݒ�
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // �p�C�v���C���̍ŏI�i�K����̈ˑ��֌W��ݒ�
	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;			// �������ǂݎ��A�N�Z�X�̈ˑ��֌W��ݒ�
	// �������ȉ��̑O�ɔ������Ȃ���΂Ȃ�Ȃ�
	subpassDependencies[0].dstSubpass = 0;  // �ŏ��̃T�u�p�X�ւ̈ˑ��֌W��ݒ�
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	// �J���[�A�^�b�`�����g�o�̓X�e�[�W�ւ̈ˑ��֌W��ݒ�
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |			// �J���[�A�^�b�`�����g�̓ǂݏ����A�N�Z�X�̈ˑ��֌W��ݒ�
											VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;  
	subpassDependencies[0].dependencyFlags = 0;												// �ǉ��̈ˑ��֌W�t���O��ݒ�

	// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL����VK_IMAGE_LAYOUT_PRESENT_SRC_KHR�ւ̕ϊ�
	// �J�ڂ͈ȉ��̌�ɔ������Ȃ���΂Ȃ�Ȃ�
	subpassDependencies[1].srcSubpass = 0;  // �ŏ��̃T�u�p�X����̈ˑ��֌W��ݒ�
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	// �J���[�A�^�b�`�����g�o�̓X�e�[�W����̈ˑ��֌W��ݒ�
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |			// �J���[�A�^�b�`�����g�̓ǂݏ����A�N�Z�X�̈ˑ��֌W��ݒ�
											VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;  
	// �������ȉ��̑O�ɔ������Ȃ���΂Ȃ�Ȃ�
	subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;								// �O���T�u�p�X�ւ̈ˑ��֌W��ݒ�
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;				// �p�C�v���C���̍ŏI�i�K�ւ̈ˑ��֌W��ݒ�
	subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;						// �������ǂݎ��A�N�Z�X�̈ˑ��֌W��ݒ�
	subpassDependencies[1].dependencyFlags = 0;												// �ǉ��̈ˑ��֌W�t���O��ݒ�

	// �����_�[�p�X�̍쐬���
	VkRenderPassCreateInfo renderPassCreateInfo = {};										// �����_�[�p�X�̍쐬�����i�[����\���̂�������
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;					// �\���̂̌^��ݒ�
	renderPassCreateInfo.attachmentCount = 1;												// �A�^�b�`�����g�̐���ݒ�
	renderPassCreateInfo.pAttachments = &colourAttachment;									// �J���[�A�^�b�`�����g�̎Q�Ƃ�ݒ�
	renderPassCreateInfo.subpassCount = 1;													// �T�u�p�X�̐���ݒ�
	renderPassCreateInfo.pSubpasses = &subpass;												// �T�u�p�X�̎Q�Ƃ�ݒ�
	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());  // �T�u�p�X�ˑ��֌W�̐���ݒ�
	renderPassCreateInfo.pDependencies = subpassDependencies.data();						// �T�u�p�X�ˑ��֌W�̎Q�Ƃ�ݒ�

	// �����_�[�p�X���쐬
	VkResult result = vkCreateRenderPass(mainDevice.logicalDevice, &renderPassCreateInfo, nullptr, &renderPass); 
	if (result != VK_SUCCESS)  
	{
		// �쐬�Ɏ��s�����ꍇ
		throw std::runtime_error("Render Pass�̍쐬�Ɏ��s���܂����I");  // �G���[���b�Z�[�W���X���[
	}
}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
	// �V�F�[�_�[���W���[���쐬���̐ݒ�
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;		// �\���̂̃^�C�v��ݒ�
	shaderModuleCreateInfo.codeSize = code.size();									// �R�[�h�̃T�C�Y���w��
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());	// �R�[�h�̃|�C���^��uint32_t�|�C���^�ɃL���X�g���Đݒ�

	// �V�F�[�_�[���W���[���̍쐬
	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(mainDevice.logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("�V�F�[�_�[���W���[���̍쐬�Ɏ��s���܂����I");
	}

	return shaderModule;
}
