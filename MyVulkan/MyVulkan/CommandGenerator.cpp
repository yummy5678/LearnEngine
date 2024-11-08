#include "CommandGenerator.h"
#include "RenderConfig.h"



SwapChainCommandGenerator::SwapChainCommandGenerator():
    m_LogicalDevice(),
    m_PhysicalDevice(),
    m_CommandPool(),
    m_CommandBuffers()
{
    m_ClassName = "CommandGenerator";
}

SwapChainCommandGenerator::~SwapChainCommandGenerator()
{

}

void SwapChainCommandGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize)
{
    m_bCreated = true;

    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;

    // セマフォの作成
    m_SemaphoreGenerator.Create(logicalDevice, commandSize);
    m_SignalSemaphores = m_SemaphoreGenerator.GetSignalSemaphores();
    m_WaitSemaphores = m_SemaphoreGenerator.GetWaitSemaphores();

    //フェンスの作成
    m_FenceGenerator.Create(logicalDevice, commandSize);
    m_Fences = m_FenceGenerator.GetFence();

    //コマンドプール(コマンドを置く領域)を作成
    m_CommandPool = CreateCommandPool(logicalDevice, physicalDevice);

    //コマンドプールにコマンドバッファを割り当て
    m_CommandBuffers = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);




}

void SwapChainCommandGenerator::Destroy()
{
    //コマンドプールの解放
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        m_CommandBuffers.size(),
        m_CommandBuffers.data());

    //コマンドプールの破棄
    m_LogicalDevice.destroyCommandPool(m_CommandPool);
}


vk::CommandPool SwapChainCommandGenerator::GetCammandPool()
{
    CheckCreated();
    return m_CommandPool;
}

std::vector<vk::CommandBuffer> SwapChainCommandGenerator::GetCommandBuffers()
{
    CheckCreated();
    return m_CommandBuffers;
}

// ダイナミックレンダリングに未対応
void SwapChainCommandGenerator::DrawFrame(
<<<<<<< HEAD
    vk::CommandBuffer commandBuffer,
    std::vector<RenderConfig>& configs, 
    vk::ImageView colorImage, 
    vk::ImageView depthImage)
=======
    vk::CommandBuffer			commandBuffer,
    //vk::RenderPass				renderpass,
    //vk::Framebuffer				framebuffer,
    vk::Pipeline				graphicsPipeline,
    vk::PipelineLayout			pipelineLayout,
    std::vector<SceneObject>	drawMeshes,
    SceneCamera                 sceneCamera,
    vk::Rect2D					renderArea)
>>>>>>> 011c80f570db61d0cf1756b66acf04ca41bd1a4d
{

    // カラーバッファアタッチメント
    vk::RenderingAttachmentInfo colorAttachment;
    colorAttachment.imageView = colorImage;
    colorAttachment.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.clearValue = vk::ClearValue(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}));
    
    // Depthバッファアタッチメント（3Dオブジェクト用に使用）
    vk::RenderingAttachmentInfo depthAttachment;
    depthAttachment.imageView = depthImage;
    depthAttachment.imageLayout = vk::ImageLayout::eDepthAttachmentOptimal;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.clearValue = vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0));


    for (auto& config : configs)
    {
        // ダイナミックレンダリングの設定
        vk::RenderingInfo renderingInfo;
        renderingInfo.renderArea = config.GetRenderRect();
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &colorAttachment;
        renderingInfo.pDepthAttachment = &depthAttachment;

        // Dynamic Renderingを開始
        commandBuffer.beginRendering(renderingInfo);

        // 使用するパイプラインをバインドします。
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, config.GetPipeline());

        Scene* scene = config.GetPScene();
        RenderObjects(commandBuffer, config.GetPipelineLayout(), scene->GetObjects(), scene->GetMainCamera());
        
    }

<<<<<<< HEAD
    commandBuffer.endRendering();
=======
    // レンダーパスの開始情報を設定します。
    vk::RenderPassBeginInfo renderpassBeginInfo;
    renderpassBeginInfo
        //.setRenderPass(renderpass)              // 使用するレンダーパス
        //.setFramebuffer(framebuffer)            // 使用するフレームバッファ
        .setRenderArea(renderArea)              // 描画領域の設定
        .setClearValueCount(clearValues.size()) // クリア値の数
        .setPClearValues(clearValues.data());   // クリア値の配列
>>>>>>> 011c80f570db61d0cf1756b66acf04ca41bd1a4d

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

void SwapChainCommandGenerator::PresentFrame(vk::SwapchainKHR swapchain)
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

vk::CommandPool SwapChainCommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
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

std::vector<vk::CommandBuffer> SwapChainCommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
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

vk::SubmitInfo SwapChainCommandGenerator::CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores)
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

vk::SubmitInfo SwapChainCommandGenerator::CreateSubmitInfo(vk::CommandBuffer& commandBuffer)
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

uint32_t SwapChainCommandGenerator::AcquireSwapchainNextImage(vk::SwapchainKHR swapchain)
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

void SwapChainCommandGenerator::RenderObjects(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, std::vector<SceneObject> drawMeshes, SceneCamera sceneCamera)
{
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
        for (auto& mesh : model.GetMeshes())
        {
            // 頂点バッファをバインド
            commandBuffer.bindVertexBuffers(0, mesh.GetVertex().GetBuffer(), { 0 });

            // ディスクリプタセットをバインドします。
            std::array<vk::DescriptorSet, 2> descriptorSetGroup =
            {

                //descriptorSets[currentImage], //たぶんカメラ情報が入ってる(uboViewProjection)
                //samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
                sceneCamera.GetDescriptorSet(),
                model.GetMaterials()[0].GetDescriptorSet()
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
}



