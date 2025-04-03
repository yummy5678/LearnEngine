#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_LogicalDevice(VK_NULL_HANDLE),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_Swapchain(VK_NULL_HANDLE),
    m_ImageSet(nullptr),
    m_CommandBuffers(),
    m_CommandPool(VK_NULL_HANDLE),
    m_RenderFinishedSemaphores(),
    //m_Fences(),
    m_QueueSelector()
{

}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_QueueSelector.Initialize(m_PhysicalDevice);

    //コマンドプール(コマンドを置く領域)を作成
    CreateCommandPool();

    //コマンドプールにコマンドバッファを割り当て
    CreateCommandBuffers(1, m_CommandPool);

    // セマフォの作成
    CreateSemaphore(m_RenderFinishedSemaphores);
    //フェンスの作成
    //CreateFence(m_Fences);


}

//void DrawCommand::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, SwapchainRenderer* swapchainRenderer)
//{
//    m_Swapchain = swapchainRenderer->GetSwapchain();
//
//    m_LogicalDevice = logicalDevice;
//    m_PhysicalDevice = physicalDevice;
//    m_ImageSet = swapchainRenderer->GetImageSets();
//    m_QueueSelector.Initialize(m_PhysicalDevice);
//
//    // 使用するフレームの枚数
//    size_t imageSize = m_ImageSet.size();
//
//    // フレームの数だけセマフォとフェンスを作成
//    m_RenderFinishedSemaphores.resize(imageSize);
//    m_ImageAvailableSemaphores.resize(imageSize);
//    m_Fences.resize(imageSize);
//    for (size_t i = 0; i < imageSize; i++)
//    {
//        // セマフォの作成
//        CreateSemaphore(m_RenderFinishedSemaphores[i]);
//        CreateSemaphore(m_ImageAvailableSemaphores[i]);
//
//        //フェンスの作成
//        CreateFence(m_Fences[i]);
//    }
//
//    //コマンドプール(コマンドを置く領域)を作成
//    CreateCommandPool();
//
//    //コマンドプールにコマンドバッファを割り当て
//    CreateCommandBuffers(imageSize, m_CommandPool);
//}

void DrawCommand::Destroy()
{
    //コマンドプールの解放
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        1,
        &m_CommandBuffers);

    //コマンドプールの破棄
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

vk::CommandBuffer DrawCommand::GetBuffer()
{
    return m_CommandBuffers;
}

void DrawCommand::BeginRendering(RenderingImageSet* imageSet, vk::Semaphore imageAvableSemaphore, vk::Rect2D renderArea)
{
    m_ImageSet = imageSet;
    m_ImageAvailableSemaphores = imageAvableSemaphore;

    // カラーバッファアタッチメント
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.pNext;
    colorAttachment.imageView = m_ImageSet->color.view;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 1.0f, 1.0f}));
    colorAttachment.resolveImageView = VK_NULL_HANDLE;
    colorAttachment.resolveImageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.resolveMode = vk::ResolveModeFlagBits::eNone;



    // 深度バッファアタッチメント（3Dオブジェクト用に使用）
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = m_ImageSet->depth.view;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));
    depthAttachment.resolveImageView = VK_NULL_HANDLE;
    depthAttachment.resolveImageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.resolveMode = vk::ResolveModeFlagBits::eNone;

    // 深度画像にステンシルも含めるので必要ない
    //vk::RenderingAttachmentInfo stencilAttachment;

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags;
    beginInfo.pNext;
    beginInfo.pInheritanceInfo; // セカンダリコマンドバッファ(使用しない)


    // ダイナミックレンダリングの設定
    vk::RenderingInfo renderingInfo;
    renderingInfo.pNext;
    renderingInfo.flags;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;
    renderingInfo.pStencilAttachment = VK_NULL_HANDLE;

    // Dynamic Renderingを開始
    auto commandBuffer = m_CommandBuffers;
    commandBuffer.begin(beginInfo);
    commandBuffer.beginRendering(renderingInfo);
}

void DrawCommand::EndRendering(vk::Fence fence, vk::ImageLayout newImageLayout)
{
    // 描画コマンドの記録を終了する
    m_CommandBuffers.endRendering();

    // コマンドの終了前にイメージの状態を使用目的に合わせて変更する
    ImageMemoryBarrier(m_ImageSet->color.buffer, vk::ImageLayout::eUndefined, newImageLayout);

    // コマンドの記録を閉じる
    m_CommandBuffers.end();

    // 送信情報の作成
    std::vector<vk::PipelineStageFlags> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    vk::SubmitInfo submitInfo = CreateSubmitInfo(&waitStages);

    // キューにコマンドを送信
    vk::Queue queue = m_LogicalDevice.getQueue(m_QueueSelector.GetGraphicIndex(), 0);
    queue.submit(submitInfo, fence);
}

//vk::Semaphore DrawCommand::GetImageAvableSemaphore()
//{
//    return m_ImageAvailableSemaphores;
//}

vk::Semaphore DrawCommand::GetSignalSemaphore()
{
    return m_RenderFinishedSemaphores;
}

//vk::Fence DrawCommand::GetFence()
//{
//    return m_Fences;
//}

//void DrawCommand::WaitFence()
//{
//    std::vector<vk::Fence> usingFences = { m_Fences };
//    m_LogicalDevice.waitForFences(
//        usingFences,							// 利用するフェンス達
//        VK_TRUE,								// フェンスが全てシグナル状態になるまで待つ
//        UINT64_MAX);							// 最大待機時間
//
//    m_LogicalDevice.resetFences(usingFences);	// フェンスを非シグナル状態にする
//}


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
    vk::CommandBufferAllocateInfo allocateInfo;
    allocateInfo.commandPool = commandPool;                  // コマンドバッファを割り当てるコマンドプール
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    allocateInfo.commandBufferCount = commandSize;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(allocateInfo).front(); //配列で情報をやり取りする



}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(
    std::vector<vk::PipelineStageFlags>* waitStages)
{
    vk::SubmitInfo submitInfo;
    submitInfo.pNext;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffers;

    // 画像の枚数が複数ある場合は同期処理を行う
    if (m_Swapchain != VK_NULL_HANDLE)
    {
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores;
        submitInfo.pWaitDstStageMask = waitStages->data();   
    }

    return submitInfo;
}

void DrawCommand::ImageMemoryBarrier(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
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
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.newLayout = newLayout;
    imageMemoryBarrier.oldLayout = oldLayout;
    imageMemoryBarrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    // パイプラインバリア
    m_CommandBuffers.pipelineBarrier
    (
        vk::PipelineStageFlagBits::eColorAttachmentOutput, // sourceStage (レンダリングの最後)
        vk::PipelineStageFlagBits::eBottomOfPipe,         // destinationStage (次の処理)
        vk::DependencyFlags{},                            // 依存関係フラグ (特になし)
        nullptr,                                          // メモリバリア (なし)
        nullptr,                                          // バッファバリア (なし)
        imageMemoryBarrier                                // イメージバリア (1つ)
    );
}

