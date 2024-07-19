#include "RenderPassUtility.h"
using namespace VulkanUtility;

vk::RenderPassCreateInfo VulkanCreate::GetRenderPassInfo(vk::SwapchainCreateInfoKHR* swapchainInfo)
{
    // カラーバッファアタッチメントの記述
    static vk::AttachmentDescription colorAttachment;
    colorAttachment.flags           = {};
    colorAttachment.format          = swapchainInfo->imageFormat;       // スワップチェインのイメージフォーマット
    colorAttachment.samples         = vk::SampleCountFlagBits::e1;      // マルチサンプリングのサンプル数
    colorAttachment.loadOp          = vk::AttachmentLoadOp::eClear;     // レンダーパスの開始時にカラーバッファをクリア
    colorAttachment.storeOp         = vk::AttachmentStoreOp::eStore;    // レンダーパスの終了時にカラーバッファを保存
    colorAttachment.stencilLoadOp   = vk::AttachmentLoadOp::eDontCare;  // ステンシルバッファを使用しない
    colorAttachment.stencilStoreOp  = vk::AttachmentStoreOp::eDontCare; // ステンシルバッファを使用しない
    colorAttachment.initialLayout   = vk::ImageLayout::eUndefined;      // レンダーパス開始前のレイアウト
    colorAttachment.finalLayout     = vk::ImageLayout::ePresentSrcKHR;  // レンダーパス終了後のレイアウト（表示用）

    // アタッチメント参照
    static vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment   = 0;                                        // アタッチメントのインデックス
    colorAttachmentRef.layout       = vk::ImageLayout::eColorAttachmentOptimal; // アタッチメントのレイアウト

    // サブパスの記述
    static vk::SubpassDescription subpass;
    subpass.flags                   = {};
    subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;  // グラフィックスパイプラインにバインド
    subpass.inputAttachmentCount;
    subpass.pInputAttachments = nullptr;
    subpass.colorAttachmentCount    = 1;  // カラーバッファアタッチメントの数
    subpass.pColorAttachments       = &colorAttachmentRef;  // カラーバッファアタッチメントの参照
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = nullptr;
    subpass.preserveAttachmentCount;
    subpass.pPreserveAttachments = nullptr;

    // サブパス依存関係の記述
    // レイアウト遷移が発生するタイミングをサブパス依存関係を使用して決定する必要があります
    static std::array<vk::SubpassDependency, 2> dependencies;

    // VK_IMAGE_LAYOUT_UNDEFINED から VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL への変換
    // 遷移は次の後で発生する必要があります...
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                       // サブパスのインデックス (VK_SUBPASS_EXTERNAL = レンダーパスの外部を意味する特別な値)
    dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // パイプラインステージ
    dependencies[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;        // ステージアクセスマスク (メモリアクセス)
    // しかし、次の前で発生する必要があります...
    dependencies[0].dstSubpass = 0;                                         // サブパスのインデックス
    dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // パイプラインステージ
    dependencies[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // ステージアクセスマスク
    dependencies[0].dependencyFlags = vk::DependencyFlags();                // 依存関係フラグ

    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL から VK_IMAGE_LAYOUT_PRESENT_SRC_KHR への変換
    // 遷移は次の後で発生する必要があります...
    dependencies[1].srcSubpass = 0;                                         // サブパスのインデックス
    dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput; // パイプラインステージ
    dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite; // ステージアクセスマスク
    // しかし、次の前で発生する必要があります...
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                       // サブパスのインデックス
    dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe; // パイプラインステージ
    dependencies[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;        // ステージアクセスマスク
    dependencies[1].dependencyFlags = vk::DependencyFlags();                // 依存関係フラグ


    // レンダーパスの記述
    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags = {};
    renderPassInfo.attachmentCount  = 1;                // アタッチメントの数
    renderPassInfo.pAttachments = colorAttachment.get(); // アタッチメントの記述
    renderPassInfo.subpassCount     = 1;                // サブパスの数
    renderPassInfo.pSubpasses       = &subpass;         // サブパスの記述
    renderPassInfo.dependencyCount  = (uint32_t)dependencies.size();   //             // サブパス依存関係の数
    renderPassInfo.pDependencies    = dependencies.data();      // サブパス依存関係の記述



    return renderPassInfo;


}
    // レンダーパスを作成
    //vk::UniqueRenderPass renderPass = logicalDevice.createRenderPassUnique(renderPassInfo);
    //if (!renderPass)
    //{
    //    throw std::runtime_error("レンダーパスの作成に失敗しました!");
    //}

    //return renderPass;