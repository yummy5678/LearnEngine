#include "SwapChainUtility.h"


SwapchainGenerator::SwapchainGenerator(CDeviceExtensionManager& deviceExtensionManager)
{
    m_ClassName = "SwapchainGenerator";

    deviceExtensionManager.UseSwapchain();
}

SwapchainGenerator::~SwapchainGenerator()
{

}

void SwapchainGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    std::cout  << m_ClassName << "を作成" << std::endl;
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);

    m_Swapchain = logicalDevice.createSwapchainKHR(m_SwapchainInfo);

    //m_Images = CreateSwapChainImages(logicalDevice, physicalDevice, surface, m_Swapchain);
}

void SwapchainGenerator::Destroy(vk::Device logicalDevice)
{
    //中身が作成されていないなら解放処理も行わない
    if (m_bCreated == false) return;
    m_bCreated = false;
    
    //スワップチェーンの解放
    m_LogicalDevice.destroySwapchainKHR(m_Swapchain);
}

vk::SwapchainKHR SwapchainGenerator::GetSwapchain()
{
    CheckCreated();
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainGenerator::GetSwapchainInfo()
{
    CheckCreated();
    return m_SwapchainInfo;
}

uint32_t SwapchainGenerator::GetImageCount()
{
    return m_SwapchainInfo.minImageCount;
}

//std::vector<SwapchainImage> SwapchainGenerator::GetSwapChainImages()
//{
//    CheckCreated();
//    return m_Images;
//}


/// <summary>
/// スワップチェーンインフォの作成
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainGenerator::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    // スワップチェーンの設定を行う
    // サーフェスの機能を取得
    auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
    std::vector<vk::PresentModeKHR> surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);

    // サーフェスから取得した情報の中から最適なものを選ぶ
    vk::SurfaceFormatKHR format = SelectSurfaceFormat(surfaceFormats);
    vk::PresentModeKHR presentMode = SelectPresentMode(surfacePresentModes);

    // スワップチェーンに必要なイメージの数を決定
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
    if (imageCount > surfaceCapabilities.maxImageCount)
    {
        imageCount = surfaceCapabilities.maxImageCount;
    }

    // スワップチェーン作成情報の設定
    vk::SwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.pNext;                       // 拡張チェーンへのポインタ
    swapchainInfo.flags;                       // 作成フラグ
    swapchainInfo.surface = surface;           // スワップチェーンがターゲットとするサーフェス
    swapchainInfo.minImageCount = imageCount;  // スワップチェーンに含まれる最小画像数
    swapchainInfo.imageFormat = format.format;          // 画像フォーマット
    swapchainInfo.imageColorSpace = format.colorSpace;  // カラースペース
    swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;         // 画像の幅と高さ
    swapchainInfo.imageArrayLayers = 1;                                    // 画像の配列層数(平面画像なら"1"にする)
    swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;   // 画像の使用用途
    swapchainInfo.imageSharingMode = vk::SharingMode::eExclusive;          // 画像の共有モード（初期設定は排他的モード）
    swapchainInfo.queueFamilyIndexCount;                                   // キューファミリーインデックスの数
    swapchainInfo.pQueueFamilyIndices;                                     // キューファミリーインデックスの配列へのポインタ
    swapchainInfo.preTransform = surfaceCapabilities.currentTransform;     // サーフェスのトランスフォーム(変形情報)
    swapchainInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque; // アルファ合成モード
    swapchainInfo.presentMode = presentMode;        // プレゼンテーションモード
    swapchainInfo.clipped = VK_TRUE;                // クリップされるかどうか
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;    // 古いスワップチェーンへのポインタ

    //////////////////////////////
    // キューの共有設定
    //////////////////////////////
    // キューファミリが異なる場合は共有モードを設定
    QueueFamilySelector queueFamilySelector(physicalDevice);
    if (queueFamilySelector.GetGraphicIndex() != queueFamilySelector.GetPresentationIndex(surface))
    {
        std::vector<uint32_t> queueFamilyIndices = { queueFamilySelector.GetGraphicIndex(), queueFamilySelector.GetPresentationIndex(surface) };
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;      // 並行モードに設定
        swapchainInfo.queueFamilyIndexCount = queueFamilyIndices.size();    // キューファミリーインデックスの数を設定
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();      // キューファミリーインデックスの配列を設定
    }

    return swapchainInfo;
}

//std::vector<SwapchainImage> SwapchainGenerator::CreateSwapChainImages(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, vk::SwapchainKHR swapchain)
//{
//    // スワップチェーンを構成するイメージのベクターを取得
//    std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(swapchain);
//
//
//    // Swapchainの画像を格納するベクターを作成
//    std::vector<SwapchainImage> swapChainImages;
//    swapChainImages.reserve(images.size());
//
//
//    // スワップチェーン作成時に取得したのと同じ情報が欲しい
//    vk::SurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(GetSurfaceFormats(physicalDevice, surface));
//
//    for (vk::Image image : images)
//    {
//        // 画像ハンドルを保存する
//        SwapchainImage swapChainImage;
//        swapChainImage.image = image;
//        //フォーマットはchooseSwapSurfaceFormatで使用したのと同じものでなければならない
//        swapChainImage.imageView = CreateImageView(logicalDevice, image, surfaceFormat.format, vk::ImageAspectFlagBits::eColor);
//
//        // Swapchain画像リストに追加する
//        swapChainImages.push_back(swapChainImage);
//    }
//
//    return swapChainImages;
//}

//vk::ImageView SwapchainGenerator::CreateImageView(vk::Device logicalDevice, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
//{
//    // 画像ビュー作成情報の初期化
//    vk::ImageViewCreateInfo imageViewCreateInfo;
//    imageViewCreateInfo.image = image;                                            // View を作成するための Image
//    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;                         // Image の種類 (1D, 2D, 3D, Cube など)
//    imageViewCreateInfo.format = format;                                          // Image データのフォーマット
//    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;             // RGBA コンポーネントを他の RGBA 値にリマップすることができます
//    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
//    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
//    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
//
//    // Subresource は Image の一部だけを表示するための設定です
//    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlags;                 // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
//    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;                         // 表示を開始する Mipmap レベル
//    imageViewCreateInfo.subresourceRange.levelCount = 1;                           // 表示する Mipmap レベルの数
//    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;                       // 表示を開始する配列レベル
//    imageViewCreateInfo.subresourceRange.layerCount = 1;                           // 表示する配列レベルの数
//
//    //イメージビューを作成
//    vk::ImageView imageView = logicalDevice.createImageView(imageViewCreateInfo);
//    // vkCreateImageView の結果が成功ではない場合、エラーをスローします
//    if (!imageView)
//    {
//        throw std::runtime_error("Failed to create an Image View!");
//    }
//
//    // 作成した Image View のハンドルを返します
//    return imageView;
//
//}

vk::SurfaceFormatKHR SwapchainGenerator::SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
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

vk::PresentModeKHR SwapchainGenerator::SelectPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        // サーフェスの使用可能なモードの中に
        // メールボックスモードがあればそれにする
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {

            return availablePresentMode;
        }
    }
    // 無ければFIFOキューを使用する(必ずサポートされている)
    return vk::PresentModeKHR::eFifo; 
}
