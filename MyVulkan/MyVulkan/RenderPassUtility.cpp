#include "RenderPassUtility.h"

RenderpassGenerator::RenderpassGenerator()
{
    m_ClassName = "RenderpassGenerator";
}

RenderpassGenerator::~RenderpassGenerator()
{

}

void RenderpassGenerator::Create(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    auto colorAttachment = CreateColorAttachment(imageFormat);
    auto subpass = CreateSubpass();
    auto dependencies = CreateDependencies();

    auto createInfo = CreateInfo(colorAttachment, subpass, dependencies);
    m_RenderPass = logicalDevice.createRenderPass(createInfo);
    //if (!renderPass)
    //{
    //    throw std::runtime_error("レンダーパスの作成に失敗しました!");
    //}
}

void RenderpassGenerator::Destroy()
{
    //作成フラグが立っていない場合は解放処理も行わない
    if (m_bCreated == false) return;
    m_bCreated = false;

    // レンダーパスの解放
    m_LogicalDevice.destroyRenderPass(m_RenderPass);
}

vk::RenderPass RenderpassGenerator::GetRenderpass()
{
    CheckCreated();
    return m_RenderPass;
}

//void RenderpassGenerator::Release()
//{
    //m_pLogicalDevice->destroyRenderPass(m_RenderPass, nullptr);
//}

std::vector<vk::AttachmentDescription> RenderpassGenerator::CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat)
{
    vk::AttachmentDescription attachment;
    // カラーバッファアタッチメントの記述
    attachment.flags;
    attachment.format = imageFormat.format;                          // 画像フォーマット(画像作成時の設定と同じにする)
    attachment.samples = vk::SampleCountFlagBits::e1;                // マルチサンプリングのサンプル数
    attachment.loadOp = vk::AttachmentLoadOp::eClear;                // レンダーパスの開始時にカラーバッファをクリア
    attachment.storeOp = vk::AttachmentStoreOp::eStore;              // レンダーパスの終了時にカラーバッファを保存
    attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;      // ステンシルバッファを使用しない
    attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;    // ステンシルバッファを使用しない
    attachment.initialLayout = vk::ImageLayout::eUndefined;          // レンダーパス開始前のレイアウト
    attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;        // レンダーパス終了後のレイアウト（表示用）
    
    return std::vector{ attachment };
}

vk::AttachmentReference RenderpassGenerator::CreateColorAttachmentReference()
{
    vk::AttachmentReference reference;
    // サブパスから描画結果の色を出力するアタッチメントがどれかを指定します
    reference.attachment = 0;                                    // アタッチメントのインデックス
    reference.layout = vk::ImageLayout::eColorAttachmentOptimal; // アタッチメントのレイアウト
    return reference;
}

std::vector<vk::SubpassDescription> RenderpassGenerator::CreateSubpass(
    std::vector<vk::AttachmentReference> inputReferences = {},
    std::vector<vk::AttachmentReference> colorReferences = {},
    std::vector<vk::AttachmentReference> resolveReferences = {},
    std::vector<vk::AttachmentReference> depthStencilReferences = {},
    std::vector<vk::AttachmentReference> preserveReferences = {})
{
    // アタッチメント参照
    // サブパスから描画結果の色を出力するアタッチメントがどれかを指定します
    //m_ColorAttachmentRef.attachment = 0;                                    // アタッチメントのインデックス
    //m_ColorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal; // アタッチメントのレイアウト

    // サブパスの記述
    vk::SubpassDescription subpass;
    subpass.flags;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;  // グラフィックスパイプラインにバインド
    subpass.inputAttachmentCount;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = colorReferences.size();  // カラーバッファアタッチメントの数
    subpass.pColorAttachments = colorReferences.data();  // カラーバッファアタッチメントの参照
    subpass.resolveAttachmentCount
    subpass.pResolveAttachments = resolveReferences.data();
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount = preserveReferences.size();
    subpass.pPreserveAttachments = preserveReferences.data();

    return std::vector{ subpass };
}

std::vector<vk::SubpassDependency> RenderpassGenerator::CreateDependencies()
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
    m_Dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                         // サブパスのインデックス
    m_Dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;  // パイプラインステージ
    m_Dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;          // ステージアクセスマスク
    m_Dependencies[1].dependencyFlags = vk::DependencyFlags();                  // 依存関係フラグ

    return m_Dependencies;
}

vk::RenderPassCreateInfo RenderpassGenerator::CreateInfo(
    std::vector<vk::AttachmentDescription>& colorAttachment, 
    std::vector<vk::SubpassDescription>& subpass, 
    std::vector<vk::SubpassDependency>& dependencies)
{
    // レンダーパスの記述
    m_RenderPassInfo.pNext;
    m_RenderPassInfo.flags;
    m_RenderPassInfo.attachmentCount    = colorAttachment.size();   // アタッチメントの数
    m_RenderPassInfo.pAttachments       = colorAttachment.data();   // アタッチメントの記述
    m_RenderPassInfo.subpassCount       = subpass.size();           // サブパスの数
    m_RenderPassInfo.pSubpasses         = subpass.data();           // サブパスの記述
    m_RenderPassInfo.dependencyCount    = dependencies.size();      // サブパス依存関係の数
    m_RenderPassInfo.pDependencies      = dependencies.data();      // サブパス依存関係の記述

    return m_RenderPassInfo;
}
