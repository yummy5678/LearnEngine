#include "RenderConfig.h"


RenderConfig::RenderConfig(VulkanInitializer& initializer):
    RendererBase(initializer),
    m_Shader(), 
    m_RenderFunction(),
    m_pObjects(),
    m_pCamera()
{
    m_pLogicalDevice = initializer.GetPLogicalDevice();
    m_pPhygicalDevice = initializer.GetPPhysicalDevice();

}

RenderConfig::~RenderConfig()
{
}

void RenderConfig::Initialize(RenderingTarget* renderere)
{
    vk::Format colorFomat = renderere->GetColorFormat();
    vk::Format depthFomat = renderere->GetDepthFormat();
    vk::Extent2D extent = renderere->GetExtent();

    m_Shader.Create(m_pLogicalDevice);
    m_RenderArea.setOffset({ 0, 0 });
    m_RenderArea.setExtent(extent);

    // m_CameraDescriptor.UpdateDescriptorSets(camera->GetProjectionBuffer());
    // m_TextureDescriptors.UpdateDescriptorSets();


    auto descriptorSetLayouts   = m_Shader.GetDescriptorSetLayouts();
    auto constantRanges         = m_Shader.GetPushConstantRanges();
    auto vertexInputState       = m_Shader.GetVertexInputState();

    m_GraphicsPipeline.Create(
        extent,
        m_RenderArea,
        colorFomat,
        depthFomat,
        &vertexInputState,
        m_Shader.GetShaderStages(),
        descriptorSetLayouts,
        constantRanges);

    // モジュールはパイプライン作成後は不要になる
    m_Shader.DestroyModule();
}


vk::Rect2D RenderConfig::GetSissorRect()
{
    return vk::Rect2D(m_Offset, m_Extent);
}

vk::Rect2D RenderConfig::GetRenderRect()
{
    return m_RenderArea;
}

vk::Pipeline RenderConfig::GetPipeline()
{
    return m_GraphicsPipeline.GetPipeline();
}

vk::PipelineLayout RenderConfig::GetPipelineLayout()
{
    return m_GraphicsPipeline.GetPipelineLayout();
}

//VShaderConfigureBase* RenderConfig::GetPShaderConfiguer()
//{
//    return &m_Shader;
//}

std::shared_ptr<RenderFunction> RenderConfig::GetRenderFunction(RenderingObjects* pObjects, SceneCamera* pCamera)
{
    // ToDo : 必要なくなったm_RenderFunctionとそれに付随するオブジェクトのリンクを削除


    /*std::function<void(vk::CommandBuffer commandBuffer)>;*/
    std::shared_ptr<RenderFunction> result = std::make_shared<RenderFunction>(
        [this](vk::CommandBuffer commandBuffer, void* key)
        {
            // NULLチェック
            if (commandBuffer == VK_NULL_HANDLE || m_pObjects[key] == nullptr || m_pCamera[key] == nullptr)
            {
                return;
            }

            vk::DescriptorSet cameraDescriptorSet = m_pCamera[key]->GetDescriptorSet(m_Shader.GetCameraDescriptorLayout());

            auto pipeline = m_GraphicsPipeline.GetPipeline();
            auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();

            // 使用するパイプラインをバインドします。
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

            for (auto& object : *m_pObjects[key])
            {
                for (auto& mesh : *object->GetMeshes())
                {
                    //mesh.GetSPMaterial().get()->GetDescriptorSet(m_Shader.GetTextureDescriptorLayout());

                    // プッシュ定数をシェーダーに渡します。
                    commandBuffer.pushConstants(
                        pipelineLayout,
                        vk::ShaderStageFlagBits::eVertex,   // プッシュ定数を更新するシェーダーステージ
                        0,                                  // オフセット
                        sizeof(Transform),                  // プッシュするデータのサイズ
                        object->GetPTransform()              // 実際のデータ
                    );

                    auto vertexBuffer = mesh.GetSPMesh()->GetVertex();
                    auto indexBuffer = mesh.GetSPMesh()->GetIndex();


                    // 頂点バッファをバインド
                    commandBuffer.bindVertexBuffers(0, vertexBuffer.GetBuffer(), { 0 });

                    commandBuffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        pipelineLayout,
                        0,
                        { 
                            cameraDescriptorSet,
                            mesh.GetDescriptorSet(m_Shader.GetTextureDescriptorLayout())
                        },
                        nullptr);

                    // インデックスバッファ(頂点を結ぶ順番の値)を結び付けます。
                    commandBuffer.bindIndexBuffer(indexBuffer.GetBuffer(), 0, vk::IndexType::eUint32);
                    commandBuffer.drawIndexed(indexBuffer.GetSize(), 1, 0, 0, 0);   // インデックスに従って描画

                }
            }
        });
    m_RenderFunction.push_back(result);
    m_pObjects[result.get()] = pObjects;
    m_pCamera[result.get()] = pCamera;
    return result;
}

