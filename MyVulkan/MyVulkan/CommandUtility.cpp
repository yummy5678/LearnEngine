#include "CommandUtility.h"


CommandGenerator::CommandGenerator():
    m_LogicalDevice(),
    m_PhysicalDevice(),
    //m_QueueFamily(m_PhysicalDevice),
    m_CommandPool(),
    m_CommandBuffers()
{
    m_ClassName = "CommandGenerator";
}

CommandGenerator::~CommandGenerator()
{

}

void CommandGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;

    //コマンドプール(コマンドを置く領域)を作成
    m_CommandPool = CreateCommandPool(logicalDevice, physicalDevice);

    //コマンドプールにコマンドバッファを割り当て
    m_CommandBuffers = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);


}

void CommandGenerator::Destroy()
{
    //コマンドプールの解放
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //コマンドプールの破棄
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}

void CommandGenerator::RecordGraphicCommands(std::vector<vk::Framebuffer> framebuffers, vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline)
{
    CheckCreated();

    // 各コマンドバッファの開始方法に関する情報
    vk::CommandBufferBeginInfo bufferBeginInfo;
    bufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse; // バッファが再使用可能であることを示すフラグ

    // レンダーパスを開始するための情報 (グラフィカルなアプリケーションの場合のみ必要)
    vk::RenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.renderPass = renderPass;                             // 開始するレンダーパス
    renderPassBeginInfo.renderArea.offset = vk::Offset2D{ 0, 0 };              // レンダーパスの開始位置 (ピクセル単位)
    renderPassBeginInfo.renderArea.extent = extent;                          // レンダーパスを実行する領域のサイズ (offsetから始まる)
    std::array<vk::ClearValue, 1> clearValues = {
        vk::ClearValue{std::array<float, 4>{0.6f, 0.65f, 0.4f, 1.0f}}        // クリアする値のリスト
    };
    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();                   // クリアする値のリスト


    for (size_t i = 0; i < m_CommandBuffers.size(); i++)
    {
        renderPassBeginInfo.framebuffer = framebuffers[i];          // 使用するフレームバッファを設定する

        // コマンドバッファの記録を開始する
        vk::Result result = m_CommandBuffers[i].begin(&bufferBeginInfo);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("コマンドバッファの記録の開始に失敗しました！");
        }

        // レンダーパスを開始する
        m_CommandBuffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        // 使用するパイプラインをバインドする
        m_CommandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        // パイプラインを実行する
        m_CommandBuffers[i].draw(3, 1, 0, 0);

        // レンダーパスを終了する
        m_CommandBuffers[i].endRenderPass();

        // コマンドバッファの記録を終了する
        //result = m_Buffers[i].end();
        m_CommandBuffers[i].end();
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("コマンドバッファの記録の終了に失敗しました！");
        }
    }
}

//void CommandGenerator::RecordGraphicCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline)
//{
//    CheckCreated();
//
//    // 各コマンドバッファの開始方法に関する情報
//    vk::CommandBufferBeginInfo bufferBeginInfo;
//    bufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse; // バッファが再使用可能であることを示すフラグ
//
//    // レンダーパスを開始するための情報 (グラフィカルなアプリケーションの場合のみ必要)
//    vk::RenderPassBeginInfo renderPassBeginInfo;
//    renderPassBeginInfo.renderPass = renderPass;                             // 開始するレンダーパス
//    renderPassBeginInfo.renderArea.offset = vk::Offset2D{ 0, 0 };              // レンダーパスの開始位置 (ピクセル単位)
//    renderPassBeginInfo.renderArea.extent = extent;                          // レンダーパスを実行する領域のサイズ (offsetから始まる)
//    std::array<vk::ClearValue, 1> clearValues = {
//        vk::ClearValue{std::array<float, 4>{0.6f, 0.65f, 0.4f, 1.0f}}        // クリアする値のリスト
//    };
//    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
//    renderPassBeginInfo.pClearValues = clearValues.data();                   // クリアする値のリスト
//
//
//
//    //とりあえず空のコマンドを作成
//    for (size_t i = 0; i < m_Buffers.size(); i++)
//    {
//        //renderPassBeginInfo.framebuffer = m_Framebuffers[i];          // 使用するフレームバッファを設定する
//
//        // コマンドバッファの記録を開始する
//        vk::Result result = m_Buffers[i].begin(&bufferBeginInfo);
//        if (result != vk::Result::eSuccess)
//        {
//            throw std::runtime_error("コマンドバッファの記録の開始に失敗しました！");
//        }
//
//        // レンダーパスを開始する
//        m_Buffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);
//
//        // 使用するパイプラインをバインドする
//        m_Buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
//
//        // パイプラインを実行する
//        m_Buffers[i].draw(3, 1, 0, 0);
//
//        // レンダーパスを終了する
//        m_Buffers[i].endRenderPass();
//
//        // コマンドバッファの記録を終了する
//        //result = m_Buffers[i].end();
//        m_Buffers[i].end();
//        if (result != vk::Result::eSuccess)
//        {
//            throw std::runtime_error("コマンドバッファの記録の終了に失敗しました！");
//        }
//    }
//}

