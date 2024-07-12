#include "SwapChainUtility.h"


SwapChainDetails SwapChainUtility::getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// 特定の物理デバイスとサーフェスに対する表面のキャビティを取得する
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainDetails.surfaceCapabilities);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// フォーマットが返された場合、フォーマットのリストを取得する
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainDetails.formats.data());
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// プレゼンテーションモードが返された場合、プレゼンテーションモードのリストを取得する
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, swapChainDetails.presentationModes.data());
	}

	return swapChainDetails;
}


/// <summary>
/// スワップチェインの作成
/// </summary>
vk::UniqueSwapchainKHR SwapChainUtility::createSwapchain(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // スワップチェインの設定を行う
    // サーフェスの機能を取得
    vk::SurfaceCapabilitiesKHR surfaceCapabilities = getSurfaceCapabilities(physicalDevice, surface);

    // スワップチェインの設定
    // 適切なサーフェスフォーマットを選択
    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(getSurfaceFormats(physicalDevice, surface));
    // 適切なプレゼンテーションモードを選択
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(getPresentModes(physicalDevice, surface));
    // スワップチェインのエクステント（幅と高さ）を選択
    vk::Extent2D extent = chooseSwapExtent(surfaceCapabilities);

    // スワップチェインに必要なイメージの数を決定
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // スワップチェイン作成情報の設定
    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.pNext;                       // 拡張チェーンへのポインタ（必要に応じて設定）
    createInfo.flags;                       // 作成フラグ（必要に応じて設定）
    createInfo.surface = surface;           // スワップチェインがターゲットとするサーフェス
    createInfo.minImageCount = imageCount;  // スワップチェインに含まれる最小イメージ数
    createInfo.imageFormat = surfaceFormat.format;          // イメージフォーマット
    createInfo.imageColorSpace = surfaceFormat.colorSpace;  // カラースペース
    createInfo.imageExtent = extent;        // スワップチェインの幅と高さ
    createInfo.imageArrayLayers = 1;        // イメージの配列層数
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // イメージの使用用途
    createInfo.imageSharingMode = vk::SharingMode::eExclusive;          // イメージの共有モード（初期設定は排他的モード）
    createInfo.queueFamilyIndexCount;                                   // キューファミリーインデックスの数
    createInfo.pQueueFamilyIndices;                                     // キューファミリーインデックスの配列へのポインタ
    createInfo.preTransform = surfaceCapabilities.currentTransform;     // サーフェスのトランスフォーム
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // アルファ合成モード
    createInfo.presentMode = presentMode;   // プレゼンテーションモード
    createInfo.clipped = VK_TRUE;           // クリップされるかどうか
    createInfo.oldSwapchain = nullptr;      // 古いスワップチェインへのポインタ

    // グラフィックスキューファミリのインデックスを取得する
    // キューファミリのインデックスを取得
    QueueFamilyIndices indices = QueueUtility::getQueueFamilies(physicalDevice, surface);
    // キューファミリーインデックスの配列を作成
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentationFamily };

    // キューファミリが異なる場合は共有モードを設定
    if (indices.graphicsFamily != indices.presentationFamily)
    {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent; // 並行モードに設定
        createInfo.queueFamilyIndexCount = 2;                       // キューファミリーインデックスの数を設定
        createInfo.pQueueFamilyIndices = queueFamilyIndices;        // キューファミリーインデックスの配列を設定
    }

    // スワップチェインを作成する
    // スワップチェインを作成
    vk::UniqueSwapchainKHR swapchain = device.createSwapchainKHRUnique(createInfo);
    // エラーチェックを行う
    if (!swapchain)
    {
        throw std::runtime_error("スワップチェインの作成に失敗しました!");  // エラーが発生した場合は例外を投げる
    }

    return swapchain;  // 作成したスワップチェインを返す
}



// スワップチェインのフォーマットを選択する関数
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


// スワップチェインのプレゼントモードを選択する関数
vk::PresentModeKHR SwapChainUtility::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            return availablePresentMode;
        }
    }

    return vk::PresentModeKHR::eFifo; // フィフォキューを使用する（必ずサポートされている）
}


// スワップチェインのエクステントを選択する関数
vk::Extent2D SwapChainUtility::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        vk::Extent2D actualExtent = { windowWidth, windowHeight }; // 自分のウィンドウのサイズを設定する
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
