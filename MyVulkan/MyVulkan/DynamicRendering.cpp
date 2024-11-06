#include "DynamicRendering.h"

VDynamicRenderingCommand::VDynamicRenderingCommand(DeviceExtensionManager& deviceExtensionManager)
{
	deviceExtensionManager.UseDynamicRendering();
}

VDynamicRenderingCommand::~VDynamicRenderingCommand()
{
}

void VDynamicRenderingCommand::RenderWithDynamicRendering(
    vk::ImageView               colorImageView,
    vk::Extent2D                extent, 
    vk::Pipeline                graphicsPipeline, 
    vk::PipelineLayout          pipelineLayout,
    std::vector<SceneObject>    drawModels, 
    SceneCamera                 sceneCamera)
{
    // 1. カラーアタッチメント情報の設定
    vk::ClearValue clearColor = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}); // 黒色でクリア

    vk::RenderingAttachmentInfo colorAttachmentInfo;
    colorAttachmentInfo
        .setImageView(colorImageView)                                // カラーイメージを指定
        .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)    // レイアウトを最適化
        .setLoadOp(vk::AttachmentLoadOp::eClear)                     // 描画前にクリア
        .setStoreOp(vk::AttachmentStoreOp::eStore)                   // 描画後に結果を保持
        .setClearValue(clearColor);                                  // クリアカラー設定

    // 2. レンダリング情報の設定
    vk::RenderingInfo renderingInfo;
    
    renderingInfo.setRenderArea(vk::Rect2D({ 0, 0 }, extent));  // 描画エリア
    renderingInfo.setLayerCount(1);                             // レイヤー数
    renderingInfo.setColorAttachments(colorAttachmentInfo);     // カラーアタッチメント情報を指定

    // 3. Dynamic Renderingの開始
    m_CommandBuffer.beginRendering(renderingInfo);

    // ここでグラフィックスパイプラインをバインドして描画を行います
    m_CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

    // 描画するメッシュをループします。
    for (auto& model : drawModels)
    {
        // プッシュ定数をシェーダーに渡します。
        m_CommandBuffer.pushConstants(
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
            m_CommandBuffer.bindVertexBuffers(0, mesh.GetVertex().GetBuffer(), { 0 });

            // ディスクリプタセットをバインドします。
            std::array<vk::DescriptorSet, 2> descriptorSetGroup =
            {

                //descriptorSets[currentImage], //たぶんカメラ情報が入ってる(uboViewProjection)
                //samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
                sceneCamera.GetDescriptorSet(),             // 
                model.GetMaterials()[0].GetDescriptorSet()  // テクスチャ画像のディスクプリタ
            };

            m_CommandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipelineLayout,
                0,
                descriptorSetGroup,
                nullptr
            );

            // インデックスバッファ(頂点を結ぶ順番の値)を結び付けます。
            m_CommandBuffer.bindIndexBuffer(mesh.GetIndex().GetBuffer(), 0, vk::IndexType::eUint32);
            m_CommandBuffer.drawIndexed(mesh.GetIndex().GetSize(), 1, 0, 0, 0);   // インデックスに従って描画
        }


    // 4. Dynamic Renderingの終了
    m_CommandBuffer.endRendering();
}
