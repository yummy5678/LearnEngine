#include "Renderer.h"




VulkanRenderer::VulkanRenderer()
{
}

int VulkanRenderer::init(GameWindow renderWindow)
{
	window = renderWindow.getWindowPointer(); //ウィンドウのポインタのセット

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
		//エラーメッセージ受け取り
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

	// エラーチェック
	if (validationEnabled && !checkValidationLayerSupport())
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}

	// アプリケーションの情報を初期化
	// ここは好きな値を書き込む
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";                     // アプリケーションの名前
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);       // アプリケーションのバージョン名 
	appInfo.pEngineName = "No Engine";                           // エンジンの名前
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);            // エンジンのバージョン名
	appInfo.apiVersion = VK_API_VERSION_1_0;                     // Vulkan APIのバージョン

	/*/////////////////////
	* インスタンスの作成
	*//////////////////////
	VkInstanceCreateInfo instCreateInfo = {};
	instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	// インスタンスのタイプ
	instCreateInfo.pNext;											// 拡張機能の情報 
	instCreateInfo.flags;											// インスタンスの作成フラグ 
	instCreateInfo.pApplicationInfo = &appInfo;						// アプリケーション情報へのポインタ
	instCreateInfo.enabledLayerCount;								// 有効にするレイヤーの数 
	instCreateInfo.ppEnabledLayerNames;								// 有効にするレイヤーの名前の配列 
	instCreateInfo.enabledExtensionCount;							// 有効にする拡張機能の数 
	instCreateInfo.ppEnabledExtensionNames;							// 有効にする拡張機能の名前の配列 


	// インスタンス拡張機能のリストを作成する
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	// インスタンスが使用する拡張機能を設定する
	uint32_t glfwExtensionCount = 0;                    // GLFWは複数の拡張機能を要求する場合がある
	const char** glfwExtensions;                        // 拡張機能はC文字列の配列として渡されるため、ポインタ（配列）のポインタ（C文字列）が必要

	// GLFWの拡張機能を取得する
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFWの拡張機能をリストに追加する
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	// バリデーションが有効な場合、検証用のデバッグ情報拡張機能を追加する
	if (validationEnabled)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// インスタンスが必要とする拡張機能がサポートされているか確認する
	if (!checkInstanceExtensionSupport(&instanceExtensions))
	{
		//throw std::runtime_error("VkInstance does not support required extensions!");
		throw std::runtime_error("VkInstance は必要な拡張機能をサポートしていません！");
	}

	// 有効な拡張機能の数と名前の配列を設定する
	instCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// バリデーションが有効な場合、有効なレイヤーの数と名前の配列を設定する
	if (validationEnabled)
	{
		//有効な場合
		instCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		instCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		//無効な場合
		instCreateInfo.enabledLayerCount = 0;
		instCreateInfo.ppEnabledLayerNames = nullptr;
	}

	// インスタンスを作成する
	VkResult result = vkCreateInstance(&instCreateInfo, nullptr, &instance);

	//インスタンスの作成に失敗した場合のエラーメッセージ
	if (result != VK_SUCCESS)
	{
		//throw std::runtime_error("Failed to create a Vulkan Instance!");
		throw std::runtime_error("Vulkan インスタンスの作成に失敗しました！");
	}
}

void VulkanRenderer::createDebugCallback()
{
	// バリデーションが有効な場合のみコールバックを作成する
	if (!validationEnabled) return;

	// デバッグレポートコールバックの作成情報を初期化する
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;				// デバッグレポートコールバックの作成情報の構造体のタイプを指定
	callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;		// 呼び出しを開始するバリデーションレポートの種類
	callbackCreateInfo.pfnCallback = debugCallback;													// コールバック関数へのポインタ

	// カスタムの作成関数を使用してデバッグコールバックを作成する
	VkResult result = CreateDebugReportCallbackEXT(instance, &callbackCreateInfo, nullptr, &callback);
	if (result != VK_SUCCESS)
	{
		//throw std::runtime_error("Failed to create Debug Callback!");
		throw std::runtime_error("デバッグコールバックの作成に失敗しました！");
	}
}