//void RenderConfig::BindRenderingCommand(
//    vk::CommandBuffer command,
//    std::vector<RenderObject*>* pObjects, 
//    SceneCamera* pCamera)
//{
//     NULLチェック
//    if (command == VK_NULL_HANDLE || pObjects == nullptr || pCamera == nullptr)
//        return;
//
//    auto pipeline = m_GraphicsPipeline.GetPipeline();
//    auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();
//
//     使用するパイプラインをバインドします。
//    command.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
//
//    for (auto& object : *pObjects)
//    {
//        for (auto& mesh : *object->GetMeshes())
//        {
//            mesh.GetSPMaterial().get()->GetDescriptorSet(m_Shader.GetTextureDescriptorLayout());
//
//             プッシュ定数をシェーダーに渡します。
//            command.pushConstants(
//                pipelineLayout,
//                vk::ShaderStageFlagBits::eVertex,   // プッシュ定数を更新するシェーダーステージ
//                0,                                  // オフセット
//                sizeof(Transform),                  // プッシュするデータのサイズ
//                object->GetPTransform()              // 実際のデータ
//            );
//
//            auto vertexBuffer = mesh.GetSPMesh()->GetVertex();
//            auto indexBuffer = mesh.GetSPMesh()->GetIndex();
//
//
//             頂点バッファをバインド
//            command.bindVertexBuffers(0, vertexBuffer.GetBuffer(), {0});
//
//            command.bindDescriptorSets(
//                vk::PipelineBindPoint::eGraphics,
//                pipelineLayout,
//                0,
//                m_Shader.GetDescriptorSets(),
//                nullptr);
//
//             インデックスバッファ(頂点を結ぶ順番の値)を結び付けます。
//            command.bindIndexBuffer(indexBuffer.GetBuffer(), 0, vk::IndexType::eUint32);
//            command.drawIndexed(indexBuffer.GetSize(), 1, 0, 0, 0);   // インデックスに従って描画
//
//        }
//    }
//}


//std::vector<vk::DescriptorSet> RenderConfig::GetDescriptorSets()
//{
//    // ディスクリプタセットをバインド
//    std::vector<vk::DescriptorSet> descriptorSets;
//    descriptorSets.push_back(m_CameraDescriptor.GetPDescriptorSets());
//    descriptorSets.push_back(m_TextureDescriptors.GetPDescriptorSets());
//
//    return descriptorSets;
//}

//std::vector<vk::DescriptorSetLayout> RenderConfig::GetDescriptorSetLayouts()
//{
//    // ディスクリプタセットをバインド
//    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
//    descriptorSetLayouts.push_back(m_CameraDescriptor.GetDescriptorSetLayout());
//    descriptorSetLayouts.push_back(m_TextureDescriptors.GetDescriptorSetLayout());
//
//    return descriptorSetLayouts;
//}

//std::vector<vk::PushConstantRange> RenderConfig::GetPushConstantRanges()
//{
//    return { GetPushConstantModelRange() };
//}

//void RenderConfig::DrawImage(std::vector<RenderObject>* objects, SceneCamera* camera)
//{
//    auto pipeline = m_GraphicsPipeline.GetPipeline();
//    auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();
//    auto descriptorSets = GetDescriptorSets();
//
//    m_DrawCommand.BeginRendering(pipeline, m_RenderArea);
//
//    m_CameraDescriptor.UpdateDescriptorSets(camera->GetProjectionBuffer());
//
//
//    for (auto& model : *objects)
//    {
//        for (auto& mesh : *model.GetMeshes())
//        {
//            m_TextureDescriptors.UpdateDescriptorSets(mesh.GetSPMaterial()->GetTextureImageView(), mesh.GetSPMaterial()->GetSampler());
//            m_DrawCommand.RenderMesh(m_GraphicsPipeline.GetPipelineLayout(), &descriptorSets, &mesh, model.GetPTransform());
//        }
//    }
//
//    m_DrawCommand.EndRendering();
//}

//vk::PushConstantRange RenderConfig::GetPushConstantModelRange()
//{
//    return vk::PushConstantRange
//    {
//        vk::ShaderStageFlagBits::eVertex,	// 渡したいシェーダーステージ
//        0,								    // 渡したデータからどの位置のデータを見るか
//        sizeof(Transform)					// 渡したいデータのサイズ
//    };
//}

//vk::PipelineVertexInputStateCreateInfo RenderConfig::GetVertexInputState()
//{
//    // バインディングの定義   //後でテンプレート化する
//    m_BindingDescriptions =
//    {
//        vk::VertexInputBindingDescription
//        {
//           0,                              // binding  
//           sizeof(Vertex),                 // stride   
//           vk::VertexInputRate::eVertex    // inputRate
//        }
//    };
//
//    // 入力属性の定義
//    m_AttributeDescriptions = {
//        // 座標
//      vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },
//
//       // 法線
//      vk::VertexInputAttributeDescription{ 1, 0, vk::Format::eR32G32B32Sfloat,  offsetof(Vertex, normal) },
//
//      // テクスチャ座標
//      vk::VertexInputAttributeDescription{ 2, 0, vk::Format::eR32G32Sfloat,     offsetof(Vertex, textureCoord) }
//    };
//
//    // パイプライン頂点入力状態の作成
//    vk::PipelineVertexInputStateCreateInfo resultInfo;
//    resultInfo.pVertexBindingDescriptions = m_BindingDescriptions.data();
//    resultInfo.vertexBindingDescriptionCount = m_BindingDescriptions.size();
//    resultInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
//    resultInfo.vertexAttributeDescriptionCount = m_AttributeDescriptions.size();
//
//    m_InputStateInfo.setVertexAttributeDescriptions(m_AttributeDescriptions);
//    m_InputStateInfo.setVertexBindingDescriptions(m_BindingDescriptions);
//    return resultInfo;
//}
