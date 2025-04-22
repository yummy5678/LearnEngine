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

    // ���W���[���̓p�C�v���C���쐬��͕s�v�ɂȂ�
    m_Shader.DestroyModule();
}


void RenderConfig::Cleanup()
{
    printf("RenderConfig��������܂�\n");
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
    // ToDo : �K�v�Ȃ��Ȃ���m_RenderFunction�Ƃ���ɕt������I�u�W�F�N�g�̃����N���폜


    /*std::function<void(vk::CommandBuffer commandBuffer)>;*/
    std::shared_ptr<RenderFunction> result = std::make_shared<RenderFunction>(
        [this](vk::CommandBuffer commandBuffer, void* key)
        {
            // NULL�`�F�b�N
            if (commandBuffer == VK_NULL_HANDLE || m_pObjects[key] == nullptr || m_pCamera[key] == nullptr)
            {
                return;
            }

            vk::DescriptorSet cameraDescriptorSet = m_pCamera[key]->GetDescriptorSet(m_Shader.GetCameraDescriptorLayout());

            auto pipeline = m_GraphicsPipeline.GetPipeline();
            auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();

            // �g�p����p�C�v���C�����o�C���h���܂��B
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

            for (auto& object : *m_pObjects[key])
            {
                for (auto& mesh : *object->GetMeshes())
                {
                    //mesh.GetSPMaterial().get()->GetDescriptorSet(m_Shader.GetTextureDescriptorLayout());

                    // �v�b�V���萔���V�F�[�_�[�ɓn���܂��B
                    commandBuffer.pushConstants(
                        pipelineLayout,
                        vk::ShaderStageFlagBits::eVertex,   // �v�b�V���萔���X�V����V�F�[�_�[�X�e�[�W
                        0,                                  // �I�t�Z�b�g
                        sizeof(Transform),                  // �v�b�V������f�[�^�̃T�C�Y
                        object->GetPTransform()              // ���ۂ̃f�[�^
                    );

                    auto vertexBuffer = mesh.GetSPMesh()->GetPVertex();
                    auto indexBuffer = mesh.GetSPMesh()->GetPIndex();


                    // ���_�o�b�t�@���o�C���h
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

                    // �C���f�b�N�X�o�b�t�@(���_�����ԏ��Ԃ̒l)�����ѕt���܂��B
                    commandBuffer.bindIndexBuffer(indexBuffer->GetBuffer(), 0, vk::IndexType::eUint32);
                    commandBuffer.drawIndexed(indexBuffer->GetVertexCount(), 1, 0, 0, 0);   // �C���f�b�N�X�ɏ]���ĕ`��

                }
            }
        });
    m_RenderFunction.push_back(result);
    m_pObjects[result.get()] = pObjects;
    m_pCamera[result.get()] = pCamera;
    return result;
}

