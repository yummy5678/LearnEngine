#include "RenderPassUtility.h"
using namespace VulkanUtility;


RenderpassGenerator::RenderpassGenerator(vk::Device logicalDevice, vk::SwapchainCreateInfoKHR swapchainInfo)
{
    CreateRenderpass(logicalDevice, swapchainInfo);


}

RenderpassGenerator::~RenderpassGenerator()
{
    Release();
}

void RenderpassGenerator::CreateRenderpass(vk::Device logicalDevice,vk::SwapchainCreateInfoKHR swapchainInfo)
{
    auto colorAttachment = CreateColorAttachment(swapchainInfo);
    auto subpass = CreateSubpass();
    auto dependencies = CreateDependencies();

    auto createInfo = CreateInfo(colorAttachment, subpass, dependencies);

    //レンダーパスを作成

    m_RenderPass = logicalDevice.createRenderPassUnique(createInfo);
    //if (!renderPass)
    //{
    //    throw std::runtime_error("レンダーパスの作成に失敗しました!");
    //}
}

vk::RenderPass RenderpassGenerator::GetRenderpass()
{
    return m_RenderPass.get();
}

void RenderpassGenerator::Release()
{
    //m_pLogicalDevice->destroyRenderPass(m_RenderPass, nullptr);
}

vk::AttachmentDescription* RenderpassGenerator::CreateColorAttachment(const vk::SwapchainCreateInfoKHR swapchainInfo)
{
    // カラーバッファアタッチメントの記述
    m_ColorAttachment.flags = {};
    m_ColorAttachment.format = swapchainInfo.imageFormat;       // スワップチェインのイメージフォーマット
    m_ColorAttachment.samples = vk::SampleCountFlagBits::e1;      // マルチサンプリングのサンプル数
    m_ColorAttachment.loadOp = vk::AttachmentLoadOp::eClear;     // レンダーパスの開始時にカラーバッファをクリア
    m_ColorAttachment.storeOp = vk::AttachmentStoreOp::eStore;    // レンダーパスの終了時にカラーバッファを保存
    m_ColorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;  // ステンシルバッファを使用しない
    m_ColorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare; // ステンシルバッファを使用しない
    m_ColorAttachment.initialLayout = vk::ImageLayout::eUndefined;      // レンダーパス開始前のレイアウト
    m_ColorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;  // レンダーパス終了後のレイアウト（表示用）
    
    return &m_ColorAttachment;
}

vk::SubpassDescription* RenderpassGenerator::CreateSubpass()
{
    // アタッチメント参照
    // サブパスからレンダリング結果の色を出力するアタッチメントがどれかを指定します
    m_ColorAttachmentRef.attachment = 0;                                        // アタッチメントのインデックス
    m_ColorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal; // アタッチメントのレイアウト

    // サブパスの記述
    m_Subpass.flags = {};
    m_Subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;  // グラフィックスパイプラインにバインド
    m_Subpass.inputAttachmentCount;
    m_Subpass.pInputAttachments = nullptr;
    m_Subpass.colorAttachmentCount = 1;  // カラーバッファアタッチメントの数
    m_Subpass.pColorAttachments = &m_ColorAttachmentRef;  // カラーバッファアタッチメントの参照
    m_Subpass.pResolveAttachments = nullptr;
    m_Subpass.pDepthStencilAttachment = nullptr;
    m_Subpass.preserveAttachmentCount;
    m_Subpass.pPreserveAttachments = nullptr;

    return &m_Subpass;
}

std::vector<vk::SubpassDependency>* RenderpassGenerator::CreateDependencies()
{
    // サブパス依存関係の記述
    // レイアウト遷移が発生するタイミングをサブパス依存関係を使用して決定する必要があります
    m_Dependencies.resize(2);
    // VK_IMAGE_LAYOUT_UNDEFINED から VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL への変換
    // 遷移は次の後で発生する必要があります...
    m_Dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                       // サブパスのインデックス (VK_SUBPASS_EXTERNAL = レンダーパスの外部を意味する特別な値)
    m_Dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // パイプラインステージ
    m_Dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;        // ステージアクセスマスク (メモリアクセス)
    // しかし、次の前で発生する必要があります...
    m_Dependencies[0].dstSubpass = 0;                                         // サブパスのインデックス
    m_Dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // パイプラインステージ
    m_Dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // ステージアクセスマスク
    m_Dependencies[0].dependencyFlags = vk::DependencyFlags();                // 依存関係フラグ
    
    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL から VK_IMAGE_LAYOUT_PRESENT_SRC_KHR への変換
    // 遷移は次の後で発生する必要があります...
    m_Dependencies[1].srcSubpass = 0;                                         // サブパスのインデックス
    m_Dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // パイプラインステージ
    m_Dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // ステージアクセスマスク
    // しかし、次の前で発生する必要があります...
    m_Dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                       // サブパスのインデックス
    m_Dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // パイプラインステージ
    m_Dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;        // ステージアクセスマスク
    m_Dependencies[1].dependencyFlags = vk::DependencyFlags();                // 依存関係フラグ

    return &m_Dependencies;
}

vk::RenderPassCreateInfo RenderpassGenerator::CreateInfo(vk::AttachmentDescription* colorAttachment, vk::SubpassDescription* subpass, std::vector<vk::SubpassDependency>* dependencies)
{
    // レンダーパスの記述
    m_RenderPassInfo.pNext = nullptr;
    m_RenderPassInfo.flags = {};
    m_RenderPassInfo.attachmentCount = 1;                // アタッチメントの数
    m_RenderPassInfo.pAttachments = colorAttachment; // アタッチメントの記述
    m_RenderPassInfo.subpassCount = 1;                // サブパスの数
    m_RenderPassInfo.pSubpasses = subpass;         // サブパスの記述
    m_RenderPassInfo.dependencyCount = (uint32_t)dependencies->size();   // サブパス依存関係の数
    m_RenderPassInfo.pDependencies = dependencies->data();      // サブパス依存関係の記述

    return m_RenderPassInfo;
}
