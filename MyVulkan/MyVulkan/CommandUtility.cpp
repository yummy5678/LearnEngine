#include "CommandUtility.h"


CommandGenerator::CommandGenerator()
{
    m_ClassName = "CommandGenerator";
}

CommandGenerator::~CommandGenerator()
{
}

void CommandGenerator::Create(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice, uint32_t commandSize)
{
    m_bCreated = true;
    m_Pool = CreateCommandPool(logicalDevice, phygicalDevice);
    m_Buffers = CreateCommandBuffers(logicalDevice, commandSize, m_Pool);
}

void CommandGenerator::Destroy(vk::Device logicalDevice)
{
    vkDestroyCommandPool(logicalDevice, m_Pool, nullptr);
}

void CommandGenerator::RecordCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline)
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



    //とりあえず空のコマンドを作成
    for (size_t i = 0; i < m_Buffers.size(); i++)
    {
        //renderPassBeginInfo.framebuffer = m_Framebuffers[i];          // 使用するフレームバッファを設定する

        // コマンドバッファの記録を開始する
        vk::Result result = m_Buffers[i].begin(&bufferBeginInfo);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("コマンドバッファの記録の開始に失敗しました！");
        }

        // レンダーパスを開始する
        m_Buffers[i].beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline);

        //// 使用するパイプラインをバインドする
        //m_Buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

        //// パイプラインを実行する
        //m_Buffers[i].draw(3, 1, 0, 0);

        //// レンダーパスを終了する
        //m_Buffers[i].endRenderPass();

        // コマンドバッファの記録を終了する
        //result = commandBuffers[i].end();
        m_Buffers[i].end();
        //if (result != vk::Result::eSuccess)
        //{
        //    throw std::runtime_error("コマンドバッファの記録の終了に失敗しました！");
        //}
    }
}

vk::CommandPool CommandGenerator::GetPool()
{
    CheckCreated();
    return m_Pool;
}

std::vector<vk::CommandBuffer> CommandGenerator::GetBuffers()
{
    CheckCreated();
    return m_Buffers;
}

vk::CommandPool CommandGenerator::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice)
{
    QueueFamilySelector queue(phygicalDevice);
    // コマンドプールの作成に必要な情報を設定する
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.queueFamilyIndex = queue.GetGraphicIndex();	            // このコマンドプールが使用するキューファミリータイプ
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;    // コマンドバッファのリセットを許可する場合はフラグを追加する

    // グラフィックスキューファミリー用のコマンドプールを作成する
    return logicalDevice.createCommandPool(poolInfo);
    //if (!commandPool)
    //{
    //    throw std::runtime_error("コマンドプールの作成に失敗しました！");
    //}
}

std::vector<vk::CommandBuffer> CommandGenerator::CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool)
{
    // エラーチェック: logicalDeviceが有効であるか確認
    if (!logicalDevice)
    {
        throw std::runtime_error("logicalDeviceが無効です！");
    }

    // エラーチェック: commandPoolが有効であるか確認
    if (!commandPool)
    {
        throw std::runtime_error("commandPoolが無効です！");
    }

    std::vector<vk::CommandBuffer> commandBuffers;
    commandBuffers.reserve(commandSize); // 容量を確保

    // コマンドバッファを割り当てるための情報を設定する
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = commandPool;                          // コマンドバッファを割り当てるコマンドプール
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;           // コマンドバッファのレベル (PRIMARY: 直接キューに送信するバッファ)
    cbAllocInfo.commandBufferCount = commandSize; // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    commandBuffers = logicalDevice.allocateCommandBuffers(cbAllocInfo);

    // エラーチェック: コマンドバッファの割り当てに失敗していないことを確認
    if (commandBuffers.empty())
    {
        throw std::runtime_error("コマンドバッファの割り当てに失敗しました！");
    }

    return commandBuffers;
}
