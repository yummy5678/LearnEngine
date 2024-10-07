#include "CommandUtility.h"




CommandGenerator::CommandGenerator():
    m_LogicalDevice(),
    m_PhysicalDevice(),
    m_CommandPool(),
    m_CommandBuffers()
{
    m_ClassName = "CommandGenerator";
}

CommandGenerator::~CommandGenerator()
{

}

void CommandGenerator::LoadShader(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;

    // セマフォの作成
    m_SemaphoreGenerator.LoadShader(logicalDevice, commandSize);
    m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    //フェンスの作成
    m_FenceGenerator.LoadShader(logicalDevice, commandSize);
    m_Fences = m_FenceGenerator.GetFence();

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

void CommandGenerator::DrawFrame(vk::CommandBuffer buffer, vk::RenderPass renderpass, vk::Framebuffer framebuffer, vk::Rect2D renderArea, vk::Pipeline graphicsPipeline, std::vector<SceneObject> drawMeshes)
{
    //指定したフレームバッファにレンダーパスとパイプラインを関連付けて
    //書き込むコマンドの作成と送信を行う関数

    //フレームの初期化する色
    std::array<vk::ClearValue, 3> clearValues = {};
    clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
    clearValues[1].color = { 0.6f, 0.65f, 0.4f, 1.0f };
    clearValues[2].depthStencil.depth = 1.0f;

    vk::CommandBufferBeginInfo cmdBeginInfo;
    if (buffer.begin(&cmdBeginInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("コマンドバッファの開始に失敗しました！");
    }

    vk::RenderPassBeginInfo renderpassBeginInfo;
    renderpassBeginInfo.renderPass = renderpass;
    renderpassBeginInfo.framebuffer = framebuffer;
    renderpassBeginInfo.renderArea = renderArea;
    renderpassBeginInfo.clearValueCount = clearValues.size();
    renderpassBeginInfo.pClearValues = clearValues.data();

    buffer.beginRenderPass(renderpassBeginInfo, vk::SubpassContents::eInline);

    // 使用するパイプラインをバインドする
    buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    {////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        for (size_t j = 0; j < drawMeshes.size(); j++)
        {
            auto thisModel = drawMeshes[j];
            auto thisMeshes = thisModel.GetMesh();

            vkCmdPushConstants(
                buffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT,		// Stage to push constants to
                0,								// Offset of push constants to update
                sizeof(Transform),				// Size of data being pushed
                &thisModel.GetTransform());			// Actual data being pushed (can be array)

            for (size_t k = 0; k < thisMeshes.meshes.size(); k++)
            {

                VkBuffer vertexBuffers[] = { thisModel.GetMeshBuffer()};					// Buffers to bind
                VkDeviceSize offsets[] = { 0 };												// Offsets into buffers being bound
                vkCmdBindVertexBuffers(commandBuffers[currentImage], 0, 1, vertexBuffers, offsets);	// Command to bind vertex buffer before drawing with them

                // Bind mesh index buffer, with 0 offset and using the uint32 type
                vkCmdBindIndexBuffer(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

                // Dynamic Offset Amount
                // uint32_t dynamicOffset = static_cast<uint32_t>(modelUniformAlignment) * j;

                // "Push" constants to given shader stage directly (no buffer)


                std::array<VkDescriptorSet, 2> descriptorSetGroup = { descriptorSets[currentImage],
                    samplerDescriptorSets[thisModel.getMesh(k)->getTexId()] };

                // Bind Descriptor Sets
                vkCmdBindDescriptorSets(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
                    0, static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0, nullptr);

                // Execute pipeline
                vkCmdDrawIndexed(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexCount(), 1, 0, 0, 0);
            }
        }

    }////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ここでサブパス0番の処理
    buffer.draw(3, 1, 0, 0);

    buffer.endRenderPass();

    buffer.end();

    auto submit = CreateSubmitInfo(buffer);

    // 使用するキュー（グラフィックキューやプレゼントキューなど）のインデックスを取得
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    graphicsQueue.submit(submit, nullptr);

    // 終了待機
    //graphicsQueue.waitIdle();

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

void CommandGenerator::PresentFrame(vk::SwapchainKHR swapchain)
{
    vk::PresentInfoKHR presentInfo;

    auto index = AcquireSwapchainNextImage(swapchain);

    auto presentSwapchains = { swapchain };
    auto imgIndices = { index };

    presentInfo.swapchainCount = presentSwapchains.size();
    presentInfo.pSwapchains = presentSwapchains.begin();
    presentInfo.pImageIndices = imgIndices.begin();

    // 使用するキュー(グラフィックキューやプレゼントキューなど)のインデックスを取得
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    if (graphicsQueue.presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンの画像の表示に失敗しました！");
    }

    // graphicsQueue.waitIdle();
}

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
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
        pool = logicalDevice.createCommandPool(poolInfo);
    }
    catch (const std::exception&)
    {
        throw std::runtime_error("コマンドプールの作成に失敗しました！");
    }

    return pool;
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

vk::SubmitInfo CommandGenerator::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
{
    vk::SubmitInfo submitInfo;
    //submitInfo.pNext;
    submitInfo.signalSemaphoreCount = m_SignalSemaphores.size();
    submitInfo.pSignalSemaphores = m_SignalSemaphores.data();
    submitInfo.allowDuplicate;
    submitInfo.pWaitDstStageMask;
    submitInfo.waitSemaphoreCount = m_WaitSemaphores.size();
    submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
    submitInfo.commandBufferCount = commandBuffers.size();
    submitInfo.pCommandBuffers = commandBuffers.data();

    return submitInfo;
}

vk::SubmitInfo CommandGenerator::CreateSubmitInfo(vk::CommandBuffer& commandBuffer)
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

uint32_t CommandGenerator::AcquireSwapchainNextImage(vk::SwapchainKHR swapchain)
{
    // スワップチェーンから次に描画するイメージ（フレームバッファのようなもの）のインデックスを取得します。
    uint32_t imageIndex;
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
	    swapchain,                              // スワップチェーン
	    std::numeric_limits<uint64_t>::max(),   // タイムアウトの設定（ここでは無限待機）
	    //imageAvailable[currentFrame],         // イメージが使用可能になるのを通知するセマフォ
	    nullptr,                                // イメージが使用可能になるのを通知するセマフォ
	    nullptr,                                // フェンス（ここでは使用しないのでnullptr）
	    &imageIndex                             // イメージのインデックスが格納される
    );
    // イメージ取得に失敗した場合、エラーメッセージを投げる
    if (result != vk::Result::eSuccess)
    {
	    throw std::runtime_error("スワップチェーンからイメージを取得できませんでした！");
    }

    return imageIndex;
}



