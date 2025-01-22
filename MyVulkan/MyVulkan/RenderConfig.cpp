#include "RenderConfig.h"


RenderConfig::RenderConfig(VulkanInitializer& initializer):
    m_pLogicalDevice(nullptr),
    m_pPhygicalDevice(nullptr),
    m_GraphicsPipeline(initializer),
    m_Shader(), 
    m_RenderArea(), 
    m_Offset(), 
    m_Extent()
{
    m_pLogicalDevice = initializer.GetPLogicalDevice();
    m_pPhygicalDevice = initializer.GetPPhysicalDevice();
}

RenderConfig::~RenderConfig()
{
}

void RenderConfig::Initialize(RendererBase* renderere, std::vector<RenderObject>* objects, SceneCamera* camera)
{
    m_CameraDescriptor.Initialize(m_pLogicalDevice);
    m_TextureDescriptor.Initialize(m_pLogicalDevice);

    vk::Format colorFomat = renderere->GetColorFormat();
    vk::Format depthFomat = renderere->GetDepthFormat();
    vk::Extent2D extent = renderere->GetExtent();
    std::vector<ImageViewSet> imageView = renderere->GetImageSets();

    m_Shader.Create(m_pLogicalDevice, DefaultShaderDefine.VertexShaderPath, DefaultShaderDefine.FragmentShaderPath);
    m_RenderArea.setOffset({ 0, 0 });
    m_RenderArea.setExtent(extent);

    m_CameraDescriptor.Update(camera->GetProjectionBuffer());
    // m_TextureDescriptor.Update();


    auto descriptorSetLayouts   = GetDescriptorSetLayouts();
    auto constantRanges         = GetPushConstantRanges();
    auto vertexInputState       = GetVertexInputState();

    m_GraphicsPipeline.Create(
        m_pLogicalDevice,
        extent,
        m_RenderArea,
        colorFomat,
        depthFomat,
        &vertexInputState,
        m_Shader.GetShaderStages(),
        descriptorSetLayouts,
        constantRanges);

    m_DrawCommand.Create(m_pLogicalDevice, m_pPhygicalDevice, imageView);
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

std::vector<vk::DescriptorSet> RenderConfig::GetDescriptorSets()
{
    // �f�B�X�N���v�^�Z�b�g���o�C���h
    std::vector<vk::DescriptorSet> descriptorSets;
    descriptorSets.push_back(m_CameraDescriptor.GetDescriptorSet());
    descriptorSets.push_back(m_TextureDescriptor.GetDescriptorSet());

    return descriptorSets;
}

std::vector<vk::DescriptorSetLayout> RenderConfig::GetDescriptorSetLayouts()
{
    // �f�B�X�N���v�^�Z�b�g���o�C���h
    std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
    descriptorSetLayouts.push_back(m_CameraDescriptor.GetDescriptorSetLayout());
    descriptorSetLayouts.push_back(m_TextureDescriptor.GetDescriptorSetLayout());

    return descriptorSetLayouts;
}

std::vector<vk::PushConstantRange> RenderConfig::GetPushConstantRanges()
{
    return { GetPushConstantModelRange() };
}

void RenderConfig::DrawImage(std::vector<RenderObject>* objects, SceneCamera* camera)
{
    auto pipeline = m_GraphicsPipeline.GetPipeline();
    auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();
    auto descriptorSets = GetDescriptorSets();

    m_DrawCommand.BeginRendering(pipeline, m_RenderArea);

    m_CameraDescriptor.Update(camera->GetProjectionBuffer());


    for (auto& model : *objects)
    {
        for (auto& mesh : *model.GetMeshes())
        {
            m_TextureDescriptor.Update(mesh.GetPMaterial()->GetTextureImageView(), mesh.GetPMaterial()->GetSampler());
            m_DrawCommand.RenderMesh(m_GraphicsPipeline.GetPipelineLayout(), &descriptorSets, &mesh, model.GetPTransform());
        }
    }

    m_DrawCommand.EndRendering();
}

vk::PushConstantRange RenderConfig::GetPushConstantModelRange()
{
    return vk::PushConstantRange
    {
        vk::ShaderStageFlagBits::eVertex,	// �n�������V�F�[�_�[�X�e�[�W
        0,								    // �n�����f�[�^����ǂ̈ʒu�̃f�[�^�����邩
        sizeof(Transform)					// �n�������f�[�^�̃T�C�Y
    };
}

vk::PipelineVertexInputStateCreateInfo RenderConfig::GetVertexInputState()
{
    // �o�C���f�B���O�̒�`   //��Ńe���v���[�g������
    m_BindingDescriptions =
    {
        vk::VertexInputBindingDescription
        {
           0,                              // binding  
           sizeof(Vertex),                 // stride   
           vk::VertexInputRate::eVertex    // inputRate
        }
    };

    // ���͑����̒�`
    m_AttributeDescriptions = {
        // ���W
      vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },

       // �@��
      vk::VertexInputAttributeDescription{ 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) },

      // �e�N�X�`�����W
      vk::VertexInputAttributeDescription{ 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) }

    };

    // �p�C�v���C�����_���͏�Ԃ̍쐬
    vk::PipelineVertexInputStateCreateInfo resultInfo;
    resultInfo.pVertexBindingDescriptions = m_BindingDescriptions.data();
    resultInfo.vertexBindingDescriptionCount = m_BindingDescriptions.size();
    resultInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
    resultInfo.vertexAttributeDescriptionCount = m_AttributeDescriptions.size();
    return resultInfo;
}