void VulkanRenderer::createLogicalDevice()
{
	// 選択した物理デバイスのキューファミリーインデックスを取得する
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	// キュー作成情報用のベクターとファミリーインデックス用のセットを準備する
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> queueFamilyIndices = { indices.graphicsFamily, indices.presentationFamily };

	// ロジカルデバイスで作成する必要があるキューとその情報を設定する
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;                        // キューを作成するファミリーのインデックス
		queueCreateInfo.queueCount = 1;                                             // 作成するキューの数
		float priority = 1.0f;														// 優先度
		queueCreateInfo.pQueuePriorities = &priority;                               // Vulkanは複数のキューをどのように扱うかを知る必要があるため、優先度を指定する (1が最高優先度)

		queueCreateInfos.push_back(queueCreateInfo);
	}

	// ロジカルデバイスを作成するための情報を設定する
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // キュー作成情報の数
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // 有効なロジカルデバイス拡張機能の数
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // 有効なロジカルデバイス拡張機能のリスト

	// ロジカルデバイスが使用する物理デバイスの機能
	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;            // ロジカルデバイスが使用する物理デバイスの機能

	// 指定された物理デバイスに対してロジカルデバイスを作成する
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("ロジカルデバイスの作成に失敗しました！");
	}

	// キューはデバイスと同時に作成されるため
	// キューへのハンドルを取得する
	// 指定されたロジカルデバイスから、指定されたキューファミリーの指定されたキューインデックス（0は唯一のキューなので0）、指定されたVkQueueへの参照を置く
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.presentationFamily, 0, &presentationQueue);
}

void VulkanRenderer::createSurface()
{
	// サーフェスを作成する（サーフェス作成情報構造体を作成し、サーフェス作成関数を実行し、結果を返す）
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("サーフェスの作成に失敗しました！");
	}
}


void VulkanRenderer::createSwapChain()
{
	// Swap Chainの詳細を取得して最適な設定を選択する
	SwapChainDetails swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);

	// Swap Chainの最適なSurfaceフォーマットを選択する
	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapChainDetails.formats);

	// Swap Chainの最適なプレゼンテーションモードを選択する
	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentationModes);

	// Swap Chainの最適なExtent（サイズ）を選択する
	VkExtent2D extent = chooseSwapExtent(swapChainDetails.surfaceCapabilities);

	// Swap Chainに含まれる画像の数を決定する
	uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;

	// もしimageCountがmaxを超えていたらmaxに制限する
	if (swapChainDetails.surfaceCapabilities.maxImageCount > 0
		&& swapChainDetails.surfaceCapabilities.maxImageCount < imageCount)
	{
		imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;
	}


	/*//////////////////////////
	* スワップチェインの作成
	*///////////////////////////
	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;					// スワップチェインのタイプ
	swapChainCreateInfo.surface = surface;														// スワップチェインの対象となるSurface
	swapChainCreateInfo.imageFormat = surfaceFormat.format;										// スワップチェインの画像フォーマット
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;								// スワップチェインのカラースペース
	swapChainCreateInfo.presentMode = presentMode;												// スワップチェインのプレゼンテーションモード
	swapChainCreateInfo.imageExtent = extent;													// スワップチェインの画像のサイズ
	swapChainCreateInfo.minImageCount = imageCount;												// スワップチェイン内の最小画像数
	swapChainCreateInfo.imageArrayLayers = 1;													// 画像の配列レイヤー数
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;						// 画像がどのように使用されるか
	swapChainCreateInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;	// Swapchain画像に対して行う変換
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;						// 外部グラフィックスとのブレンディングの扱い方
	swapChainCreateInfo.clipped = VK_TRUE;														// 画面外の部分をクリップするかどうか

	// キューファミリーインデックスを取得する
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	// グラフィクスファミリーとプレゼンテーションファミリーが異なる場合、画像をファミリー間で共有する必要がある
	if (indices.graphicsFamily != indices.presentationFamily)
	{
		// 異なる場合
		// 共有するキューの指定
		uint32_t queueFamilyIndices[] = {
			static_cast<uint32_t>(indices.graphicsFamily),
			static_cast<uint32_t>(indices.presentationFamily)
		};

		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;		// 画像共有のモード
		swapChainCreateInfo.queueFamilyIndexCount = 2;							// 画像を共有するキューの数
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;			// 画像を共有するキューの配列
	}
	else
	{
		// 同じだった場合
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;		// 画像共有のモード
		swapChainCreateInfo.queueFamilyIndexCount = 0;							// 画像を共有するキューの数
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;						// 画像を共有するキューの配列
	}

	// 古いSwapchainが破棄され、新しいSwapchainが置き換えられる場合、旧Swapchainをリンクして責任を迅速に引き継ぐ
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	// Swapchainを作成する
	VkResult result = vkCreateSwapchainKHR(mainDevice.logicalDevice, &swapChainCreateInfo, nullptr, &swapchain);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Swapchainの作成に失敗しました！");
	}

	// 後で参照するために保存する
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	// Swapchainの画像を取得する（数を取得し、値を取得する）
	uint32_t swapChainImageCount;	//スワップチェインの画像数
	vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapChainImageCount, nullptr);
	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(mainDevice.logicalDevice, swapchain, &swapChainImageCount, images.data());

	for (VkImage image : images)
	{
		// 画像ハンドルを保存する
		SwapchainImage swapChainImage = {};
		swapChainImage.image = image;
		swapChainImage.imageView = createImageView(image, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);

		// Swapchain画像リストに追加する
		swapChainImages.push_back(swapChainImage);
	}
}