vk::CommandPool CommandGenerator::GetCammandPool()
{
    CheckCreated();
    return m_CommandPool;
}

std::vector<vk::CommandBuffer> CommandGenerator::GetCommandBuffers()
{
    CheckCreated();
    return m_CommandBuffers;
}

void CommandGenerator::DrawFrame(vk::RenderPass renderpass, vk::Framebuffer framebuffer, vk::Rect2D renderArea, vk::Pipeline graphicsPipeline)
{
    //指定したフレームバッファにレンダーパスとパイプラインを関連付けて
    //書き込むコマンドの作成と送信を行う関数

    //フレームの初期化する色
    vk::ClearValue clearVal;
    clearVal.color.float32[0] = 0.0f;
    clearVal.color.float32[1] = 0.0f;
    clearVal.color.float32[2] = 0.0f;
    clearVal.color.float32[3] = 1.0f;

    vk::CommandBufferBeginInfo cmdBeginInfo;
    m_CommandBuffers[0].begin(cmdBeginInfo);

    vk::RenderPassBeginInfo renderpassBeginInfo;
    renderpassBeginInfo.renderPass = renderpass;
    renderpassBeginInfo.framebuffer = framebuffer;
    renderpassBeginInfo.renderArea = renderArea;
    renderpassBeginInfo.clearValueCount = 1;
    renderpassBeginInfo.pClearValues = &clearVal;

    m_CommandBuffers[0].beginRenderPass(renderpassBeginInfo, vk::SubpassContents::eInline);

    // 使用するパイプラインをバインドする
    m_CommandBuffers[0].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    // ここでサブパス0番の処理
    m_CommandBuffers[0].draw(3, 1, 0, 0);

    m_CommandBuffers[0].endRenderPass();

    m_CommandBuffers[0].end();

    //vk::SubmitInfo submitInfo;
    //submitInfo.pNext;
    //submitInfo.signalSemaphoreCount;
    //submitInfo.pSignalSemaphores;
    //submitInfo.allowDuplicate;
    //submitInfo.pWaitDstStageMask;
    //submitInfo.waitSemaphoreCount;
    //submitInfo.pWaitSemaphores;
    //submitInfo.commandBufferCount = 1;
    //submitInfo.pCommandBuffers = &m_Buffers[0];

    auto submit = CreateSubmitInfo(m_CommandBuffers[0]);

    //// 使用するキュー（グラフィックキューやプレゼントキューなど）のインデックスを取得
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    graphicsQueue.submit(submit, nullptr);

    //vk::PresentInfoKHR presentInfo;
    //presentInfo.pNext;
    //presentInfo.waitSemaphoreCount;
    //presentInfo.pWaitSemaphores = nullptr;
    //presentInfo.swapchainCount;
    //presentInfo.pSwapchains = nullptr;
    //presentInfo.pImageIndices = &imageIndex;
    //presentInfo.pResults;
    //graphicsQueue.presentKHR(presentInfo);
}

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
    QueueFamilySelector queue(physicalDevice);
    // コマンドプールの作成に必要な情報を設定する
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;    // コマンドバッファのリセットを許可する場合はフラグを追加する
    poolInfo.queueFamilyIndex = queue.GetGraphicIndex();	            // このコマンドプールが使用するキューファミリータイプ
    poolInfo.allowDuplicate;

    // グラフィックスキューファミリー用のコマンドプールを作成する
    return logicalDevice.createCommandPool(poolInfo);
    //if (!commandPool)
    //{
    //    throw std::runtime_error("コマンドプールの作成に失敗しました！");
    //}
}

std::vector<vk::CommandBuffer> CommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{

    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                  // コマンドバッファを割り当てるコマンドプール
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    cbAllocInfo.commandBufferCount = commandSize;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    std::vector<vk::CommandBuffer> commandBuffers = logicalDevice.allocateCommandBuffers(cbAllocInfo); //配列で情報をやり取りする

    // エラーチェック
    if (commandBuffers.empty())
    {
        throw std::runtime_error("コマンドバッファの割り当てに失敗しました！");
    }

    return commandBuffers;
}

vk::SubmitInfo CommandGenerator::CreateSubmitInfo(vk::CommandBuffer commandBuffer)
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



