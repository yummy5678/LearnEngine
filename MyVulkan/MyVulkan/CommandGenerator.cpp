#include "CommandGenerator.h"


SwapChainCommandGenerator::SwapChainCommandGenerator():
    m_LogicalDevice(nullptr),
    m_PhysicalDevice(nullptr),
    m_CommandPool(nullptr),
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
    m_SignalSemaphores  = m_SemaphoreGenerator.GetSignalSemaphores();
    m_WaitSemaphores    = m_SemaphoreGenerator.GetWaitSemaphores();

    //フェンスの作成
    m_FenceGenerator.Create(logicalDevice, commandSize);
    m_Fences            = m_FenceGenerator.GetFence();

    //コマンドプール(コマンドを置く領域)を作成
    m_CommandPool       = CreateCommandPool(logicalDevice, physicalDevice);

    //コマンドプールにコマンドバッファを割り当て
    m_CommandBuffers    = CreateCommandBuffers(logicalDevice, commandSize, m_CommandPool);




}

void SwapChainCommandGenerator::Destroy()
{
    //コマンドプールの解放
    m_LogicalDevice.freeCommandBuffers(
        m_CommandPool,
        (uint32_t)m_CommandBuffers.size(),
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


void SwapChainCommandGenerator::PresentFrame(vk::SwapchainKHR swapchain, uint32_t commandIndex)
{
    vk::PresentInfoKHR presentInfo;

    presentInfo.swapchainCount =    1;
    presentInfo.pSwapchains =       &swapchain;
    presentInfo.pImageIndices =     &commandIndex;

    // 使用するキュー(グラフィックキューやプレゼントキューなど)のインデックスを取得
    auto queueFamily = QueueFamilySelector(m_PhysicalDevice);
    auto graphicsQueue = m_LogicalDevice.getQueue(queueFamily.GetGraphicIndex(), 0);

    if (graphicsQueue.presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンの画像の表示に失敗しました！");
    }
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



