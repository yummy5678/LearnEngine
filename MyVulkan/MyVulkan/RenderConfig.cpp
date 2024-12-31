#include "RenderConfig.h"
#include "SceneCamera.h"


RenderConfig::RenderConfig(VulkanInitializer& initializer):
    m_Device(VK_NULL_HANDLE),
    m_GraphicsPipeline(initializer),
    m_Shader(), 
    m_RenderArea(), 
    m_Offset(), 
    m_Extent()
{
}

RenderConfig::~RenderConfig()
{
}

void RenderConfig::Initialize(RendererBase* renderere, std::vector<RenderObject>* objects, SceneCamera* camera)
{
    vk::Format colorFomat = renderere->GetColorFormat();
    vk::Format depthFomat = renderere->GetDepthFormat();
    vk::Extent2D extent = renderere->GetExtent();

    m_Shader.Create(m_Device, DefaultShaderDefine.VertexShaderPath, DefaultShaderDefine.FragmentShaderPath);
    m_RenderArea.setOffset({ 0, 0 });
    m_RenderArea.setExtent(extent);

    m_TextureDescriptor.CreateSingleDescriptorSet();
    // �f�B�X�N���v�^�Z�b�g���o�C���h���܂��B
    std::vector<vk::DescriptorSet> descriptorSetGroup =
    {
        camera->GetDescriptorSet(),
        objects[0]->GetMaterials()[0].GetDescriptorSet()
    };
    // �p�C�v���C�����C�A�E�g�̍쐬	//���͍��Ȃ��Ă�������
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = { m_TextureDescriptor.GetDescriptorSetLayout() };
    //CreatePipelineLayout(logicalDevice, { descriptorSetLayouts }, { GetPushConstantModelRange()});

    m_GraphicsPipeline.Create(
        m_Device,
        extent,
        m_RenderArea,
        colorFomat,
        depthFomat,
        m_Shader.GetShaderStages());
}

//void RenderConfig::Initialize(vk::Device logicalDevice, vk::Extent2D extent, vk::Format colorFomat, vk::Format depthFomat)
//{
//    m_Shader.Create(logicalDevice, DefaultShaderDefine.VertexShaderPath, DefaultShaderDefine.FragmentShaderPath);
//    m_RenderArea.setOffset({0, 0});
//    m_RenderArea.setExtent(extent);
//
//    m_TextureDescriptor.CreateSingleDescriptorSet();
//    // �f�B�X�N���v�^�Z�b�g���o�C���h���܂��B
//    std::vector<vk::DescriptorSet> descriptorSetGroup =
//    {
//
//        //descriptorSets[currentImage], //���Ԃ�J������񂪓����Ă�(uboViewProjection)
//        //samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
//        SceneCamera::GetDescriptorSet(),
//        model.GetMaterials()[0].GetDescriptorSet()
//    };
//    // �p�C�v���C�����C�A�E�g�̍쐬	//���͍��Ȃ��Ă�������
//    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = { m_TextureDescriptor.GetDescriptorSetLayout() };
//    //CreatePipelineLayout(logicalDevice, { descriptorSetLayouts }, { GetPushConstantModelRange()});
//
//    m_GraphicsPipeline.Create(
//        logicalDevice, 
//        extent, 
//        m_RenderArea, 
//        colorFomat, 
//        depthFomat,
//        m_Shader.GetShaderStages());
//}

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

std::vector<vk::PipelineShaderStageCreateInfo> RenderConfig::GetShaderStages()
{
    return m_Shader.GetShaderStages();
}

void RenderConfig::CreateDescriptors(std::vector<RenderObject>* objects, SceneCamera* camera)
{
    // �f�B�X�N���v�^�Z�b�g���o�C���h
    std::vector<vk::DescriptorSet> descriptorSetGroup;
    descriptorSetGroup.push_back(camera->GetDescriptorSet());

    // �`�悷�郁�b�V�������[�v
    for (auto& model : *objects)
    {
        // �e���b�V�������[�v
        for (auto& mesh : model.GetMeshes())
        {
            descriptorSetGroup.push_back(mesh.);

        }
    }
}
