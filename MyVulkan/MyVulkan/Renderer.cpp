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
		//createSurface();
		surface			= SurfaceUtilities::createUnique(instance.get(), window);
		physicalDevice	= DeviceUtility::getPhysicalDevice(instance.get(), surface.get());
		logicalDevice   = DeviceUtility::createLogicalDevice(physicalDevice, surface.get());
		swapchain		= SwapChainUtility::createSwapchain(logicalDevice.get(), physicalDevice, surface.get());
		
		renderPass		= RenderPassUtility::createRenderPass(logicalDevice.get(), physicalDevice, surface.get());
		pipelineLayout  = GraphicsPipelineUtility::createPipelineLayout(logicalDevice.get());
		graphicsPipeline = GraphicsPipelineUtility::createGraphicsPipeline(logicalDevice.get(), swapChainExtent, pipelineLayout.get(), renderPass.get());

		swapChainImages = ;//swapChainImagesを作成しておく
		swapChainFramebuffers = SwapChainUtility::createFramebuffers(logicalDevice.get(), swapChainImages, renderPass.get(), swapChainExtent);
		createCommandPool();
		createCommandBuffers();
		recordCommands();
		createSynchronisation();
	}
	catch (const std::runtime_error& e) {
		//エラーメッセージ受け取り
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
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;	// インスタンスのタイプ
	createInfo.pNext;											// 拡張機能の情報 
	createInfo.flags;											// インスタンスの作成フラグ 
	createInfo.pApplicationInfo = &appInfo;						// アプリケーション情報へのポインタ
	createInfo.enabledLayerCount = 0;							// 有効にするレイヤーの数 
	createInfo.ppEnabledLayerNames = nullptr;					// 有効にするレイヤーの名前の配列 
	createInfo.enabledExtensionCount;							// 有効にする拡張機能の数 
	createInfo.ppEnabledExtensionNames;							// 有効にする拡張機能の名前の配列 	

	// インスタンス拡張機能のリストを作成する
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	// インスタンスが使用する拡張機能を設定する
	uint32_t glfwExtensionCount = 0;	// GLFWは複数の拡張機能を要求する場合がある
	const char** glfwExtensions;		// 拡張機能はC文字列の配列として渡されるため、ポインタ(配列)のポインタ(C文字列)が必要

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
		throw std::runtime_error("VkInstance は必要な拡張機能をサポートしていません！");
	}

	// 有効な拡張機能の数と名前の配列を設定する
	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// バリデーションが有効な場合、有効なレイヤーの数と名前の配列を設定する
	if (validationEnabled)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}	

	// インスタンスを作成する
	instance = vk::createInstanceUnique(createInfo);

	// インスタンスの作成に失敗した場合のエラーメッセージ
	if (!instance) {
		throw std::runtime_error("Vulkanインスタンスの作成に失敗しました！");
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
	VkResult result = CreateDebugReportCallbackEXT(instance.get(), &callbackCreateInfo, nullptr, &callback);
	if (result != VK_SUCCESS)
	{
		//throw std::runtime_error("Failed to create Debug Callback!");
		throw std::runtime_error("デバッグコールバックの作成に失敗しました！");
	}
}

/// <summary>
/// 論理デバイスの作成
/// </summary>
void VulkanRenderer::createLogicalDevice()
{
	//論理デバイスの作成に必要なもの
	//1,使用するデバイスの拡張
	//2,使用するデバイスのレイヤー
	//3,デバイスのどのキューを使用するか

	// 選択した物理デバイスのキューファミリーインデックスを取得する
	QueueFamilyIndices queueIndex = getQueueFamilies(physicalDevice);

	// キュー作成情報用のベクター
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	//ファミリーインデックス用のセット
	std::set<int> queueFamilyIndices = { queueIndex.graphicsFamily, queueIndex.presentationFamily };

	// 論理デバイスで作成する必要があるキューとその情報を設定する
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

	// 論理デバイスを作成するための情報を設定する
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // キュー作成情報の数
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // デバイスが必要とするキューを作成するためのキュー作成情報のリスト
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // 有効なロジカルデバイス拡張機能の数
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // 有効なロジカルデバイス拡張機能のリスト

	// 論理デバイスが使用する物理デバイスの機能
	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;            // ロジカルデバイスが使用する物理デバイスの機能

	// 指定された物理デバイスに対してロジカルデバイスを作成する
	logicalDevice = physicalDevice.createDeviceUnique(deviceCreateInfo);
	if (!logicalDevice)
	{
		throw std::runtime_error("ロジカルデバイスの作成に失敗しました！");
	}

	// キューはデバイスと同時に作成されるため
	// キューへのハンドルを取得する
	// 指定されたロジカルデバイスから、指定されたキューファミリーの指定されたキューインデックス（0は唯一のキューなので0）、指定されたVkQueueへの参照を置く
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.presentationFamily, 0, &presentationQueue);
}

