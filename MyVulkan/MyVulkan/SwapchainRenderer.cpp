#include "SwapChainRenderer.h"


SwapchainRenderer::SwapchainRenderer(VulkanInitializer& initializer) : 
    m_pAllocator(nullptr),
    m_LogicalDevice(VK_NULL_HANDLE),
    m_SwapchainInfo(),
    m_Swapchain(VK_NULL_HANDLE),
    m_SwapChainImages(),
    m_Semaphore(VK_NULL_HANDLE),
    m_Fence(VK_NULL_HANDLE),
    m_PresentationCommand()
{    
    initializer.GetPDeviceExtension()->UseSwapchain();
}

SwapchainRenderer::~SwapchainRenderer()
{

}

void SwapchainRenderer::Create(VmaAllocator* allocator, vk::SurfaceKHR surface)
{
    m_pAllocator = allocator;

    // VMAに紐づけられているオブジェクトの情報を取得
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo);
    m_LogicalDevice = allocatorInfo.device;
	vk::PhysicalDevice physicalDevice = allocatorInfo.physicalDevice;

    // セマフォの作成
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphoreInfo.flags;
    m_Semaphore = m_LogicalDevice.createSemaphore(semaphoreInfo);

    // フェンスの作成
    vk::FenceCreateInfo fenceCreateInfo;
    fenceCreateInfo.pNext;
    fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled; // 作成時からシグナル状態
    m_Fence = m_LogicalDevice.createFence(fenceCreateInfo);

    // スワップチェインの作成
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = m_LogicalDevice.createSwapchainKHR(m_SwapchainInfo);

    // スワップチェイン用のフレームバッファの作成
    m_SwapChainImages.Create(allocator, m_Swapchain, m_SwapchainInfo);

    //コマンドバッファの作成
    m_PresentationCommand.Initialize(m_LogicalDevice, physicalDevice, m_Swapchain);
}

void SwapchainRenderer::Destroy()
{
    //スワップチェーンの解放
    m_LogicalDevice.destroySwapchainKHR(m_Swapchain);
}

vk::SwapchainKHR SwapchainRenderer::GetSwapchain()
{
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainRenderer::GetSwapchainInfo()
{
    return m_SwapchainInfo;
}

SwapChainImage SwapchainRenderer::GetImages()
{
    return m_SwapChainImages;
}

vk::Extent2D SwapchainRenderer::GetExtent()
{
    return m_SwapchainInfo.imageExtent;
}

std::vector<RenderingImageSet> SwapchainRenderer::GetImageSets()
{
    return m_SwapChainImages.GetImageSets();
}

vk::Format SwapchainRenderer::GetColorFormat()
{
    return m_SwapChainImages.GetColorFormat();
}

vk::Format SwapchainRenderer::GetDepthFormat()
{
    return m_SwapChainImages.GetDepthFormat();
}

vk::Semaphore SwapchainRenderer::GetSemaphore()
{
    return m_Semaphore;
}

vk::Fence SwapchainRenderer::GetFence()
{
    return m_Fence;
}

uint32_t SwapchainRenderer::AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore)
{
    return m_PresentationCommand.AcquireSwapchainNextImage(imageAvailableSemaphore);
}

void SwapchainRenderer::UpdateFrame(vk::Semaphore imageAvailableSemaphore)
{
    //vk::ResultValue acquire = m_LogicalDevice.acquireNextImageKHR(
    //    m_Swapchain, std::numeric_limits<uint64_t>::max(), m_Semaphore, m_Fence);
    //if (acquire.result != vk::Result::eSuccess) std::cerr << "次フレームの取得に失敗しました。" << std::endl;

    /*m_PresentationCommand.PresentFrame(m_Swapchain, acquire.value);*/
    m_PresentationCommand.RunningCommand(imageAvailableSemaphore);
}

/// <summary>
/// スワップチェーンインフォの作成
/// </summary>
vk::SwapchainCreateInfoKHR SwapchainRenderer::CreateSwapchainInfo(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
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
    QueueFamilySelector queueFamilySelector;
    queueFamilySelector.Initialize(physicalDevice);
    if (queueFamilySelector.GetGraphicIndex() != queueFamilySelector.GetPresentationIndex(surface))
    {
        std::vector<uint32_t> queueFamilyIndices = { queueFamilySelector.GetGraphicIndex(), queueFamilySelector.GetPresentationIndex(surface) };
        swapchainInfo.imageSharingMode = vk::SharingMode::eConcurrent;      // 並行モードに設定
        swapchainInfo.queueFamilyIndexCount = (uint32_t)queueFamilyIndices.size();    // キューファミリーインデックスの数を設定
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices.data();      // キューファミリーインデックスの配列を設定
    }

    return swapchainInfo;
}

vk::SurfaceFormatKHR SwapchainRenderer::SelectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
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

vk::PresentModeKHR SwapchainRenderer::SelectPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
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
