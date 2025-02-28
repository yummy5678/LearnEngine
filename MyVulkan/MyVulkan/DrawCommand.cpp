#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_LogicalDevice(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Swapchain(VK_NULL_HANDLE),
    m_CurrentIndex(0),
    m_NextIndex(0),
    m_ImageSet(),
    m_CommandBuffers(),
    m_CommandPool(VK_NULL_HANDLE),
    m_SignalSemaphores(),
    m_Fences(),
    m_QueueSelector()
{

}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, RenderingImageSet* imageSet)
{
    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_ImageSet = { imageSet };
    m_QueueSelector.Initialize(m_PhysicalDevice);

    // 使用するフレームの枚数
    size_t imageSize = m_ImageSet.size();

    // フレームの数だけセマフォとフェンスを作成
    m_SignalSemaphores.resize(imageSize);
    m_ImageAvailableSemaphores.resize(imageSize);
    m_Fences.resize(imageSize);

    //フェンスの作成
    CreateFence(m_Fences[0]);

    //コマンドプール(コマンドを置く領域)を作成
    CreateCommandPool();

    //コマンドプールにコマンドバッファを割り当て
    CreateCommandBuffers(m_ImageSet.size(), m_CommandPool);

}

void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, SwapchainRenderer* swapchainRenderer)
{
    m_Swapchain = swapchainRenderer->GetSwapchain();

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_ImageSet = swapchainRenderer->GetImageSets();
    m_QueueSelector.Initialize(m_PhysicalDevice);

    // 使用するフレームの枚数
    size_t imageSize = m_ImageSet.size();

    // フレームの数だけセマフォとフェンスを作成
    m_SignalSemaphores.resize(imageSize);
    m_ImageAvailableSemaphores.resize(imageSize);
    m_Fences.resize(imageSize);
    for (size_t i = 0; i < imageSize; i++)
    {
        // セマフォの作成
        CreateSemaphore(m_SignalSemaphores[i]);
        CreateSemaphore(m_ImageAvailableSemaphores[i]);

        //フェンスの作成
        CreateFence(m_Fences[i]);
    }

    //コマンドプール(コマンドを置く領域)を作成
    CreateCommandPool();

    //コマンドプールにコマンドバッファを割り当て
    CreateCommandBuffers(imageSize, m_CommandPool);
}

void DrawCommand::Destroy()
{
    //コマンドプールの解放
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        (uint32_t)m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //コマンドプールの破棄
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

vk::CommandBuffer DrawCommand::GetBuffer()
{
    if (m_CommandBuffers.empty() == true) 
        throw std::runtime_error("コマンドを取り出そうとしましたが、それはまだ発行されていません");

    return m_CommandBuffers[m_CurrentIndex];
}

void DrawCommand::BeginRendering(vk::Rect2D renderArea)
{
    if (m_ImageSet.empty() == true) return; // 描画する為のイメージがセットされていなければ何もしない

    m_NextIndex = AcquireSwapchainNextImage();

    // カラーバッファアタッチメント
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = m_ImageSet[m_NextIndex].color.imageView;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));

    // Depthバッファアタッチメント（3Dオブジェクト用に使用）
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = m_ImageSet[m_NextIndex].depth.imageView;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags;
    beginInfo.pNext;
    beginInfo.pInheritanceInfo; // セカンダリコマンドバッファ(使用しない)


    // ダイナミックレンダリングの設定
    vk::RenderingInfo renderingInfo;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;

    // Dynamic Renderingを開始
    auto commandBuffer = m_CommandBuffers[m_NextIndex];
    commandBuffer.begin(beginInfo);
    commandBuffer.beginRendering(renderingInfo);



}

void DrawCommand::EndRendering()
{
    m_CommandBuffers[m_NextIndex].endRendering();
    m_CommandBuffers[m_NextIndex].end();

    std::vector<vk::PipelineStageFlags> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    vk::SubmitInfo submitInfo = CreateSubmitInfo(&waitStages);

    // キューにコマンドを送信
    vk::Queue queue = m_LogicalDevice.getQueue(m_QueueSelector.GetGraphicIndex(), 0);
    queue.submit(submitInfo, m_Fences[m_CurrentIndex]);

    // インデックスのカウントを進める
    m_CurrentIndex = (m_CurrentIndex + 1) % m_ImageSet.size();   //  添え字がコマンド数の範囲内に収まるよう調整
}