//void VulkanRenderer::createSurface()
//{
//
//	VkSurfaceKHR c_surface;
//	// サーフェスを作成する（サーフェス作成情報構造体を作成し、サーフェス作成関数を実行し、結果を返す）
//	VkResult result = glfwCreateWindowSurface(instance.get(), window, nullptr, &c_surface);
//
//	if (result != VK_SUCCESS)
//	{
//		throw std::runtime_error("サーフェスの作成に失敗しました！");
//	}
//
//	// コンストラクタの第二引数で親となるインスタンスの情報を渡す
//	surface = vk::UniqueSurfaceKHR{ c_surface, instance.get() };
//}


//void VulkanRenderer::createSwapChain()
//{
//	// Swap Chainの詳細を取得して最適な設定を選択する
//	SwapChainDetails swapChainDetails = getSwapChainDetails(physicalDevice);
//
//	// Swap Chainの最適なSurfaceフォーマットを選択する
//	VkSurfaceFormatKHR surfaceFormat = chooseBestSurfaceFormat(swapChainDetails.formats);
//
//	// Swap Chainの最適なプレゼンテーションモードを選択する
//	VkPresentModeKHR presentMode = chooseBestPresentationMode(swapChainDetails.presentationModes);
//
//	// Swap Chainの最適なExtent（サイズ）を選択する
//	VkExtent2D extent = chooseSwapExtent(swapChainDetails.surfaceCapabilities);
//
//	// Swap Chainに含まれる画像の数を決定する
//	uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;
//
//	// もしimageCountがmaxを超えていたらmaxに制限する
//	if (swapChainDetails.surfaceCapabilities.maxImageCount > 0
//		&& swapChainDetails.surfaceCapabilities.maxImageCount < imageCount)
//	{
//		imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;
//	}
//
//
//	/*//////////////////////////
//	* スワップチェインの作成
//	*///////////////////////////
//	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
//	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;					// スワップチェインのタイプ
//	swapChainCreateInfo.surface = surface.get();												// スワップチェインの対象となるSurface
//	swapChainCreateInfo.imageFormat = surfaceFormat.format;										// スワップチェインの画像フォーマット
//	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;								// スワップチェインのカラースペース
//	swapChainCreateInfo.presentMode = presentMode;												// スワップチェインのプレゼンテーションモード
//	swapChainCreateInfo.imageExtent = extent;													// スワップチェインの画像のサイズ
//	swapChainCreateInfo.minImageCount = imageCount;												// スワップチェイン内の最小画像数
//	swapChainCreateInfo.imageArrayLayers = 1;													// 画像の配列レイヤー数
//	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;						// 画像がどのように使用されるか
//	swapChainCreateInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;	// Swapchain画像に対して行う変換
//	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;						// 外部グラフィックスとのブレンディングの扱い方
//	swapChainCreateInfo.clipped = VK_TRUE;														// 画面外の部分をクリップするかどうか
//
//	// キューファミリーインデックスを取得する
//	QueueFamilyIndices indices = getQueueFamilies(physicalDevice);
//
//	// グラフィクスファミリーとプレゼンテーションファミリーが異なる場合、画像をファミリー間で共有する必要がある
//	if (indices.graphicsFamily != indices.presentationFamily)
//	{
//		// 異なる場合
//		// 共有するキューの指定
//		uint32_t queueFamilyIndices[] = {
//			static_cast<uint32_t>(indices.graphicsFamily),
//			static_cast<uint32_t>(indices.presentationFamily)
//		};
//
//		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;		// 画像共有のモード
//		swapChainCreateInfo.queueFamilyIndexCount = 2;							// 画像を共有するキューの数
//		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;			// 画像を共有するキューの配列
//	}
//	else
//	{
//		// 同じだった場合
//		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;		// 画像共有のモード
//		swapChainCreateInfo.queueFamilyIndexCount = 0;							// 画像を共有するキューの数
//		swapChainCreateInfo.pQueueFamilyIndices = nullptr;						// 画像を共有するキューの配列
//	}
//
//	// 古いSwapchainが破棄され、新しいSwapchainが置き換えられる場合、旧Swapchainをリンクして責任を迅速に引き継ぐ
//	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
//
//	// Swapchainを作成する
//	swapchain = logicalDevice->createSwapchainKHRUnique(swapChainCreateInfo);
//	if (!swapchain)
//	{
//		throw std::runtime_error("Swapchainの作成に失敗しました！");
//	}
//
//	// 後で参照するために保存する
//	swapChainImageFormat = surfaceFormat.format;
//	swapChainExtent = extent;
//
//	// Swapchainの画像を取得する（数を取得し、値を取得する）
//	uint32_t swapChainImageCount;	//スワップチェインの画像数
//	vkGetSwapchainImagesKHR(logicalDevice.get(), swapchain.get(), &swapChainImageCount, nullptr);
//	std::vector<VkImage> images(swapChainImageCount);
//	vkGetSwapchainImagesKHR(logicalDevice.get(), swapchain.get(), &swapChainImageCount, images.data());
//
//	for (VkImage image : images)
//	{
//		// 画像ハンドルを保存する
//		SwapchainImage swapChainImage = {};
//		swapChainImage.image = image;
//		swapChainImage.imageView = createImageView(image, swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
//
//		// Swapchain画像リストに追加する
//		swapChainImages.push_back(swapChainImage);
//	}
//}

