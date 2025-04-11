#include "SwapChainRenderer.h"



SwapchainRenderer::SwapchainRenderer(VulkanInitializer& initializer) : 
    m_pAllocator(nullptr),
    m_AllocatorInfo(),
    m_Surface(VK_NULL_HANDLE),
    m_SwapchainInfo(),
    m_Swapchain(VK_NULL_HANDLE),
    m_ColorFormat(vk::Format::eUndefined),
    m_DepthFormat(vk::Format::eUndefined),
    m_QueueFamily(),
    m_ImageAvailableSemaphores(),
    m_PresentCommandPool(VK_NULL_HANDLE),
    m_PresentCommandBuffers(),
    //m_Fences(),
    //m_PresentationCommand(), 
    m_ImageIndex(0)
{    
    initializer.GetPDeviceExtension()->UseSwapchain();

    // アロケータの内容の構造体を初期化しておく
    m_AllocatorInfo.instance = VK_NULL_HANDLE;
    m_AllocatorInfo.physicalDevice = VK_NULL_HANDLE;
    m_AllocatorInfo.device = VK_NULL_HANDLE;
}

SwapchainRenderer::~SwapchainRenderer()
{

}

void SwapchainRenderer::Create(VmaAllocator* allocator, vk::SurfaceKHR surface)
{
    m_pAllocator = allocator;

    // VMAに紐づけられているオブジェクトの情報を取得
    vmaGetAllocatorInfo(*allocator, &m_AllocatorInfo);
	vk::PhysicalDevice physicalDevice = m_AllocatorInfo.physicalDevice;
	vk::Device logicalDevice = m_AllocatorInfo.device;
    m_QueueFamily.Initialize(physicalDevice);
    m_Surface = surface;


    
    // スワップチェインの作成
    m_SwapchainInfo = CreateSwapchainInfo(physicalDevice, surface);
    m_Swapchain = logicalDevice.createSwapchainKHR(m_SwapchainInfo);

    // スワップチェイン用のフレームバッファの作成
    CreateSwapchainImage();

    // スワップチェインの描画コマンドを作成
    CreatePresentationCommands();

    // セマフォを画像の数だけ作成
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphoreInfo.flags;
    m_ImageAvailableSemaphores.resize(m_SwapchainInfo.minImageCount);
    for (uint32_t i = 0; i < m_SwapchainInfo.minImageCount; i++)
    {
        m_ImageAvailableSemaphores[i] = logicalDevice.createSemaphore(semaphoreInfo);
    }

    // イメージインデックスを初期化
    m_ImageIndex = 0;
    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[m_ImageIndex]);
}

void SwapchainRenderer::Cleanup()
{
    // NULLチェック
    vk::Device logicalDevice = m_AllocatorInfo.device;
    vk::Instance instance = m_AllocatorInfo.instance;
    if (logicalDevice == VK_NULL_HANDLE ||
        instance == VK_NULL_HANDLE) return;

    // セマフォの解放
    for (auto& semaphore : m_ImageAvailableSemaphores)
    {
        logicalDevice.destroySemaphore(semaphore);
    }

    // 
    m_QueueFamily.Cleanup();
    
    // コマンドの解放
    logicalDevice.destroyCommandPool(m_PresentCommandPool);
    m_PresentCommandBuffers.clear();

    // フレームバッファの解放
    for (uint32_t i = 0;i < m_DepthImageAllocation.size();i++)
    {
        vmaDestroyImage(*m_pAllocator, m_ImageSets[i].depth.buffer, m_DepthImageAllocation[i]);
    }
    m_DepthFormat = vk::Format::eUndefined;
    m_ColorFormat = vk::Format::eUndefined;
    m_ImageSets.clear();

    // サーフェスの解放
    instance.destroySurfaceKHR(m_Surface);

    //スワップチェーンの解放
    logicalDevice.destroySwapchainKHR(m_Swapchain);

    // フレームバッファのカウントを初期化
    m_ImageIndex = 0;

    // アロケータの内容の構造体を初期化
    m_AllocatorInfo.instance = VK_NULL_HANDLE;
    m_AllocatorInfo.physicalDevice = VK_NULL_HANDLE;
    m_AllocatorInfo.device = VK_NULL_HANDLE;

}

vk::SwapchainKHR SwapchainRenderer::GetSwapchain()
{
    return m_Swapchain;
}

