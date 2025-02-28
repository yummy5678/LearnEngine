#include "VPresentationCommand.h"

VPresentationCommand::VPresentationCommand() : 
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
    m_Swapchain(VK_NULL_HANDLE),
	m_CommandPool(VK_NULL_HANDLE),
	m_CommandBuffers(),
	m_Semaphores(),
    m_SwapChainIndex(0),
    m_QueueFamily()
{
}

VPresentationCommand::~VPresentationCommand()
{
    Cleanup();
}

void VPresentationCommand::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain)
{
    m_LogicalDevice = logicalDevice;
    m_PhysicalDevice = physicalDevice;
    m_Swapchain = swapchain;

    // スワップチェインの画像を取得
    std::vector<vk::Image> swapchainImages = logicalDevice.getSwapchainImagesKHR(swapchain);
    // 画像の枚数を取得
    size_t imageCount = swapchainImages.size();

    CreateCommandBuffer(imageCount);

    /*m_Semaphores*/


    m_QueueFamily.Initialize(m_PhysicalDevice);
}

void VPresentationCommand::RunningCommand(vk::Semaphore imageAvailableSemaphore)
{
    m_SwapChainIndex = AcquireSwapchainNextImage(imageAvailableSemaphore);

    vk::PresentInfoKHR presentInfo;

    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_Swapchain;
    presentInfo.pImageIndices = &m_SwapChainIndex;

    // 使用するキュー(グラフィックキューやプレゼントキューなど)のインデックスを取得
    auto graphicsQueue = m_LogicalDevice.getQueue(m_QueueFamily.GetGraphicIndex(), 0);

    if (graphicsQueue.presentKHR(presentInfo) != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンの画像の表示に失敗しました！");
    }
}

void VPresentationCommand::Cleanup()
{
}

void VPresentationCommand::CreateCommandPool()
{
    if (m_LogicalDevice == VK_NULL_HANDLE || m_PhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("デバイスが未作成です。コマンドプールの作成に失敗しました！");
        return;
    };

    if (m_CommandPool != VK_NULL_HANDLE)
    {
        throw std::runtime_error("既にコマンドプールは作成されています！");
        return;
    }
    QueueFamilySelector queueFamily;
    queueFamily.Initialize(m_PhysicalDevice);

    // コマンドプールの作成に必要な情報を設定する
    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.pNext = nullptr;
    poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = queueFamily.GetGraphicIndex();


    // グラフィックスキューファミリー用のコマンドプールを作成する
    m_CommandPool = m_LogicalDevice.createCommandPool(poolInfo);
}

void VPresentationCommand::CreateCommandBuffer(size_t commandSize)
{
    CreateCommandPool();

    // コマンドバッファをアロケート(割り当てる)ための情報を設定する
    vk::CommandBufferAllocateInfo cbAllocInfo;
    cbAllocInfo.commandPool = m_CommandPool;                  // コマンドバッファを割り当てるコマンドプール
    cbAllocInfo.level = vk::CommandBufferLevel::ePrimary;   // コマンドバッファの種類(PRIMARY: 直接キューに送信するバッファ)
    cbAllocInfo.commandBufferCount = commandSize;           // 割り当てるコマンドバッファの数

    // コマンドバッファを割り当てて、そのハンドルをバッファの配列に格納する
    m_CommandBuffers = m_LogicalDevice.allocateCommandBuffers(cbAllocInfo); //配列で情報をやり取りする

}

const uint32_t VPresentationCommand::AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore)
{
    // スワップチェーンから次に描画するイメージ（フレームバッファのようなもの）のインデックスを取得します。
    uint32_t imageIndex;
    vk::Result result = m_LogicalDevice.acquireNextImageKHR(
        m_Swapchain,                            // スワップチェーン
        std::numeric_limits<uint64_t>::max(),   // タイムアウトの設定(ここでは無限待機)
        imageAvailableSemaphore,                // イメージが使用可能になるのを通知するセマフォ
        nullptr,                                // フェンス(ここでは使用しないのでnullptr)
        &imageIndex                             // イメージのインデックスが格納される
    );

    // イメージ取得に失敗した場合、エラーメッセージを投げる
    if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("スワップチェーンからイメージを取得できませんでした！");
    }

    return imageIndex;
}
