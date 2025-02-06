#include "DrawCommand.h"



DrawCommand::DrawCommand() : 
    m_pLogicalDevice(nullptr),
    m_pPhysicalDevice(VK_NULL_HANDLE),
    m_ImageDrawIndex(0),
    m_ImageSet(),
    m_CommandBuffers(),
    m_CommandPool(VK_NULL_HANDLE),
    m_QueueSelector(m_pPhysicalDevice)
{

}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::Create(vk::Device* pLogicalDevice, vk::PhysicalDevice* pPhysicalDevice, std::vector<ImageViewSet> imageSet)
{
    m_pLogicalDevice = pLogicalDevice;
    m_pPhysicalDevice = pPhysicalDevice;
    m_ImageSet = imageSet;

    //// セマフォの作成
    //m_SemaphoreGenerator.Create(pLogicalDevice, commandSize);
    //m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    //m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    ////フェンスの作成
    //m_FenceGenerator.Create(pLogicalDevice, commandSize);
    //m_Fences = m_FenceGenerator.GetFence();

    //コマンドプール(コマンドを置く領域)を作成
    CreateCommandPool(pLogicalDevice);

    //コマンドプールにコマンドバッファを割り当て
    CreateCommandBuffers(pLogicalDevice, imageSet.size(), m_CommandPool);

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

vk::CommandBuffer DrawCommand::GetBuffer()
{
    if (m_CommandBuffers.empty() == true) 
        throw std::runtime_error("コマンドを取り出そうとしましたが、それはまだ発行されていません");

    return m_CommandBuffers[m_ImageDrawIndex];
}

void DrawCommand::BeginRendering(uint32_t index, vk::Rect2D renderArea)
{
    if (m_ImageSet.empty() == true) return; // 描画する為のイメージがセットされていなければ何もしない

    // インデックスのカウントを進める
    m_ImageDrawIndex = index % m_ImageSet.size();   //  添え字がコマンド数の範囲内に収まるよう調整

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
    auto commandBuffer = m_CommandBuffers[m_ImageDrawIndex];
    commandBuffer.begin(beginInfo);
    commandBuffer.beginRendering(renderingInfo);



}

void DrawCommand::EndRendering()
{
    m_CommandBuffers[m_ImageDrawIndex].endRendering();
    m_CommandBuffers[m_ImageDrawIndex].end();

    // キューにコマンドを送信
    vk::Queue queue = m_pLogicalDevice->getQueue(m_QueueSelector.GetGraphicIndex(), 0);
    queue.submit(CreateSubmitInfo(m_CommandBuffers[m_ImageDrawIndex]), m_Fences[m_ImageDrawIndex]);
}


void DrawCommand::CreateCommandPool(vk::Device* pLogicalDevice)
{
    if(m_pLogicalDevice == nullptr || *m_pLogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドプールを作成しようとしましたが、論理デバイスがNULLです");

    if (m_pPhysicalDevice == nullptr || *m_pPhysicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドプールを作成しようとしましたが、物理デバイスがNULLです");

    // コマンドプールの作成に必要な情報を設定する
    VkCommandPoolCreateInfo poolInfo;
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;	// コマンドバッファのリセットを許可する場合はフラグを追加する
    poolInfo.queueFamilyIndex = m_QueueSelector.GetGraphicIndex();	            // このコマンドプールが使用するキューファミリー

   m_CommandPool = pLogicalDevice->createCommandPool(poolInfo);
}

void DrawCommand::CreateCommandBuffers(vk::Device* pLogicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{
    if (m_pLogicalDevice == nullptr || *m_pLogicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドバッファを作成しようとしましたが、論理デバイスがNULLです");

    if (m_CommandPool == VK_NULL_HANDLE)
        throw std::runtime_error("コマンドバッファを作成しようとしましたが、コマンドプールがNULLです");


    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // コマンドバッファを割り当てるコマンドプール
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    cbAllocInfo.commandBufferCount = commandSize;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    m_CommandBuffers = pLogicalDevice->allocateCommandBuffers(cbAllocInfo); //配列で情報をやり取りする

}

vk::SubmitInfo DrawCommand::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
{
    vk::SubmitInfo submitInfo;
    submitInfo.pNext;
    submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores.size();
    submitInfo.pSignalSemaphores = m_SignalSemaphores.data();
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

//void DrawCommand::RenderMesh(
//    vk::Pipeline pipeline,
//    vk::PipelineLayout pipelineLayout,
//    std::vector<vk::DescriptorSet>* descriptorSets,
//    VMeshObject* drawMesh,
//    Transform* ObjectTransform)
//{
//
//    // 使用するパイプラインをバインドします。
//    m_CommandBuffers[m_ImageDrawIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
//
//    // プッシュ定数をシェーダーに渡します。
//    m_CommandBuffers[m_ImageDrawIndex].pushConstants(
//        pipelineLayout,
//        vk::ShaderStageFlagBits::eVertex,   // プッシュ定数を更新するシェーダーステージ
//        0,                                  // オフセット
//        sizeof(Transform),                  // プッシュするデータのサイズ
//        ObjectTransform                     // 実際のデータ
//    );
//
//
//    // 頂点バッファをバインド
//    m_CommandBuffers[m_ImageDrawIndex].bindVertexBuffers(0, drawMesh->GetSPMesh()->GetVertex().GetBuffer(), {0});
//
//    m_CommandBuffers[m_ImageDrawIndex].bindDescriptorSets(
//        vk::PipelineBindPoint::eGraphics,
//        pipelineLayout,
//        0,
//        *descriptorSets,
//        nullptr);
//
//        // インデックスバッファ(頂点を結ぶ順番の値)を結び付けます。
//    m_CommandBuffers[m_ImageDrawIndex].bindIndexBuffer(drawMesh->GetSPMesh()->GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
//    m_CommandBuffers[m_ImageDrawIndex].drawIndexed(drawMesh->GetSPMesh()->GetIndex().GetSize(), 1, 0, 0, 0);   // インデックスに従って描画
//
//}