vk::Semaphore DrawCommand::GetImageAvableSemaphore()
{
    return m_ImageAvailableSemaphores[m_CurrentIndex];
}

vk::Fence DrawCommand::GetFence()
{
    return m_Fences[m_CurrentIndex];
}

uint32_t DrawCommand::GetCurrentIndex()
{
    return m_CurrentIndex;
}


void DrawCommand::CreateSemaphore(vk::Semaphore& semaphore)
{
    vk::SemaphoreCreateInfo semaphoreInfo;
    semaphoreInfo.pNext;
    semaphore = m_LogicalDevice.createSemaphore(semaphoreInfo);
}

void DrawCommand::CreateFence(vk::Fence& fence)
{
    vk::FenceCreateInfo fenceInfo;
    fenceInfo.pNext;
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    fence = m_LogicalDevice.createFence(fenceInfo);
}

void DrawCommand::CreateCommandPool()
{
    if(m_LogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドプールを作成しようとしましたが、論理デバイスがNULLです");

    if (m_PhysicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドプールを作成しようとしましたが、物理デバイスがNULLです");

    // コマンドプールの作成に必要な情報を設定する
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;	// コマンドバッファのリセットを許可する場合はフラグを追加する
    poolInfo.queueFamilyIndex = m_QueueSelector.GetGraphicIndex();	            // このコマンドプールが使用するキューファミリー

   m_CommandPool = m_LogicalDevice.createCommandPool(poolInfo);
}

void DrawCommand::CreateCommandBuffers(uint32_t commandSize, vk::CommandPool commandPool)
{
    if (m_LogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドバッファを作成しようとしましたが、論理デバイスがNULLです");

    if (m_CommandPool == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドバッファを作成しようとしましたが、コマンドプールがNULLです");


    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // コマンドバッファを割り当てるコマンドプール
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    cbAllocInfo.commandBufferCount = commandSize;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(cbAllocInfo); //配列で情報をやり取りする



}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(
    std::vector<vk::PipelineStageFlags>* waitStages)
{
    vk::SubmitInfo submitInfo;
    submitInfo.pNext;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers[m_NextIndex];

    // 画像の枚数が複数ある場合は同期処理を行う
    if (m_Swapchain != VK_NULL_HANDLE)
    {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_SignalSemaphores[m_CurrentIndex];
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[m_CurrentIndex];
        submitInfo.pWaitDstStageMask = waitStages->data();   
    }

    return submitInfo;
}

uint32_t DrawCommand::AcquireSwapchainNextImage()
{
    if (m_Swapchain == VK_NULL_HANDLE) return 0;

    // スワップチェーンから次に描画するイメージ（フレームバッファのようなもの）のインデックスを取得します。
    uint32_t imageIndex;
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
        m_Swapchain,                                // スワップチェーン
        std::numeric_limits<uint64_t>::max(),       // タイムアウトの設定(ここでは無限待機)
        m_ImageAvailableSemaphores[m_CurrentIndex], // イメージが使用可能になるのを通知するセマフォ
        nullptr,                                    // フェンス(ここでは使用しないのでnullptr)
        &imageIndex                                 // イメージのインデックスが格納される
    );

    // イメージ取得に失敗した場合、エラーメッセージを投げる
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンからイメージを取得できませんでした！");
    }

    return imageIndex;
}

void DrawCommand::ImageMemoryBarrier(vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{

    vk::ImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.layerCount = 1;
    subresourceRange.levelCount = 1;
    
    // イメージバリアの設定
    vk::ImageMemoryBarrier imageMemoryBarrier;
    imageMemoryBarrier.dstAccessMask = vk::AccessFlagBits::eNoneKHR;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = m_ImageSet[m_CurrentIndex].color.image;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // パイプラインバリア
    m_CommandBuffers[m_CurrentIndex].pipelineBarrier
    (
        vk::PipelineStageFlagBits::eColorAttachmentOutput, // sourceStage (レンダリングの最後)
        vk::PipelineStageFlagBits::eBottomOfPipe,         // destinationStage (次の処理)
        vk::DependencyFlags{},                            // 依存関係フラグ (特になし)
        nullptr,                                          // メモリバリア (なし)
        nullptr,                                          // バッファバリア (なし)
        imageMemoryBarrier                                // イメージバリア (1つ)
    );
}