void VulkanRenderer::createGraphicsPipeline()
{
	//// シェーダーのSPIR-Vコードを読み込む
	//auto vertexShaderCode = readFile("Shaders/vert.spv");
	//auto fragmentShaderCode = readFile("Shaders/frag.spv");

	//// シェーダーモジュールを作成する
	//VkShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
	//VkShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

	//// -- シェーダーステージ作成情報 --
	//// 頂点シェーダーステージの作成情報
	//VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	//vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;				// シェーダーステージの種類
	//vertexShaderCreateInfo.module = vertexShaderModule;						// 使用するシェーダーモジュール
	//vertexShaderCreateInfo.pName = "main";									// エントリーポイント

	//// フラグメントシェーダーステージの作成情報
	//VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	//fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;				// シェーダーステージの種類
	//fragmentShaderCreateInfo.module = fragmentShaderModule;						// 使用するシェーダーモジュール
	//fragmentShaderCreateInfo.pName = "main";									// エントリーポイント

	//// シェーダーステージ作成情報を配列に格納
	//VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };


	//// -- 頂点入力 (TODO: リソースが作成されたら頂点の説明を追加する) --
	//VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
	//vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	//vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// 頂点バインディングの説明 (データの間隔やストライド情報)
	//vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	//vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// 頂点属性の説明 (データフォーマットやバインド先/元)


	//// -- 入力アセンブリ --
	//VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	//inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	//inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;		// 頂点をアセンブルする基本図形の種類
	//inputAssembly.primitiveRestartEnable = VK_FALSE;					// "strip" トポロジーを上書きして新しいプリミティブを開始することを許可するか


	//// -- ビューポート & シザー --
	//// ビューポート情報の構造体を作成
	//VkViewport viewport = {};
	//viewport.x = 0.0f;									// x 開始座標
	//viewport.y = 0.0f;									// y 開始座標
	//viewport.width = (float)swapChainExtent.width;		// ビューポートの幅
	//viewport.height = (float)swapChainExtent.height;	// ビューポートの高さ
	//viewport.minDepth = 0.0f;							// フレームバッファの最小深度
	//viewport.maxDepth = 1.0f;							// フレームバッファの最大深度

	//// シザー情報の構造体を作成
	//VkRect2D scissor = {};
	//scissor.offset = { 0,0 };							// 使用する領域のオフセット
	//scissor.extent = swapChainExtent;					// 使用する領域の範囲とオフセットから開始

	//VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	//viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	//viewportStateCreateInfo.viewportCount = 1;
	//viewportStateCreateInfo.pViewports = &viewport;
	//viewportStateCreateInfo.scissorCount = 1;
	//viewportStateCreateInfo.pScissors = &scissor;


	//// -- ダイナミックステート --
	//// 有効にするダイナミックステート
	////std::vector<VkDynamicState> dynamicStateEnables;
	////dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	// ダイナミックビューポート: vkCmdSetViewport(commandbuffer, 0, 1, &viewport); でコマンドバッファ内でリサイズ可能
	////dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	// ダイナミックシザー: vkCmdSetScissor(commandbuffer, 0, 1, &scissor); でコマンドバッファ内でリサイズ可能

	////// ダイナミックステートの作成情報
	////VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	////dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	////dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	////dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();


	//// -- ラスタライザ --
	//VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
	//rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	//rasterizerCreateInfo.depthClampEnable = VK_FALSE;			// フラグメントが近接/遠隔平面を超えた場合にクリップ (デフォルト) または平面にクランプするかを変更
	//rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;	// データを破棄しラスタライザをスキップするかどうか。フレームバッファ出力なしのパイプラインにのみ適している
	//rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;	// 頂点間のポイントの塗りつぶし方法
	//rasterizerCreateInfo.lineWidth = 1.0f;						// 描画時の線の太さ
	//rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;		// 三角形のどの面をカリングするか
	//rasterizerCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;	// 前面を決定するための巻き方向
	//rasterizerCreateInfo.depthBiasEnable = VK_FALSE;			// フラグメントに深度バイアスを追加するか (シャドウマッピングで "影アクネ" を防ぐのに有効)


	//// -- マルチサンプリング --
	//VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = {};
	//multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	//multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;					// マルチサンプルシェーディングを有効にするかどうか
	//multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;	// フラグメントごとに使用するサンプル数


	//// -- ブレンディング --
	//// ブレンディングは、書き込まれる新しい色と古い値をどのようにブレンドするかを決定する

	//// ブレンドアタッチメントステート (ブレンディングの処理方法)
	//VkPipelineColorBlendAttachmentState colourState = {};
	//colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT	// ブレンディングを適用する色
	//	| VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	//colourState.blendEnable = VK_TRUE;													// ブレンディングを有効にするかどうか

	//// ブレンディングは方程式を使用: (srcColorBlendFactor * 新しい色) colorBlendOp (dstColorBlendFactor * 古い色)
	//colourState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	//colourState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	//colourState.colorBlendOp = VK_BLEND_OP_ADD;

	//// 要約: (VK_BLEND_FACTOR_SRC_ALPHA * 新しい色) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * 古い色)
	//// (新しい色のアルファ * 新しい色) + ((1 - 新しい色のアルファ) * 古い色)

	//colourState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	//colourState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	//colourState.alphaBlendOp = VK_BLEND_OP_ADD;
	//// 要約: (1 * 新しいアルファ) + (0 * 古いアルファ) = 新しいアルファ

	//VkPipelineColorBlendStateCreateInfo colourBlendingCreateInfo = {};
	//colourBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	//colourBlendingCreateInfo.logicOpEnable = VK_FALSE;				// 計算の代わりに論理演算を使用するかどうか
	//colourBlendingCreateInfo.attachmentCount = 1;
	//colourBlendingCreateInfo.pAttachments = &colourState;


	//// -- パイプラインレイアウト (TODO: 将来のディスクリプタセットレイアウトを適用する) --
	//VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	//pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	//pipelineLayoutCreateInfo.setLayoutCount = 0;
	//pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	//pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	//pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	//// パイプラインレイアウトを作成
	//VkResult result = vkCreatePipelineLayout(logicalDevice.get(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout.get());
	//if (result != VK_SUCCESS)
	//{
	//	throw std::runtime_error("パイプラインレイアウトの作成に失敗しました！");
	//}


	//// -- 深度ステンシルテスト --
	//// TODO: 深度ステンシルテストの設定


	//// -- グラフィックスパイプラインの作成 --
	//VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	//pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	//pipelineCreateInfo.stageCount = 2;									// シェーダーステージの数
	//pipelineCreateInfo.pStages = shaderStages;							// シェーダーステージのリスト
	//pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;		// すべての固定機能パイプラインステート
	//pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	//pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	//pipelineCreateInfo.pDynamicState = nullptr;
	//pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	//pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	//pipelineCreateInfo.pColorBlendState = &colourBlendingCreateInfo;
	//pipelineCreateInfo.pDepthStencilState = nullptr;
	//pipelineCreateInfo.layout = pipelineLayout.get();							// パイプラインが使用するパイプラインレイアウト
	//pipelineCreateInfo.renderPass = renderPass.get();							// パイプラインが互換性のあるレンダーパスの説明
	//pipelineCreateInfo.subpass = 0;										// パイプラインで使用するサブパス

	//// パイプラインの派生: 最適化のために相互に派生する複数のパイプラインを作成できる
	//pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;              // 派生元の既存のパイプライン...
	//pipelineCreateInfo.basePipelineIndex = -1;                           // または作成中のパイプラインのインデックス (複数作成する場合)

	//// グラフィックスパイプラインを作成
	//result = vkCreateGraphicsPipelines(logicalDevice.get(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline.get());
	//if (result != VK_SUCCESS)
	//{
	//	throw std::runtime_error("グラフィックスパイプラインの作成に失敗しました！");
	//}

	////パイプラインの作成後に不要になったシェーダーモジュールを破棄
	//vkDestroyShaderModule(logicalDevice.get(), fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice.get(), vertexShaderModule, nullptr);
}


