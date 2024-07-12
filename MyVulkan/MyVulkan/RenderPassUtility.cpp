#include "RenderPassUtility.h"
using namespace SwapChainUtility;

vk::UniqueRenderPass RenderPassUtility::createRenderPass(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    vk::Format              swapChainImageFormat    = chooseSwapSurfaceFormat(getSurfaceFormats(physicalDevice, surface)).format;

    // カラーバッファアタッチメントの記述
    vk::AttachmentDescription colorAttachment = {};
    colorAttachment.format          = swapChainImageFormat;             // スワップチェインのイメージフォーマット
    colorAttachment.samples         = vk::SampleCountFlagBits::e1;      // マルチサンプリングのサンプル数
    colorAttachment.loadOp          = vk::AttachmentLoadOp::eClear;     // レンダーパスの開始時にカラーバッファをクリア
    colorAttachment.storeOp         = vk::AttachmentStoreOp::eStore;    // レンダーパスの終了時にカラーバッファを保存
    colorAttachment.stencilLoadOp   = vk::AttachmentLoadOp::eDontCare;  // ステンシルバッファを使用しない
    colorAttachment.stencilStoreOp  = vk::AttachmentStoreOp::eDontCare; // ステンシルバッファを使用しない
    colorAttachment.initialLayout   = vk::ImageLayout::eUndefined;      // レンダーパス開始前のレイアウト
    colorAttachment.finalLayout     = vk::ImageLayout::ePresentSrcKHR;  // レンダーパス終了後のレイアウト（表示用）

    // アタッチメント参照
    vk::AttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment   = 0;  // アタッチメントのインデックス
    colorAttachmentRef.layout       = vk::ImageLayout::eColorAttachmentOptimal;  // アタッチメントのレイアウト

    // サブパスの記述
    vk::SubpassDescription subpass = {};
    subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;  // グラフィックスパイプラインにバインド
    subpass.colorAttachmentCount    = 1;  // カラーバッファアタッチメントの数
    subpass.pColorAttachments       = &colorAttachmentRef;  // カラーバッファアタッチメントの参照

    // サブパス依存関係の記述
    vk::SubpassDependency dependency = {};
    dependency.srcSubpass           = VK_SUBPASS_EXTERNAL;  // 外部サブパス（レンダーパスの前）
    dependency.dstSubpass           = 0;  // 最初のサブパス
    dependency.srcStageMask         = vk::PipelineStageFlagBits::eColorAttachmentOutput;  // カラーバッファの書き込みステージ
    dependency.srcAccessMask        = vk::AccessFlagBits::eNone;  // アクセスマスクなし
    dependency.dstStageMask         = vk::PipelineStageFlagBits::eColorAttachmentOutput;  // カラーバッファの書き込みステージ
    dependency.dstAccessMask        = vk::AccessFlagBits::eColorAttachmentWrite;  // カラーバッファの書き込みアクセス

    // レンダーパスの記述
    vk::RenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.attachmentCount  = 1;                // アタッチメントの数
    renderPassInfo.pAttachments     = &colorAttachment; // アタッチメントの記述
    renderPassInfo.subpassCount     = 1;                // サブパスの数
    renderPassInfo.pSubpasses       = &subpass;         // サブパスの記述
    renderPassInfo.dependencyCount  = 1;                // サブパス依存関係の数
    renderPassInfo.pDependencies    = &dependency;      // サブパス依存関係の記述

    // レンダーパスを作成
    vk::UniqueRenderPass renderPass = logicalDevice.createRenderPassUnique(renderPassInfo);
    if (!renderPass)
    {
        throw std::runtime_error("レンダーパスの作成に失敗しました!");
    }

    return renderPass;
}
