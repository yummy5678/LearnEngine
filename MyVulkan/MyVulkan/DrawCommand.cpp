#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_pLogicalDevice(nullptr),
    m_PhysicalDevice(VK_NULL_HANDLE),
    m_ImageDrawIndex(0),
    m_ImageSet(),
    m_CommandBuffers(),
    m_CommandPool()
{
}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device* pLogicalDevice, vk::PhysicalDevice physicalDevice, std::vector<ImageViewSet> imageSet)
{
    m_pLogicalDevice = pLogicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_ImageSet = imageSet;

    //// セマフォの作成
    //m_SemaphoreGenerator.Create(pLogicalDevice, commandSize);
    //m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    //m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    ////フェンスの作成
    //m_FenceGenerator.Create(pLogicalDevice, commandSize);
    //m_Fences = m_FenceGenerator.GetFence();

    //コマンドプール(コマンドを置く領域)を作成
    m_CommandPool = CreateCommandPool(pLogicalDevice, physicalDevice);

    //コマンドプールにコマンドバッファを割り当て
    m_CommandBuffers = CreateCommandBuffers(pLogicalDevice, imageSet.size(), m_CommandPool);

}

void DrawCommand::Destroy()
{
    //コマンドプールの解放
    m_pLogicalDevice->freeCommandBuffers(
        m_CommandPool,
        (uint32_t)m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //コマンドプールの破棄
    m_pLogicalDevice->destroyCommandPool(m_CommandPool);
}

void DrawCommand::BeginRendering(vk::Pipeline pipeline, vk::Rect2D renderArea)
{
    if (m_ImageSet.empty() == true) return; // 描画する為のイメージビューがセットされていなければ何もしない

    // インデックスのカウントを進める
    m_ImageDrawIndex++;
    m_ImageDrawIndex %= m_ImageSet.size();

    // カラーバッファアタッチメント
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = m_ImageSet[m_ImageDrawIndex].color;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));

    // Depthバッファアタッチメント（3Dオブジェクト用に使用）
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = m_ImageSet[m_ImageDrawIndex].depth;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));


    // ダイナミックレンダリングの設定
    vk::RenderingInfo renderingInfo;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &colorAttachment;
    renderingInfo.pDepthAttachment = &depthAttachment;

    // Dynamic Renderingを開始
    auto commandBuffer = m_CommandBuffers[m_ImageDrawIndex];
    commandBuffer.beginRendering(renderingInfo);

    // 使用するパイプラインをバインドします。
    m_CommandBuffers[m_ImageDrawIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

}

void DrawCommand::EndRendering()
{
    m_CommandBuffers[m_ImageDrawIndex].endRendering();
}


vk::CommandPool DrawCommand::CreateCommandPool(vk::Device* pLogicalDevice, vk::PhysicalDevice physicalDevice)
{
    // Get indices of queue families from device
    QueueFamilySelector queue(physicalDevice);

    // コマンドプールの作成に必要な情報を設定する
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;	// コマンドバッファのリセットを許可する場合はフラグを追加する
    poolInfo.queueFamilyIndex = queue.GetGraphicIndex();	            // このコマンドプールが使用するキューファミリー

    // グラフィックスキューファミリー用のコマンドプールを作成する
    vk::CommandPool pool;
    try
    {
        pool = pLogicalDevice->createCommandPool(poolInfo);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("コマンドプールの作成に失敗しました！");
    }

    return pool;
}

std::vector<vk::CommandBuffer> DrawCommand::CreateCommandBuffers(vk::Device* pLogicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{
    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // コマンドバッファを割り当てるコマンドプール
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    cbAllocInfo.commandBufferCount = commandSize;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    std::vector<vk::CommandBuffer> commandBuffers = pLogicalDevice->allocateCommandBuffers(cbAllocInfo); //配列で情報をやり取りする

    // エラーチェック
    if (commandBuffers.empty())
    {
        throw std::runtime_error("コマンドバッファの割り当てに失敗しました！");
    }

    return commandBuffers;
}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
{
    vk::SubmitInfo submitInfo;
    //submitInfo.pNext;
    submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores.size();
    submitInfo.pSignalSemaphores = m_SignalSemaphores.data();
    submitInfo.allowDuplicate;
    submitInfo.pWaitDstStageMask;
    submitInfo.waitSemaphoreCount = (uint32_t)m_WaitSemaphores.size();
    submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
    submitInfo.commandBufferCount = (uint32_t)commandBuffers.size();
    submitInfo.pCommandBuffers = commandBuffers.data();

    return submitInfo;
}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(vk::CommandBuffer& commandBuffer)
{
    vk::SubmitInfo submitInfo;
    //submitInfo.pNext;
    //submitInfo.signalSemaphoreCount;
    //submitInfo.pSignalSemaphores;
    //submitInfo.allowDuplicate;
    //submitInfo.pWaitDstStageMask;
    //submitInfo.waitSemaphoreCount;
    //submitInfo.pWaitSemaphores;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    return submitInfo;
}

void DrawCommand::RenderMesh(
    vk::PipelineLayout pipelineLayout,
    std::vector<vk::DescriptorSet>* descriptorSets,
    VMeshObject* drawMesh,
    Transform* ObjectTransform)
{

    // プッシュ定数をシェーダーに渡します。
    m_CommandBuffers[m_ImageDrawIndex].pushConstants(
        pipelineLayout,
        vk::ShaderStageFlagBits::eVertex,   // プッシュ定数を更新するシェーダーステージ
        0,                                  // オフセット
        sizeof(Transform),                  // プッシュするデータのサイズ
        ObjectTransform                     // 実際のデータ
    );


    // 頂点バッファをバインド
    m_CommandBuffers[m_ImageDrawIndex].bindVertexBuffers(0, drawMesh->GetPMesh()->GetVertex().GetBuffer(), {0});

    m_CommandBuffers[m_ImageDrawIndex].bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        pipelineLayout,
        0,
        *descriptorSets,
        nullptr);

        // インデックスバッファ(頂点を結ぶ順番の値)を結び付けます。
    m_CommandBuffers[m_ImageDrawIndex].bindIndexBuffer(drawMesh->GetPMesh()->GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
    m_CommandBuffers[m_ImageDrawIndex].drawIndexed(drawMesh->GetPMesh()->GetIndex().GetSize(), 1, 0, 0, 0);   // インデックスに従って描画

}