void VulkanRenderer::createFramebuffers()
{
	//// スワップチェインの画像数に合わせてフレームバッファの数をリサイズする
	//swapChainFramebuffers.resize(swapChainImages.size());

	//// 各スワップチェイン画像に対してフレームバッファを作成する
	//for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	//{
	//	// VkImageViewの確認
	//	if (swapChainImages[i].imageView == VK_NULL_HANDLE)
	//	{
	//		throw std::runtime_error("ImageViewが無効です！");
	//	}

	//	// フレームバッファに添付するイメージビューの配列を作成する
	//	std::array<VkImageView, 1> attachments = {
	//		swapChainImages[i].imageView
	//	};

	//	// フレームバッファの作成に必要な情報を設定する
	//	VkFramebufferCreateInfo framebufferCreateInfo = {};
	//	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//	framebufferCreateInfo.renderPass = renderPass.get();                                     // フレームバッファが使用されるレンダーパスのレイアウト
	//	framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	//	framebufferCreateInfo.pAttachments = attachments.data();                           // 添付するイメージビューのリスト (レンダーパスと1:1対応)
	//	framebufferCreateInfo.width = swapChainExtent.width;                                // フレームバッファの幅
	//	framebufferCreateInfo.height = swapChainExtent.height;                              // フレームバッファの高さ
	//	framebufferCreateInfo.layers = 1;                                                    // フレームバッファのレイヤー数

	//	// フレームバッファを作成する
	//	VkResult result = vkCreateFramebuffer(logicalDevice.get(), &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
	//	if (result != VK_SUCCESS)
	//	{
	//		throw std::runtime_error("フレームバッファの作成に失敗しました！");
	//	}
	//}
}

