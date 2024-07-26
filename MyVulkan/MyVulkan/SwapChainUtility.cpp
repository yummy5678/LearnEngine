#include "SwapChainUtility.h"



SwapChainDetails VulkanUtility::getSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
    // 特定の物理デバイスとサーフェスに対する表面のキャパビリティを取得する
    swapChainDetails.surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// フォーマットが返された場合、フォーマットのリストを取得する
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
        swapChainDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// プレゼンテーションモードが返された場合、プレゼンテーションモードのリストを取得する
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
        swapChainDetails.presentationModes = physicalDevice.getSurfacePresentModesKHR(surface);
	}

	return swapChainDetails;
}


// スワップチェインのフォーマットを選択する関数
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


// スワップチェインのプレゼントモードを選択する関数
vk::PresentModeKHR VulkanUtility::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
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
vk::Extent2D VulkanUtility::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
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
    // スワップチェーンを構成するイメージのベクターを取得
    std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain);


    // Swapchainの画像を格納するベクターを作成
    std::vector<SwapchainImage> swapChainImages;
    swapChainImages.reserve(images.size());

    // スワップチェイン作成時に取得したのと同じ情報が欲しい
    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(getSurfaceFormats(physicalDevice, surface));

    for (vk::Image image : images)
    {
        // 画像ハンドルを保存する
        SwapchainImage swapChainImage = {};
        swapChainImage.image = image;
        //フォーマットはchooseSwapSurfaceFormatで使用したのと同じものでなければならない
        swapChainImage.imageView = createImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor);
        
        // Swapchain画像リストに追加する
        swapChainImages.push_back(swapChainImage);
    }
    return swapChainImages;
}

vk::ImageView VulkanUtility::createImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
    // 画像ビュー作成情報の初期化
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.image = image;                                            // View を作成するための Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image の種類 (1D, 2D, 3D, Cube など)
    imageViewCreateInfo.format = format;                                          // Image データのフォーマット
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA コンポーネントを他の RGBA 値にリマップすることができます
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
    
    // Subresource は Image の一部だけを表示するための設定です
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;                         // 表示を開始する Mipmap レベル
    imageViewCreateInfo.subresourceRange.levelCount = 1;                           // 表示する Mipmap レベルの数
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // 表示を開始する配列レベル
    imageViewCreateInfo.subresourceRange.layerCount = 1;                           // 表示する配列レベルの数

    // vkCreateImageView 関数を使用して Image View を作成します
    // mainDevice.logicalDevice: Image View を作成するための論理デバイス
    // &viewCreateInfo: Image View の作成に必要な情報が格納された構造体へのポインタ
    // nullptr: カスタムのアロケーターを使用しないためのオプション (通常は nullptr を指定します)
    // &imageView: 作成された Image View のハンドルを受け取る変数へのポインタ
    vk::ImageView imageView = logicalDevice.createImageView(imageViewCreateInfo);
    // vkCreateImageView の結果が成功ではない場合、エラーをスローします
    if (!imageView)
    {
        throw std::runtime_error("Failed to create an Image View!");
    }

    // 作成した Image View のハンドルを返します
    return imageView;
}