void VulkanRenderer::createGraphicsPipeline()
{
	// シェーダーのSPIR-Vコードを読み込む
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// シェーダーモジュールの作成
	VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
	VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

	// -- シェーダーステージの作成情報 --
	// 頂点シェーダーステージの作成情報
	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;	// 頂点シェーダーステージ作成情報の構造体タイプを設定
	vertexShaderCreateInfo.pNext = nullptr;												// この構造体を拡張する構造体へのポインタ
	vertexShaderCreateInfo.flags;														// シェーダ ステージの生成方法を指定するVkPipelineShaderStageCreateFlagBitsのビットマスク(調べたけどよく分からなかった)
	vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;							// シェーダーステージの種類
	vertexShaderCreateInfo.module = vertexShaderModule;									// 使用するシェーダーモジュール
	vertexShaderCreateInfo.pName = "main";												// シェーダーのエントリーポイント名

	// フラグメントシェーダーステージの作成情報
	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;	// フラグメントシェーダーステージ作成情報の構造体タイプを設定
	fragmentShaderCreateInfo.pNext = nullptr;												// この構造体を拡張する構造体へのポインタ
	fragmentShaderCreateInfo.flags;															// シェーダ ステージの生成方法を指定するVkPipelineShaderStageCreateFlagBitsのビットマスク
	fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;							// シェーダーステージの種類
	fragmentShaderCreateInfo.module = fragmentShaderModule;									// 使用するシェーダーモジュール
	fragmentShaderCreateInfo.pName = "main";												// シェーダーのエントリーポイント名


	// シェーダーステージ作成情報を配列に格納
	// グラフィックスパイプラインの作成情報にはシェーダーステージ作成情報の配列が必要
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	// パイプラインの作成

	// パイプラインが作成された後にはシェーダーモジュールは不要になるため破棄する
	vkDestroyShaderModule(mainDevice.logicalDevice, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(mainDevice.logicalDevice, vertexShaderModule, nullptr);
}



void VulkanRenderer::getPhysicalDevice()
{
	// vkInstanceがアクセスできる物理デバイスを列挙する
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// 利用可能なデバイスがない場合
	if (deviceCount == 0)
	{
		throw std::runtime_error("VulkanをサポートするGPUが見つかりません！");
	}

	// 物理デバイスのリストを取得する
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

	// 適切なデバイスが見つかるまでループする
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
	// Vulkanのインスタンスがサポートする利用可能な拡張機能の数を取得する
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// 利用可能な拡張機能の情報を保持するためのVkExtensionPropertiesのリストを作成する
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// 指定された拡張機能が利用可能な拡張機能リストに含まれているかチェックする
	for (const auto& checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// 拡張機能名が一致するかどうかをstrcmpで比較する
			if (strcmp(checkExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// 指定された拡張機能が見つからない場合はfalseを返す
		if (!hasExtension)
		{
			return false;
		}
	}

	// すべての指定された拡張機能が見つかった場合はtrueを返す
	return true;
}


bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// デバイスがサポートする拡張機能の数を取得する
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// もし拡張機能が見つからない場合、失敗としてfalseを返す
	if (extensionCount == 0)
	{
		return false;
	}

	// 拡張機能の情報を保持するためのVkExtensionPropertiesのリストを作成する
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

	// デバイスが必要とする拡張機能がサポートされているかチェックする
	for (const auto& deviceExtension : deviceExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// 拡張機能名が一致するかどうかをstrcmpで比較する
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// 必要な拡張機能が見つからない場合はfalseを返す
		if (!hasExtension)
		{
			return false;
		}
	}

	// すべての必要な拡張機能が見つかった場合はtrueを返す
	return true;
}


bool VulkanRenderer::checkValidationLayerSupport()
{
	// バリデーションレイヤーの数を取得し、適切なサイズのベクターを作成する
	uint32_t validationLayerCount;
	vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

	// もしバリデーションレイヤーが見つからない場合かつ少なくとも1つのレイヤーが必要な場合はfalseを返す
	if (validationLayerCount == 0 && validationLayers.size() > 0)
	{
		return false;
	}

	// 利用可能なレイヤーの情報を保持するためのVkLayerPropertiesのリストを作成する
	std::vector<VkLayerProperties> availableLayers(validationLayerCount);
	vkEnumerateInstanceLayerProperties(&validationLayerCount, availableLayers.data());

	// 指定されたバリデーションレイヤーが利用可能なレイヤーリストに含まれているかチェックする
	for (const auto& validationLayer : validationLayers)
	{
		bool hasLayer = false;
		for (const auto& availableLayer : availableLayers)
		{
			// レイヤー名が一致するかどうかをstrcmpで比較する
			if (strcmp(validationLayer, availableLayer.layerName) == 0)
			{
				hasLayer = true;
				break;
			}
		}

		// 指定されたバリデーションレイヤーが見つからない場合はfalseを返す
		if (!hasLayer)
		{
			return false;
		}
	}

	// すべての指定されたバリデーションレイヤーが見つかった場合はtrueを返す
	return true;
}


bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	/*
	// デバイス自体に関する情報 (ID、名前、タイプ、ベンダーなど)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// デバイスの機能に関する情報 (ジオメトリシェーダー、テッセレーションシェーダー、ワイドラインなど)
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	*/

	// キューファミリーのインデックスを取得する
	QueueFamilyIndices indices = getQueueFamilies(device);

	// デバイスが必要とする拡張機能をサポートしているか確認する
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	// Swapchainが有効かどうかを確認する
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// 特定の物理デバイスに対するSwap Chainの詳細を取得する
		SwapChainDetails swapChainDetails = getSwapChainDetails(device);

		// Swap Chainの有効性を確認する。プレゼンテーションモードが空でなく、フォーマットも空でない場合に有効とみなす。
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// デバイスが適合しているかを確認する
	return indices.isValid() && extensionsSupported && swapChainValid;
}


QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// 物理デバイスに対するすべてのキューファミリープロパティ情報を取得する
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	// 各キューファミリーを調べ、必要なタイプのキューを少なくとも1つ持っているかどうかを確認する
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// まず、キューファミリーが少なくとも1つのキューを持っているか確認する（キューがない可能性もある）
		// キューはビットフィールドで複数のタイプを定義することができる。VK_QUEUE_*_BITとビットごとのAND演算を行い、必要なタイプを持っているか確認する
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// キューファミリーが有効であれば、そのインデックスを取得する
		}

		// キューファミリーがプレゼンテーションをサポートしているか確認する
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
		// キューがプレゼンテーションタイプであるかどうかを確認する（グラフィックスとプレゼンテーションの両方になり得る）
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		// キューファミリーのインデックスが有効な状態にあるかどうかを確認し、そうであれば検索を停止する
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
	// 特定の物理デバイスとサーフェスに対する表面のキャビティを取得する
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails.surfaceCapabilities);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	// フォーマットが返された場合、フォーマットのリストを取得する
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails.formats.data());
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, nullptr);

	// プレゼンテーションモードが返された場合、プレゼンテーションモードのリストを取得する
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}

	return swapChainDetails;
}