void VulkanRenderer::createCommandPool()
{
	// デバイスからキューファミリーのインデックスを取得する
	QueueFamilyIndices queueFamilyIndices = getQueueFamilies(physicalDevice);

	// コマンドプールの作成に必要な情報を設定する
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;   // このコマンドプールが使用するキューファミリータイプ

	// グラフィックスキューファミリー用のコマンドプールを作成する
	VkResult result = vkCreateCommandPool(logicalDevice.get(), &poolInfo, nullptr, &graphicsCommandPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("コマンドプールの作成に失敗しました！");
	}
}

void VulkanRenderer::createCommandBuffers()
{
	// コマンドバッファの数をフレームバッファごとにリサイズする
	commandBuffers.resize(swapChainFramebuffers.size());

	// コマンドバッファを割り当てるための情報を設定する
	VkCommandBufferAllocateInfo cbAllocInfo = {};
	cbAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbAllocInfo.commandPool = graphicsCommandPool;                                  // コマンドバッファを割り当てるコマンドプール
	cbAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;                            // コマンドバッファのレベル (PRIMARY: 直接キューに送信するバッファ)
	cbAllocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());  // 割り当てるコマンドバッファの数

	// コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
	VkResult result = vkAllocateCommandBuffers(logicalDevice.get(), &cbAllocInfo, commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("コマンドバッファの割り当てに失敗しました！");
	}
}

