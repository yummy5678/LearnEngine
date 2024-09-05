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

    //auto colorAttachment = CreateColorAttachment(imageFormat);
    //auto colorAttachmentReference = CreateColorAttachmentReference();
    //auto subpass = CreateSubpass({}, { colorAttachmentReference }, {}, {}, {});
    //auto dependencies = CreateDependencies();
    //std::vector<vk::AttachmentDescription> a{ colorAttachment };
    //auto createInfo = CreateInfo(a, subpass, dependencies);
    //m_RenderPass = logicalDevice.createRenderPass(createInfo);
    m_RenderPass = CreateRenderpass(logicalDevice);
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

vk::AttachmentDescription RenderpassGenerator::CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat)
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
    attachment.initialLayout = vk::ImageLayout::eUndefined;          // レンダーパス開始時のレイアウト
    attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;        // レンダーパス終了時のレイアウト(表示用)
    
    return attachment;
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
    std::vector<vk::AttachmentReference> inputReferences,
    std::vector<vk::AttachmentReference> colorReferences,
    vk::AttachmentReference resolveReferences,
    vk::AttachmentReference depthStencilReferences,
    std::vector<uint32_t> preserveReferences)
{
    // アタッチメント参照
    // サブパスから描画結果の色を出力するアタッチメントがどれかを指定します
    m_ColorAttachmentRef.attachment = 0;                                    // アタッチメントのインデックス
    m_ColorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal; // アタッチメントのレイアウト

    // サブパスの記述
    vk::SubpassDescription subpass;
    subpass.flags;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;  // グラフィックスパイプラインにバインド
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount = 1;  // カラーバッファアタッチメントの数
    subpass.pColorAttachments = &m_ColorAttachmentRef;  // カラーバッファアタッチメントの参照
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

    return std::vector{ subpass };
}

std::vector<vk::SubpassDependency> RenderpassGenerator::CreateDependencies()
{

    std::vector<vk::SubpassDependency> dependencies;
    dependencies.resize(2); 

    // サブパス依存関係の記述
    // レイアウト遷移が発生するタイミングをサブパス依存関係を使用して決定する必要があります
    
    // VK_IMAGE_LAYOUT_UNDEFINED から VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL への変換
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                           // サブパスのインデックス (VK_SUBPASS_EXTERNAL = レンダーパスの外部を意味する特別な値)
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;    // パイプラインステージ
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;            // ステージアクセスマスク (メモリアクセス)
    dependencies[0].dstSubpass = 0;                                             // サブパスのインデックス
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // パイプラインステージ
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // ステージアクセスマスク
    dependencies[0].dependencyFlags = vk::DependencyFlags();                    // 依存関係フラグ
    
    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL から VK_IMAGE_LAYOUT_PRESENT_SRC_KHR への変換
    dependencies[1].srcSubpass = 0;                                             // サブパスのインデックス
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // パイプラインステージ
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // ステージアクセスマスク
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                           // サブパスのインデックス
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;    // パイプラインステージ
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;            // ステージアクセスマスク
    dependencies[1].dependencyFlags = vk::DependencyFlags();                    // 依存関係フラグ

    return dependencies;
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

vk::RenderPass RenderpassGenerator::CreateRenderpass(vk::Device logicalDevice)
{
    vk::AttachmentDescription attachments[1];
    attachments[0].format = vk::Format::eR8G8B8A8Unorm;
    attachments[0].samples = vk::SampleCountFlagBits::e1;
    attachments[0].loadOp = vk::AttachmentLoadOp::eDontCare;
    attachments[0].storeOp = vk::AttachmentStoreOp::eStore;
    attachments[0].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachments[0].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachments[0].initialLayout = vk::ImageLayout::eUndefined;
    attachments[0].finalLayout = vk::ImageLayout::eGeneral;

    vk::AttachmentReference subpass0_attachmentRefs[1];
    subpass0_attachmentRefs[0].attachment = 0;
    subpass0_attachmentRefs[0].layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::SubpassDescription subpasses[1];
    subpasses[0].pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpasses[0].colorAttachmentCount = 1;
    subpasses[0].pColorAttachments = subpass0_attachmentRefs;

    vk::RenderPassCreateInfo renderpassCreateInfo;
    renderpassCreateInfo.attachmentCount = 1;
    renderpassCreateInfo.pAttachments = attachments;
    renderpassCreateInfo.subpassCount = 1;
    renderpassCreateInfo.pSubpasses = subpasses;
    renderpassCreateInfo.dependencyCount = 0;
    renderpassCreateInfo.pDependencies = nullptr;

    return logicalDevice.createRenderPass(renderpassCreateInfo);
}