// ベストなフォーマットの選択は主観的ですが、通常以下のようにします:
// format     :   VK_FORMAT_R8G8B8A8_UNORM (バックアップとして VK_FORMAT_B8G8R8A8_UNORM)
// colorSpace :   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
VkSurfaceFormatKHR VulkanRenderer::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	// フォーマットが1つだけあり、未定義の場合、すべてのフォーマットが利用可能とみなす（制限なし）
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	// 制限されている場合、最適なフォーマットを探す
	for (const auto& format : formats)
	{
		if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM)
			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}

	// 最適なフォーマットが見つからない場合、最初のフォーマットを返す
	return formats[0];
}


VkPresentModeKHR VulkanRenderer::chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes)
{
	// メールボックス（Mailbox）プレゼンテーションモードを探す
	// メールボックスモードは、バッファのスワップ処理を行う際に、最新のフレームのみを保持し、古いフレームを破棄するため、
	// レンダリングの遅延を最小限に抑えることができます。
	// もしメールボックスモードが利用可能であれば、それを選択します。
	for (const auto& presentationMode : presentationModes)
	{
		if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentationMode;
		}
	}

	// メールボックスモードが見つからない場合、Vulkan 仕様により FIFO モードを使用することが保証されています。
	// FIFO モードは、キューに溜まったフレームを順番に処理するため、レンダリングの安定性が保たれますが、
	// レイテンシは比較的高くなる場合があります。
	return VK_PRESENT_MODE_FIFO_KHR;
}

