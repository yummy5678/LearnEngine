#include "RenderConfig.h"


RenderConfig::RenderConfig(VulkanInitializer& initializer):
    RendererBase(initializer),
    m_Shader(), 
    m_RenderFunction(),
    m_pObjects(),
    m_pCamera()
{
}

RenderConfig::~RenderConfig()
{
    Cleanup();
}

void RenderConfig::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, RenderingTarget* renderere)
{
    m_LogicalDevice = logicalDevice;
    m_PhygicalDevice = physicalDevice;

    vk::Format colorFomat = renderere->GetColorFormat();
    vk::Format depthFomat = renderere->GetDepthFormat();
    vk::Extent2D extent = renderere->GetExtent();



    m_Shader.Create(m_LogicalDevice);
    m_RenderArea.setOffset({ 0, 0 });
    m_RenderArea.setExtent(extent);

    // m_CameraDescriptor.UpdateDescriptorSets(camera->GetProjectionBuffer());
    // m_TextureDescriptors.UpdateDescriptorSets();


    auto descriptorSetLayouts   = m_Shader.GetDescriptorSetLayouts();
    auto constantRanges         = m_Shader.GetPushConstantRanges();
    auto vertexInputState       = m_Shader.GetVertexInputState();

    m_GraphicsPipeline.Create(
        m_LogicalDevice,
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


void RenderConfig::Cleanup()
{
    printf("RenderConfigを解放します\n");
    m_pCamera.clear();
    m_pObjects.clear();
    m_RenderFunction.clear();
    m_Shader.Cleanup();
    RendererBase::Cleanup();
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

                    auto vertexBuffer = mesh.GetSPMesh()->GetPVertex();
                    auto indexBuffer = mesh.GetSPMesh()->GetPIndex();


                    // 頂点バッファをバインド
                    commandBuffer.bindVertexBuffers(0, vertexBuffer->GetBuffer(), { 0 });

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
                    commandBuffer.bindIndexBuffer(indexBuffer->GetBuffer(), 0, vk::IndexType::eUint32);
                    commandBuffer.drawIndexed(indexBuffer->GetVertexCount(), 1, 0, 0, 0);   // インデックスに従って描画

                }
            }
        });
    m_RenderFunction.push_back(result);
    m_pObjects[result.get()] = pObjects;
    m_pCamera[result.get()] = pCamera;
    return result;
}