void VulkanRenderer::createSynchronisation()
{
	// MAX_FRAME_DRAWS 分のセマフォとフェンスをリサイズする
	imageAvailable.resize(MAX_FRAME_DRAWS);
	renderFinished.resize(MAX_FRAME_DRAWS);
	drawFences.resize(MAX_FRAME_DRAWS);

	// セマフォの作成情報を設定する
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// フェンスの作成情報を設定する
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // 初期状態でシグナルされた状態のフェンスを作成する

	// MAX_FRAME_DRAWS 分のセマフォとフェンスを作成するループ
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		// セマフォとフェンスを作成する
		if (vkCreateSemaphore(logicalDevice.get(), &semaphoreCreateInfo, nullptr, &imageAvailable[i]) != VK_SUCCESS ||
			vkCreateSemaphore(logicalDevice.get(), &semaphoreCreateInfo, nullptr, &renderFinished[i]) != VK_SUCCESS ||
			vkCreateFence(logicalDevice.get(), &fenceCreateInfo, nullptr, &drawFences[i]) != VK_SUCCESS)
		{
			// 作成に失敗した場合は例外を投げる
			throw std::runtime_error("Failed to create a Semaphore and/or Fence!");
		}
	}
}



void VulkanRenderer::recordCommands()
{
	// 各コマンドバッファの開始方法に関する情報
	VkCommandBufferBeginInfo bufferBeginInfo = {};
	bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	bufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;   // バッファが再使用可能であることを示すフラグ

	// レンダーパスを開始するための情報 (グラフィカルなアプリケーションの場合のみ必要)
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass.get();                             // 開始するレンダーパス
	renderPassBeginInfo.renderArea.offset = { 0, 0 };                        // レンダーパスの開始位置 (ピクセル単位)
	renderPassBeginInfo.renderArea.extent = swapChainExtent;                 // レンダーパスを実行する領域のサイズ (offsetから始まる)
	VkClearValue clearValues[] = {
		{0.6f, 0.65f, 0.4, 1.0f}                                            // クリアする値のリスト (TODO: 深度アタッチメントのクリア値)
	};
	renderPassBeginInfo.pClearValues = clearValues;                          // クリアする値のリスト
	renderPassBeginInfo.clearValueCount = 1;

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		renderPassBeginInfo.framebuffer = swapChainFramebuffers[i].get();          // 使用するフレームバッファを設定する

		// コマンドバッファの記録を開始する
		VkResult result = vkBeginCommandBuffer(commandBuffers[i], &bufferBeginInfo);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("コマンドバッファの記録の開始に失敗しました！");
		}

		// レンダーパスを開始する
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		// 使用するパイプラインをバインドする
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.get());	//ここでエラー

		// パイプラインを実行する
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		// レンダーパスを終了する
		vkCmdEndRenderPass(commandBuffers[i]);

		// コマンドバッファの記録を終了する
		result = vkEndCommandBuffer(commandBuffers[i]);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("コマンドバッファの記録の終了に失敗しました！");
		}
	}
}

