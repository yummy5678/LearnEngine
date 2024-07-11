#include "SwapChain.h"


//vk::UniqueSwapchainKHR SwapChain::create()
//{
//	// Swap Chainの詳細を取得して最適な設定を選択する
//	SwapChainDetails swapChainDetails = getSwapChainDetails(mainDevice.physicalDevice);
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
//	swapChainCreateInfo.surface = surface;														// スワップチェインの対象となるSurface
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
//	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);
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
//	swapchain = mainDevice.logicalDevice->createSwapchainKHRUnique(swapChainCreateInfo);
//	if (!swapchain)
//	{
//		throw std::runtime_error("Swapchainの作成に失敗しました！");
//	}
//
//	// 後で参照するために保存する
//	swapChainImageFormat = surfaceFormat.format;
//	swapChainExtent = extent;
//
//}