#pragma region VkPresentModeKHR解説
// VkPresentModeKHR	: それぞれのモードについての解説(ChatGPTで作成)
// 
//  VK_PRESENT_MODE_IMMEDIATE_KHR		即時モード: 垂直同期なしで画面更新が即座に行われる。                
// 即座に反応性の高い描画を求める場合に適しています。
// 例えば、リアルタイムの操作に対して遅延を最小限に抑えたい場合や、応答速度が重要なアプリケーションに適しています。
// 
//  VK_PRESENT_MODE_MAILBOX_KHR			メールボックスモード: フレームバッファの交換が新しいフレームに切り替わるたびに行われる。
// フレームレートが安定しており、最新のフレームをすぐに描画する必要がある場合に適しています。
// 例えば、ゲームなどで描画遅延を最小限に抑えたい場合に利用されます。
// 
//  VK_PRESENT_MODE_FIFO_KHR			FIFOモード: フレームバッファの交換がV-Syncと同期され、次のV-Blankまで待機する。
// 最も一般的なモードで、フレームバッファの交換がV-Syncに同期され、画面のティアリング（画面の切れ目）を防ぐために使用されます。
// 安定した描画が求められる多くのアプリケーションで利用されます。
// 
//  VK_PRESENT_MODE_FIFO_RELAXED_KHR	FIFOリラックスモード: V-Syncが有効な場合は通常のFIFOモードと同じで、V-Syncが無効な場合は即座にバッファが交換される。
// V-Syncがオフの場合や、可変フレームレート（VRR）をサポートするディスプレイでの使用が推奨されます。
// 動的なフレームレートの変化に対応する必要がある場合に役立ちます。
// 
//	VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR 需要リフレッシュ共有モード: 特定のリフレッシュ要求に基づいて画面更新が行われる。
// 需要に応じて画面更新を行うことで、省電力モードでの使用が想定されています。
// バッテリー駆動のデバイスや省電力を重視するモバイルデバイスで有効です。
// 
//  VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR 連続リフレッシュ共有モード: 連続的なリフレッシュ要求に基づいて画面更新が行われる。
// VR（仮想現実）やAR（拡張現実）アプリケーションなど、連続的な高速なリフレッシュが必要な場面で利用されます。
// 低遅延でのリアルタイムな描画が求められる環境に適しています。
#pragma endregion





VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities)
{
	// 現在の extent が数値の限界にある場合、extent は変化する可能性があります。それ以外の場合はウィンドウのサイズになります。
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		// 数値の限界でない場合、現在の extent を返します。
		return surfaceCapabilities.currentExtent;
	}
	else
	{
		// extent が変化する場合は手動で設定する必要があります。

		// ウィンドウのサイズを取得します
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// ウィンドウサイズを使用して新しい extent を作成します
		VkExtent2D newExtent = {};
		newExtent.width = static_cast<uint32_t>(width);
		newExtent.height = static_cast<uint32_t>(height);

		// Surface は最大および最小の extent を定義していますので、値が境界内にあることを確認するためにクランプします
		newExtent.width = std::max(surfaceCapabilities.minImageExtent.width, std::min(surfaceCapabilities.maxImageExtent.width, newExtent.width));
		newExtent.height = std::max(surfaceCapabilities.minImageExtent.height, std::min(surfaceCapabilities.maxImageExtent.height, newExtent.height));

		return newExtent;
	}
}


VkImageView VulkanRenderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	// 画像ビュー作成情報の初期化
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;		 // 画像ビュー作成情報の構造体タイプを設定
	viewCreateInfo.image = image;                                            // View を作成するための Image
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;                         // Image の種類 (1D, 2D, 3D, Cube など)
	viewCreateInfo.format = format;                                          // Image データのフォーマット
	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;             // RGBA コンポーネントを他の RGBA 値にリマップすることができます
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	// Subresource は Image の一部だけを表示するための設定です
	viewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
	viewCreateInfo.subresourceRange.baseMipLevel = 0;                         // 表示を開始する Mipmap レベル
	viewCreateInfo.subresourceRange.levelCount = 1;                           // 表示する Mipmap レベルの数
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // 表示を開始する配列レベル
	viewCreateInfo.subresourceRange.layerCount = 1;                           // 表示する配列レベルの数

	// Image View を作成し、そのハンドルを返します
	VkImageView imageView;  // Image View のハンドルを保持する変数を宣言します

	// vkCreateImageView 関数を使用して Image View を作成します
	// mainDevice.logicalDevice: Image View を作成するための論理デバイス
	// &viewCreateInfo: Image View の作成に必要な情報が格納された構造体へのポインタ
	// nullptr: カスタムのアロケーターを使用しないためのオプション (通常は nullptr を指定します)
	// &imageView: 作成された Image View のハンドルを受け取る変数へのポインタ
	VkResult result = vkCreateImageView(mainDevice.logicalDevice, &viewCreateInfo, nullptr, &imageView);

	// vkCreateImageView の結果が成功ではない場合、エラーをスローします
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create an Image View!");
	}

	// 作成した Image View のハンドルを返します
	return imageView;
}