std::vector<vk::Framebuffer> VulkanUtility::createFramebuffers(vk::Device logicalDevice,std::vector<SwapchainImage> swapChainImages, vk::RenderPass renderPass, vk::Extent2D extent)
{
    // logicalDeviceが有効であるか確認
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDeviceが無効です！");
    }

    // renderPassが有効であるか確認
    if (!renderPass)
    {
        throw std::runtime_error("renderPassが無効です！");
    }

    // swapChainImagesが空でないことを確認
    if (swapChainImages.empty())
    {
        throw std::runtime_error("swapChainImagesが空です！");
    }

    std::vector<vk::Framebuffer> framebuffers;
    framebuffers.reserve(swapChainImages.size());

    for (const auto& swapChainImage : swapChainImages)
    {
        if (!swapChainImage.imageView)
        {
            throw std::runtime_error("ImageViewが無効です！");
        }

        std::array<vk::ImageView, 1> attachments = { swapChainImage.imageView };

        vk::FramebufferCreateInfo framebufferCreateInfo = {};
        framebufferCreateInfo.renderPass = renderPass;
        framebufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
        framebufferCreateInfo.pAttachments = attachments.data();
        framebufferCreateInfo.width = extent.width;
        framebufferCreateInfo.height = extent.height;
        framebufferCreateInfo.layers = 1;

        // vk::Framebufferを作成する
        vk::Framebuffer framebuffer = logicalDevice.createFramebuffer(framebufferCreateInfo);
        if (!framebuffer)
        {
            throw std::runtime_error("フレームバッファの作成に失敗しました！");
        }

        // vk::Framebufferをvk::UniqueFramebufferに変換する
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
/// スワップチェインの作成
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainGenerator::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // スワップチェインの設定を行う
    // サーフェスの機能を取得
    m_SurfaceCapabilities = GetSurfaceCapabilities(physicalDevice, surface);


    // スワップチェインのエクステント（幅と高さ）を選択
    m_Extent = ChooseSwapExtent(m_SurfaceCapabilities);

    // スワップチェインの設定
    // 適切なサーフェスフォーマットを選択
    m_SurfaceFormat = ChooseSwapSurfaceFormat(VulkanUtility::getSurfaceFormats(physicalDevice, surface));
    // 適切なプレゼンテーションモードを選択
    m_PresentMode = ChooseSwapPresentMode(VulkanUtility::getPresentModes(physicalDevice, surface));

    // スワップチェインに必要なイメージの数を決定
    uint32_t imageCount = m_SurfaceCapabilities.minImageCount + 1;
    if (m_SurfaceCapabilities.maxImageCount > 0 && imageCount > m_SurfaceCapabilities.maxImageCount)
    {
        imageCount = m_SurfaceCapabilities.maxImageCount;
    }

    // スワップチェイン作成情報の設定
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.pNext;                       // 拡張チェーンへのポインタ
    swapchainInfo.flags;                       // 作成フラグ
    swapchainInfo.surface = surface;           // スワップチェインがターゲットとするサーフェス
    swapchainInfo.minImageCount = imageCount;  // スワップチェインに含まれる最小イメージ数
    swapchainInfo.imageFormat = m_SurfaceFormat.format;          // イメージフォーマット
    swapchainInfo.imageColorSpace = m_SurfaceFormat.colorSpace;  // カラースペース
    swapchainInfo.imageExtent = m_Extent;        // スワップチェインの幅と高さ
    swapchainInfo.imageArrayLayers = 1;        // イメージの配列層数
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // イメージの使用用途
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;          // イメージの共有モード（初期設定は排他的モード）
    swapchainInfo.queueFamilyIndexCount;                                   // キューファミリーインデックスの数
    swapchainInfo.pQueueFamilyIndices;                                     // キューファミリーインデックスの配列へのポインタ
    swapchainInfo.preTransform = m_SurfaceCapabilities.currentTransform;     // サーフェスのトランスフォーム
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // アルファ合成モード
    swapchainInfo.presentMode = m_PresentMode;   // プレゼンテーションモード
    swapchainInfo.clipped = VK_TRUE;           // クリップされるかどうか
    swapchainInfo.oldSwapchain = nullptr;      // 古いスワップチェインへのポインタ

    // グラフィックスキューファミリのインデックスを取得する
    // キューファミリのインデックスを取得
    QueueFamilyIndices indices = VulkanUtility::GetQueueFamilies(physicalDevice, surface);
    // キューファミリーインデックスの配列を作成
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentationFamily };

    // キューファミリが異なる場合は共有モードを設定
    if (indices.graphicsFamily != indices.presentationFamily)
    {
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent; // 並行モードに設定
        swapchainInfo.queueFamilyIndexCount = 2;                       // キューファミリーインデックスの数を設定
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;        // キューファミリーインデックスの配列を設定
    }


    return swapchainInfo;
}

std::vector<SwapchainImage> SwapchainGenerator::CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain)
{
    // スワップチェーンを構成するイメージのベクターを取得
    std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain);


    // Swapchainの画像を格納するベクターを作成
    std::vector<SwapchainImage> swapChainImages;
    swapChainImages.reserve(images.size());


    // スワップチェイン作成時に取得したのと同じ情報が欲しい
    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(GetSurfaceFormats(physicalDevice, surface));

    for (vk::Image image : images)
    {
        // 画像ハンドルを保存する
        SwapchainImage swapChainImage;
        swapChainImage.image = image;
        //フォーマットはchooseSwapSurfaceFormatで使用したのと同じものでなければならない
        swapChainImage.imageView = CreateImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor);

        // Swapchain画像リストに追加する
        swapChainImages.push_back(swapChainImage);
    }

    return swapChainImages;
}

vk::ImageView SwapchainGenerator::CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
    // 画像ビュー作成情報の初期化
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.image = image;                                            // View を作成するための Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image の種類 (1D, 2D, 3D, Cube など)
    imageViewCreateInfo.format = format;                                          // Image データのフォーマット
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA コンポーネントを他の RGBA 値にリマップすることができます
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource は Image の一部だけを表示するための設定です
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;                         // 表示を開始する Mipmap レベル
    imageViewCreateInfo.subresourceRange.levelCount = 1;                           // 表示する Mipmap レベルの数
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // 表示を開始する配列レベル
    imageViewCreateInfo.subresourceRange.layerCount = 1;                           // 表示する配列レベルの数

    //イメージビューを作成
    vk::ImageView imageView = logicalDevice.createImageView(imageViewCreateInfo);
    // vkCreateImageView の結果が成功ではない場合、エラーをスローします
    if (!imageView)
    {
        throw std::runtime_error("Failed to create an Image View!");
    }

    // 作成した Image View のハンドルを返します
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

    return vk::PresentModeKHR::eFifo; // ファイフォキューを使用する（必ずサポートされている）
}

vk::Extent2D SwapchainGenerator::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
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

std::vector<vk::SurfaceFormatKHR> SwapchainGenerator::GetSurfaceFormats(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return physicalDevice.getSurfaceFormatsKHR(surface);
}