vk::SwapchainCreateInfoKHR SwapchainRenderer::GetSwapchainInfo()
{
    return m_SwapchainInfo;
}

vk::Extent2D SwapchainRenderer::GetExtent()
{
    return m_SwapchainInfo.imageExtent;
}

uint32_t SwapchainRenderer::GetFrameCount()
{
    return m_SwapchainInfo.minImageCount;
}

RenderingImageSet SwapchainRenderer::GetRenderingImageSet()
{
    return m_ImageSets[m_ImageIndex];
}

vk::Format SwapchainRenderer::GetColorFormat()
{
    return m_ColorFormat;
}

vk::Format SwapchainRenderer::GetDepthFormat()
{
    return m_DepthFormat;
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
    vk::Device logicalDevice = m_AllocatorInfo.device;

    // スワップチェーンから次に描画するイメージ（フレームバッファのようなもの）のインデックスを取得します。
    vk::Result result = logicalDevice.acquireNextImageKHR(
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

vk::ImageCreateInfo SwapchainRenderer::CreateImageInfo(vk::Extent2D extent, vk::Format fomat, vk::ImageUsageFlags usage)
{
    vk::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.pNext;
    imageCreateInfo.flags;
    imageCreateInfo.imageType = vk::ImageType::e2D;
    imageCreateInfo.format = fomat;                                 // 画像のフォーマット
    imageCreateInfo.extent.width = extent.width;                    // 画像の幅
    imageCreateInfo.extent.height = extent.height;                  // 画像の高さ
    imageCreateInfo.extent.depth = 1;                               // 画像の奥行き
    imageCreateInfo.mipLevels = 1;                                  // ミップマップレベル
    imageCreateInfo.arrayLayers = 1;                                // レイヤー数
    imageCreateInfo.samples = vk::SampleCountFlagBits::e1;          // サンプル数
    imageCreateInfo.tiling = vk::ImageTiling::eOptimal;             // タイリング方式
    imageCreateInfo.usage = usage;                                  // 使用方法
    imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;      // 共有モード
    imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;    // 初期レイアウト
    // imageCreateInfo.queueFamilyIndexCount = 0;                     // キューファミリーインデックスの数
    // imageCreateInfo.pQueueFamilyIndices;                           // キューファミリーインデックスの配列
    return imageCreateInfo;

}

vk::ImageViewCreateInfo SwapchainRenderer::CreateImageViewInfo(vk::Image image, vk::Format fomat, vk::ImageAspectFlags aspectFlag)
{
    // 画像ビュー作成情報の初期化
    vk::ImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.pNext;
    imageViewCreateInfo.flags;
    imageViewCreateInfo.image = image;                                  // View を作成するための Image
    imageViewCreateInfo.viewType = vk::ImageViewType::e2D;              // Image の種類 (1D, 2D, 3D, Cube など)
    imageViewCreateInfo.format = fomat;                                 // Image データのフォーマット(imageと同じものでなければならない)
    imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity; // RGBA コンポーネントを他の RGBA 値にリマップすることができます
    imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
    imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

    // Subresource は Image の一部だけを表示するための設定です
    //VK_IMAGE_ASPECT_COLOR_BIT
    imageViewCreateInfo.subresourceRange.aspectMask = aspectFlag;   // Image のどの面を表示するか (例: COLOR_BIT は色を表示するため)
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;          // 表示を開始する Mipmap レベル
    imageViewCreateInfo.subresourceRange.levelCount = 1;            // 表示する Mipmap レベルの数
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;        // 表示を開始する配列レベル
    imageViewCreateInfo.subresourceRange.layerCount = 1;            // 表示する配列レベルの数

    return imageViewCreateInfo;

}

void SwapchainRenderer::CreateDepthImage(vk::Image& setImage, VmaAllocation& allocation, VmaAllocator* allocator, vk::ImageCreateInfo createInfo)
{
    VmaAllocationCreateInfo allocInfo;
    allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // 自動で最適なメモリを選択
    allocInfo.pool = VK_NULL_HANDLE;
    allocInfo.memoryTypeBits = 0;
    allocInfo.preferredFlags = 0;
    allocInfo.priority = 1;
    allocInfo.requiredFlags = 0;
    allocInfo.pUserData = nullptr;
    VkImage image = nullptr;
    auto imageInfo = (VkImageCreateInfo)createInfo;


    VkResult result = vmaCreateImage(*allocator, &imageInfo, &allocInfo, &image, &allocation, nullptr);

    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("VMAによるイメージの作成に失敗しました!");
    }

    setImage = vk::Image(image);  // VkImageをvk::Imageにキャスト

}

void SwapchainRenderer::CreatePresentationCommands()
{
    vk::Device logicalDevice = m_AllocatorInfo.device;

#pragma region コマンドプールの作成
    // コマンドプールの作成に必要な情報を設定する
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = m_QueueFamily.GetPresentationIndex(m_Surface);

    // グラフィックスキューファミリー用のコマンドプールを作成する
    m_PresentCommandPool = logicalDevice.createCommandPool(poolInfo);
#pragma endregion

#pragma region コマンドの作成
    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = m_PresentCommandPool;                  // コマンドバッファを割り当てるコマンドプール
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    allocateInfo.commandBufferCount = m_SwapchainInfo.minImageCount;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    m_PresentCommandBuffers = logicalDevice.allocateCommandBuffers(allocateInfo); //配列で情報をやり取りする
#pragma endregion
}

void SwapchainRenderer::CreateSwapchainImage()
{
    if (m_pAllocator == nullptr) throw std::runtime_error("スワップチェイン画像の作成時にVMAがNULLでした!");

    uint32_t        imageCount  = m_SwapchainInfo.minImageCount;    // スワップチェーンのフレーム数を取得
    vk::Extent2D    imageExtent = m_SwapchainInfo.imageExtent;      // 画像サイズを取得



    // 深度イメージはスワップチェーンオブジェクトのほうでサポートされていないので自前で用意
    m_DepthImageAllocation.resize(imageCount);  

    // カラーイメージだけはスワップチェイン作成時に一緒に作成されるので、それを使う
    vk::Device logicalDevice = m_AllocatorInfo.device;
    std::vector<vk::Image> colorImage = logicalDevice.getSwapchainImagesKHR(m_Swapchain);

    // 管理する画像の数をセット
    m_ImageSets.resize(imageCount);
    for (uint32_t i = 0; i < imageCount; i++)
    {
        // カラーイメージの作成
        m_ImageSets[i].color.buffer = colorImage[i];
        //イメージビュー、画像を扱う際の情報を設定
        m_ColorFormat = m_SwapchainInfo.imageFormat;
        vk::ImageViewCreateInfo colorImageViewInfo = CreateImageViewInfo(m_ImageSets[i].color.buffer, m_ColorFormat, vk::ImageAspectFlagBits::eColor);
        m_ImageSets[i].color.view = logicalDevice.createImageView(colorImageViewInfo);


        // 深度イメージの作成
        m_DepthFormat = vk::Format::eD24UnormS8Uint;    //深度イメージのフォーマット
        vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
        auto imageInfo = CreateImageInfo(imageExtent, m_DepthFormat, usage);
        CreateDepthImage(m_ImageSets[i].depth.buffer, m_DepthImageAllocation[i], m_pAllocator, imageInfo);
        //イメージビュー、画像を扱う際の情報を設定
        vk::ImageViewCreateInfo depthImageViewInfo = CreateImageViewInfo(m_ImageSets[i].depth.buffer, m_DepthFormat, vk::ImageAspectFlagBits::eDepth);
        m_ImageSets[i].depth.view = logicalDevice.createImageView(depthImageViewInfo);
    }
}

void SwapchainRenderer::Presentation(vk::SurfaceKHR surface, vk::Semaphore imageAvailableSemaphore)
{
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
    vk::Device logicalDevice = m_AllocatorInfo.device;
    auto presentQueue = logicalDevice.getQueue(m_QueueFamily.GetPresentationIndex(surface), 0);
    presentQueue.presentKHR(presentInfo);
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンの画像の表示に失敗しました！");
    }
}

void SwapchainRenderer::UpdateFrame()
{
    uint32_t index = m_ImageIndex;
    //m_PresentationCommand.RunningCommand(index, m_Surface, m_ImageAvailableSemaphores[index]);
    Presentation(m_Surface, m_ImageAvailableSemaphores[index]);

    // 次のインデックスに切り替え
    index = (index + 1) % m_SwapchainInfo.minImageCount;

    AcquireSwapchainNextImage(m_ImageAvailableSemaphores[index]);
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