void VulkanRenderer::createRenderPass()
{
	// レンダーパスのカラーアタッチメント
	VkAttachmentDescription colourAttachment = {};						// カラーアタッチメントの設定を格納する構造体
	colourAttachment.format = swapChainImageFormat;						// アタッチメントに使用するフォーマットを設定
	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;					// マルチサンプリング用のサンプル数を設定
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;				// レンダリング前にアタッチメントをクリアする設定
	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;			// レンダリング後にアタッチメントを保存する設定
	colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;	// ステンシルバッファを使用しないため無視
	colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // ステンシルバッファを使用しないため無視

	// フレームバッファデータはイメージとして保存されるが、特定の操作に最適なように異なるデータレイアウトを与えることができる
	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;		// レンダーパス開始前のイメージデータレイアウトを未定義に設定
	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // レンダーパス後のイメージデータレイアウトをプレゼント用に設定

	// アタッチメント参照は、renderPassCreateInfoに渡されるアタッチメントリスト内のインデックスを参照するアタッチメントインデックスを使用する
	VkAttachmentReference colourAttachmentReference = {};							// カラーアタッチメントの参照を格納する構造体
	colourAttachmentReference.attachment = 0;										// アタッチメントリストの最初のアタッチメントを指す
	colourAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;	// 最適なカラーアタッチメントレイアウトを設定

	// レンダーパスが使用する特定のサブパスに関する情報
	VkSubpassDescription subpass = {};								// サブパスの設定を格納する構造体を初期化
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;	// グラフィックスパイプラインにバインドするサブパスを設定
	subpass.colorAttachmentCount = 1;								// カラーアタッチメントの数を設定
	subpass.pColorAttachments = &colourAttachmentReference;			// カラーアタッチメントの参照を設定

	// サブパス依存関係を使用してレイアウト遷移が発生するタイミングを決定する必要がある
	std::array<VkSubpassDependency, 2> subpassDependencies;			// サブパスの依存関係を格納する配列を初期化

	// VK_IMAGE_LAYOUT_UNDEFINEDからVK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMALへの変換
	// 遷移は以下の後に発生しなければならない
	subpassDependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;					// 外部サブパスからの依存関係を設定
	subpassDependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; // パイプラインの最終段階からの依存関係を設定
	subpassDependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;			// メモリ読み取りアクセスの依存関係を設定
	// しかし以下の前に発生しなければならない
	subpassDependencies[0].dstSubpass = 0;  // 最初のサブパスへの依存関係を設定
	subpassDependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	// カラーアタッチメント出力ステージへの依存関係を設定
	subpassDependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |			// カラーアタッチメントの読み書きアクセスの依存関係を設定
											VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;  
	subpassDependencies[0].dependencyFlags = 0;												// 追加の依存関係フラグを設定

	// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMALからVK_IMAGE_LAYOUT_PRESENT_SRC_KHRへの変換
	// 遷移は以下の後に発生しなければならない
	subpassDependencies[1].srcSubpass = 0;  // 最初のサブパスからの依存関係を設定
	subpassDependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;	// カラーアタッチメント出力ステージからの依存関係を設定
	subpassDependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |			// カラーアタッチメントの読み書きアクセスの依存関係を設定
											VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;  
	// しかし以下の前に発生しなければならない
	subpassDependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;								// 外部サブパスへの依存関係を設定
	subpassDependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;				// パイプラインの最終段階への依存関係を設定
	subpassDependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;						// メモリ読み取りアクセスの依存関係を設定
	subpassDependencies[1].dependencyFlags = 0;												// 追加の依存関係フラグを設定

	// レンダーパスの作成情報
	VkRenderPassCreateInfo renderPassCreateInfo = {};										// レンダーパスの作成情報を格納する構造体を初期化
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;					// 構造体の型を設定
	renderPassCreateInfo.attachmentCount = 1;												// アタッチメントの数を設定
	renderPassCreateInfo.pAttachments = &colourAttachment;									// カラーアタッチメントの参照を設定
	renderPassCreateInfo.subpassCount = 1;													// サブパスの数を設定
	renderPassCreateInfo.pSubpasses = &subpass;												// サブパスの参照を設定
	renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(subpassDependencies.size());  // サブパス依存関係の数を設定
	renderPassCreateInfo.pDependencies = subpassDependencies.data();						// サブパス依存関係の参照を設定

	// レンダーパスを作成
	VkResult result = vkCreateRenderPass(mainDevice.logicalDevice, &renderPassCreateInfo, nullptr, &renderPass); 
	if (result != VK_SUCCESS)  
	{
		// 作成に失敗した場合
		throw std::runtime_error("Render Passの作成に失敗しました！");  // エラーメッセージをスロー
	}
}

VkShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
{
	// シェーダーモジュール作成情報の設定
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;		// 構造体のタイプを設定
	shaderModuleCreateInfo.codeSize = code.size();									// コードのサイズを指定
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());	// コードのポインタをuint32_tポインタにキャストして設定

	// シェーダーモジュールの作成
	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(mainDevice.logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("シェーダーモジュールの作成に失敗しました！");
	}

	return shaderModule;
}
