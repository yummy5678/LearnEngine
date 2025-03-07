#include "SwapChainRenderer.h"


SwapchainRenderer::SwapchainRenderer(VulkanInitializer& initializer) : 
    m_pAllocator(nullptr),
    m_LogicalDevice(VK_NULL_HANDLE),
    m_Surface(VK_NULL_HANDLE),
    m_SwapchainInfo(),
    m_Swapchain(VK_NULL_HANDLE),
    m_SwapChainImages(),
    m_QueueFamily(),
    m_ImageAvailableSemaphores(),
    m_PresentCommandPool(VK_NULL_HANDLE),
    m_PresentCommandBuffers(),
    //m_Fences(),
    //m_PresentationCommand(), 
    m_ImageIndex(0)
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
    m_QueueFamily.Initialize(physicalDevice);
    m_Surface = surface;


    
    // スワップチェインの作成
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = m_LogicalDevice.createSwapchainKHR(m_SwapchainInfo);

    // スワップチェイン用のフレームバッファの作成
    m_SwapChainImages.Create(allocator, m_Swapchain, m_SwapchainInfo);

    // スワップチェインの描画コマンドを作成
    CreatePresentationCommands();

    //// フェンスを画像の数だけ作成
    //vk::FenceCreateInfo fenceInfo;
    //fenceInfo.pNext;
    //fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    //m_Fences.resize(m_SwapchainInfo.minImageCount);
    //for (uint32_t i = 0; i < m_SwapchainInfo.minImageCount; i++)
    //{
    //    m_Fences[i] = m_LogicalDevice.createFence(fenceInfo);
    //}



    // セマフォを画像の数だけ作成
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphoreInfo.flags;
    m_ImageAvailableSemaphores.resize(m_SwapchainInfo.minImageCount);
    for (uint32_t i = 0; i < m_SwapchainInfo.minImageCount; i++)
    {
        m_ImageAvailableSemaphores[i] = m_LogicalDevice.createSemaphore(semaphoreInfo);
    }

    // イメージインデックスを初期化
    m_ImageIndex = 0;
    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[m_ImageIndex]);
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

vk::Semaphore SwapchainRenderer::GetImageAvailableSemaphore()
{
    return m_ImageAvailableSemaphores[m_ImageIndex];
}

//vk::Fence SwapchainRenderer::GetFence()
//{
//    return m_Fences[m_ImageIndex];
//}

const uint32_t SwapchainRenderer::GetUseImageIndex()
{
    return m_ImageIndex;
}

void SwapchainRenderer::AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore)
{
    //// 次のインデックス
    //m_LogicalDevice.waitForFences(
    //    { m_Fences[m_ImageIndex] },							// 利用するフェンス達
    //    VK_TRUE,								// フェンスが全てシグナル状態になるまで待つ
    //    UINT64_MAX);							// 最大待機時間

    //m_LogicalDevice.resetFences(m_Fences[m_ImageIndex]);	// フェンスを非シグナル状態にする


    // スワップチェーンから次に描画するイメージ（フレームバッファのようなもの）のインデックスを取得します。
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
        m_Swapchain,                            // スワップチェーン
        std::numeric_limits<uint64_t>::max(),   // タイムアウトの設定(ここでは無限待機)
        imageAvailableSemaphore,                // イメージが使用可能になるのを通知するセマフォ
        nullptr,                                // フェンス(ここでは使用しないのでnullptr)
        &m_ImageIndex                           // イメージのインデックスが格納される
    );

    // イメージ取得に失敗した場合、エラーメッセージを投げる
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンからイメージを取得できませんでした！");
    }

}

void SwapchainRenderer::CreatePresentationCommands()
{


#pragma region コマンドプールの作成
    // コマンドプールの作成に必要な情報を設定する
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = m_QueueFamily.GetPresentationIndex(m_Surface);

    // グラフィックスキューファミリー用のコマンドプールを作成する
    m_PresentCommandPool = m_LogicalDevice.createCommandPool(poolInfo);
#pragma endregion

#pragma region コマンドの作成
    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = m_PresentCommandPool;                  // コマンドバッファを割り当てるコマンドプール
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    allocateInfo.commandBufferCount = m_SwapchainInfo.minImageCount;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    m_PresentCommandBuffers = m_LogicalDevice.allocateCommandBuffers(allocateInfo); //配列で情報をやり取りする
#pragma endregion
}

void SwapchainRenderer::Presentation(vk::SurfaceKHR surface, vk::Semaphore imageAvailableSemaphore)
{
    //vk::SemaphoreCreateInfo semaphoreInfo;
    //semaphoreInfo.pNext;
    //m_Semaphores.push_back(m_LogicalDevice.createSemaphore(semaphoreInfo));
    //m_SwapChainIndex = AcquiredSwapChainIndex;
    //m_SwapChainIndex = 0;
    //m_SwapChainIndex = 0;

    vk::Result result;

    vk::PresentInfoKHR presentInfo;
    presentInfo.pNext;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &m_ImageIndex;
    presentInfo.pResults = &result;
    presentInfo.pWaitSemaphores = &imageAvailableSemaphore;
    presentInfo.waitSemaphoreCount = 1;

    // 使用するキュー(グラフィックキューやプレゼントキューなど)のインデックスを取得
    auto presentQueue = m_LogicalDevice.getQueue(m_QueueFamily.GetPresentationIndex(surface), 0);
    presentQueue.presentKHR(presentInfo);
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンの画像の表示に失敗しました！");
    }
}

void SwapchainRenderer::UpdateFrame()
{
    //vk::ResultValue acquire = m_LogicalDevice.acquireNextImageKHR(
    //    m_Swapchain, std::numeric_limits<uint64_t>::max(), m_ImageAvailableSemaphores, m_Fences);
    //if (acquire.result != vk::Result::eSuccess) std::cerr << "次フレームの取得に失敗しました。" << std::endl;

    /*m_PresentationCommand.PresentFrame(m_Swapchain, acquire.value);*/
    uint32_t index = m_ImageIndex;
    //m_PresentationCommand.RunningCommand(index, m_Surface, m_ImageAvailableSemaphores[index]);
    Presentation(m_Surface, m_ImageAvailableSemaphores[index]);

    // 次のインデックスに切り替え
    index = (index + 1) % m_SwapchainInfo.minImageCount;



    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[index]);

    printf("%d :番号を更新\n", index);
    printf("%d\n", m_ImageAvailableSemaphores[index]);
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
