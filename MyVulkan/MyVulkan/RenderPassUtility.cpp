#include "RenderPassUtility.h"

RenderpassGenerator::RenderpassGenerator()
{
    m_ClassName = "RenderpassGenerator";
}

RenderpassGenerator::~RenderpassGenerator()
{

}

void RenderpassGenerator::LoadShader(vk::Device logicalDevice, vk::Format imageFomat)
{
    m_bCreated = true;
    m_LogicalDevice = logicalDevice;

    m_RenderPass = CreateRenderpass(logicalDevice, imageFomat);
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


vk::RenderPass RenderpassGenerator::CreateRenderpass(vk::Device logicalDevice, vk::Format imageFormat)
{
    // アタッチメントの設定
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.setFormat(chooseSupportedFormat(
        { vk::Format::eR8G8B8A8Unorm },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eColorAttachment));
    colorAttachment.setSamples(vk::SampleCountFlagBits::e1); // マルチサンプリングのサンプル数
    colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear); // レンダーパス開始時にクリア
    colorAttachment.setStoreOp(vk::AttachmentStoreOp::eDontCare); // 終了時に保存しない
    colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare); // ステンシルを使用しない
    colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined); // 開始時のレイアウト
    colorAttachment.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal); // 終了時のレイアウト

    vk::AttachmentDescription depthAttachment = {};
    depthAttachment.setFormat(chooseSupportedFormat(
        { vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint },
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment));
    depthAttachment.setSamples(vk::SampleCountFlagBits::e1);
    depthAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
    depthAttachment.setStoreOp(vk::AttachmentStoreOp::eDontCare);
    depthAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    depthAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    depthAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
    depthAttachment.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    // カラーバッファの参照を設定
    vk::AttachmentReference colorAttachmentReference = {};
    colorAttachmentReference.setAttachment(1); // アタッチメントのインデックス
    colorAttachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    // デプスバッファの参照を設定
    vk::AttachmentReference depthAttachmentReference = {};
    depthAttachmentReference.setAttachment(2);
    depthAttachmentReference.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    // サブパス1の設定
    vk::SubpassDescription subpass1 = {};
    subpass1.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpass1.setColorAttachmentCount(1);
    subpass1.setPColorAttachments(&colorAttachmentReference); // カラーアタッチメントの参照
    subpass1.setPDepthStencilAttachment(&depthAttachmentReference); // デプスアタッチメントの参照

    // スワップチェイン用のカラーバッファアタッチメント
    vk::AttachmentDescription swapchainColorAttachment = {};
    swapchainColorAttachment.setFormat(imageFormat); // スワップチェインのフォーマット
    swapchainColorAttachment.setSamples(vk::SampleCountFlagBits::e1);
    swapchainColorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
    swapchainColorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore); // レンダーパス後に保存
    swapchainColorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    swapchainColorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    swapchainColorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
    swapchainColorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR); // プレゼントのためのレイアウト

    // スワップチェインのカラーバッファの参照を設定
    vk::AttachmentReference swapchainColorAttachmentReference = {};
    swapchainColorAttachmentReference.setAttachment(0);
    swapchainColorAttachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    // サブパス2の入力アタッチメントの参照を設定
    std::array<vk::AttachmentReference, 2> inputReferences = {
        vk::AttachmentReference{1, vk::ImageLayout::eShaderReadOnlyOptimal}, // サブパス1のカラーアタッチメント
        vk::AttachmentReference{2, vk::ImageLayout::eShaderReadOnlyOptimal}  // サブパス1のデプスアタッチメント
    };

    // サブパス2の設定
    vk::SubpassDescription subpass2 = {};
    subpass2.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
    subpass2.setColorAttachmentCount(1);
    subpass2.setPColorAttachments(&swapchainColorAttachmentReference); // スワップチェインのカラーアタッチメント
    subpass2.setInputAttachmentCount(static_cast<uint32_t>(inputReferences.size()));
    subpass2.setPInputAttachments(inputReferences.data()); // 入力アタッチメントの参照

    // サブパス依存関係の設定
    std::array<vk::SubpassDependency, 3> subpassDependencies;

    // 外部からの色アタッチメントへのレイアウト変換
    subpassDependencies[0] = vk::SubpassDependency{
        vk::SubpassExternal,
        0,
        vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::AccessFlagBits::eMemoryRead,
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
        {}
    };

    // サブパス1からサブパス2へのレイアウト変換
    subpassDependencies[1] = vk::SubpassDependency{
        0,
        1,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eShaderRead,
        {}
    };

    // サブパス1から外部へのレイアウト変換
    subpassDependencies[2] = vk::SubpassDependency{
        0,
        vk::SubpassExternal,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eMemoryRead,
        {}
    };

    // レンダーパスのアタッチメントを設定
    std::array<vk::AttachmentDescription, 3> renderPassAttachments = { swapchainColorAttachment, colorAttachment, depthAttachment };

    // レンダーパス作成情報の設定
    vk::RenderPassCreateInfo renderPassCreateInfo = {};
    renderPassCreateInfo.setAttachmentCount(static_cast<uint32_t>(renderPassAttachments.size()));
    renderPassCreateInfo.setPAttachments(renderPassAttachments.data());
    renderPassCreateInfo.setSubpassCount(static_cast<uint32_t>(2));
    std::array<vk::SubpassDescription, 2> subpasses = { subpass1, subpass2 };
    renderPassCreateInfo.setPSubpasses(subpasses.data());
    renderPassCreateInfo.setDependencyCount(static_cast<uint32_t>(subpassDependencies.size()));
    renderPassCreateInfo.setPDependencies(subpassDependencies.data());

    // レンダーパスの作成
    auto renderPass = logicalDevice.createRenderPass(renderPassCreateInfo);

    // エラーチェック
    if (!renderPass) {
        throw std::runtime_error("Failed to create a Render Pass!"); // レンダーパス作成失敗時
    }

    return renderPass; // 作成したレンダーパスを返す
}