/// <summary>
/// 物理デバイス(GPU)の取得
/// </summary>
void VulkanRenderer::getPhysicalDevice()
{
	//インスタンスから物理デバイスを取得
	std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

	// 適切なデバイスが見つかるまでループする
	for (const auto& device : physicalDevices)
	{
		if (checkDeviceSuitable(device))
		{
			// 適切なデバイスが見つかった
			physicalDevice = device;
			return;
		}
	}

	// 利用可能なデバイスがない場合
	throw std::runtime_error("VulkanをサポートするGPUが見つかりません！");

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

/// <summary>
/// 指定された物理デバイスが Vulkan レンダラーに適しているかどうかを確認する
/// </summary>
//bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
//{
	///*
	//// デバイス自体に関する情報 (ID、名前、タイプ、ベンダーなど)
	//VkPhysicalDeviceProperties deviceProperties;
	//vkGetPhysicalDeviceProperties(device, &deviceProperties);

	//// デバイスの機能に関する情報 (ジオメトリシェーダー、テッセレーションシェーダー、ワイドラインなど)
	//VkPhysicalDeviceFeatures deviceFeatures;
	//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	//*/

	//// キューファミリーのインデックスを取得する
	//QueueFamilyIndices indices = getQueueFamilies(device);

	//// デバイスが必要とする拡張機能をサポートしているか確認する
	//bool extensionsSupported = checkDeviceExtensionSupport(device);

	//// Swapchainが有効かどうかを確認する
	//bool swapChainValid = false;
	//if (extensionsSupported)
	//{
	//	// 特定の物理デバイスに対するSwap Chainの詳細を取得する
	//	SwapChainDetails swapChainDetails = getSwapChainDetails(device);

	//	// Swap Chainの有効性を確認する。プレゼンテーションモードが空でなく、フォーマットも空でない場合に有効とみなす。
	//	swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	//}

	//// デバイスが適合しているかを確認する
	//return indices.isValid() && extensionsSupported && swapChainValid;
//}


//QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
//{
	//QueueFamilyIndices indices;

	//// 物理デバイスに対するすべてのキューファミリープロパティ情報を取得する
	//uint32_t queueFamilyCount = 0;
	//vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	//std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	//vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	//// 各キューファミリーを調べ、必要なタイプのキューを少なくとも1つ持っているかどうかを確認する
	//int i = 0;
	//for (const auto& queueFamily : queueFamilyList)
	//{
	//	// まず、キューファミリーが少なくとも1つのキューを持っているか確認する（キューがない可能性もある）
	//	// キューはビットフィールドで複数のタイプを定義することができる。VK_QUEUE_*_BITとビットごとのAND演算を行い、必要なタイプを持っているか確認する
	//	if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
	//	{
	//		indices.graphicsFamily = i;		// キューファミリーが有効であれば、そのインデックスを取得する
	//	}

	//	// キューファミリーがプレゼンテーションをサポートしているか確認する
	//	VkBool32 presentationSupport = false;
	//	vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.get(), &presentationSupport);
	//	// キューがプレゼンテーションタイプであるかどうかを確認する（グラフィックスとプレゼンテーションの両方になり得る）
	//	if (queueFamily.queueCount > 0 && presentationSupport)
	//	{
	//		indices.presentationFamily = i;
	//	}

	//	// キューファミリーのインデックスが有効な状態にあるかどうかを確認し、そうであれば検索を停止する
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
//	// 特定の物理デバイスとサーフェスに対する表面のキャビティを取得する
//	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.get(), &swapChainDetails.surfaceCapabilities);
//
//	// -- FORMATS --
//	uint32_t formatCount = 0;
//	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.get(), &formatCount, nullptr);
//
//	// フォーマットが返された場合、フォーマットのリストを取得する
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
//	// プレゼンテーションモードが返された場合、プレゼンテーションモードのリストを取得する
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
//// ベストなフォーマットの選択は主観的ですが、通常以下のようにします:
//// format     :   VK_FORMAT_R8G8B8A8_UNORM (バックアップとして VK_FORMAT_B8G8R8A8_UNORM)
//// colorSpace :   VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
//VkSurfaceFormatKHR VulkanRenderer::chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
//{
//	// フォーマットが1つだけあり、未定義の場合、すべてのフォーマットが利用可能とみなす（制限なし）
//	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
//	{
//		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
//	}
//
//	// 制限されている場合、最適なフォーマットを探す
//	for (const auto& format : formats)
//	{
//		if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM)
//			&& format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
//		{
//			return format;
//		}
//	}
//
//	// 最適なフォーマットが見つからない場合、最初のフォーマットを返す
//	return formats[0];
//}


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
	VkResult result = vkCreateImageView(logicalDevice.get(), &viewCreateInfo, nullptr, &imageView);

	// vkCreateImageView の結果が成功ではない場合、エラーをスローします
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create an Image View!");
	}

	// 作成した Image View のハンドルを返します
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
	// シェーダーモジュール作成情報の設定
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;		// 構造体のタイプを設定
	shaderModuleCreateInfo.codeSize = code.size();									// コードのサイズを指定
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());	// コードのポインタをuint32_tポインタにキャストして設定

	// シェーダーモジュールの作成
	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(logicalDevice.get(), &shaderModuleCreateInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("シェーダーモジュールの作成に失敗しました！");
	}

	return shaderModule;
}
