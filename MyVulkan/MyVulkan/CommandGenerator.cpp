#include "CommandGenerator.h"


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

void CommandGenerator::DrawFrame(
    vk::CommandBuffer           commandBuffer, 
    vk::RenderPass              renderpass, 
    vk::Framebuffer             framebuffer, 
    vk::Rect2D                  renderArea, 
    vk::Pipeline                graphicsPipeline, 
    vk::PipelineLayout          pipelineLayout,
    std::vector<SceneObject>    drawMeshes)
{
    // フレームの初期化する色を設定します。
    std::array<vk::ClearValue, 3> clearValues = {};
    clearValues[0].setColor({ 0.0f, 0.0f, 0.0f, 1.0f });    // 背景色
    clearValues[1].setColor({ 0.6f, 0.65f, 0.4f, 1.0f });   // 追加の背景色
    clearValues[2].setDepthStencil({ 1.0f });               // 深度バッファのクリア値

    // コマンドバッファの開始情報を設定します。
    vk::CommandBufferBeginInfo cmdBeginInfo;
    cmdBeginInfo.setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse); // バッファが再使用可能であることを示すフラグ
    if (commandBuffer.begin(&cmdBeginInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("コマンドバッファの開始に失敗しました！");
    }

    // レンダーパスの開始情報を設定します。
    vk::RenderPassBeginInfo renderpassBeginInfo{};
    renderpassBeginInfo
        .setRenderPass(renderpass)              // 使用するレンダーパス
        .setFramebuffer(framebuffer)            // 使用するフレームバッファ
        .setRenderArea(renderArea)              // 描画領域の設定
        .setClearValueCount(clearValues.size()) // クリア値の数
        .setPClearValues(clearValues.data());   // クリア値の配列

    // レンダーパスを開始します。
    commandBuffer.beginRenderPass(renderpassBeginInfo, vk::SubpassContents::eInline);

    // 使用するパイプラインをバインドします。
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    // 描画するメッシュをループします。
    for (auto& model : drawMeshes)
    {
        // プッシュ定数をシェーダーに渡します。
        commandBuffer.pushConstants(
            pipelineLayout,
            vk::ShaderStageFlagBits::eVertex,   // プッシュ定数を更新するシェーダーステージ
            0,                                  // オフセット
            sizeof(Transform),                  // プッシュするデータのサイズ
            &model.GetTransform()               // 実際のデータ
        );

        // 各メッシュをループします。
        for (auto &mesh : model.GetMeshes())
        {
            // 頂点バッファをバインド
            commandBuffer.bindVertexBuffers(0, mesh.GetVertex().GetBuffer(), { 0 });

            // ディスクリプタセットをバインドします。
            std::array<vk::DescriptorSet, 2> descriptorSetGroup = {
                descriptorSets[currentImage],
                samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
            };

            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipelineLayout,
                0,
                descriptorSetGroup,
                nullptr
            );

            // インデックスバッファ(頂点を結ぶ順番の値)を結び付けます。
            commandBuffer.bindIndexBuffer(mesh.GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
            commandBuffer.drawIndexed(mesh.GetIndex().GetSize(), 1, 0, 0, 0);   // インデックスに従って描画
        }

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



