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

void RenderConfig::Initialize(RendererBase* renderere)
{
    vk::Format colorFomat = renderere->GetColorFormat();
    vk::Format depthFomat = renderere->GetDepthFormat();
    vk::Extent2D extent = renderere->GetExtent();

    m_Shader.Create(m_pLogicalDevice);
    m_RenderArea.setOffset({ 0, 0 });
    m_RenderArea.setExtent(extent);

    // m_CameraDescriptor.Update(camera->GetProjectionBuffer());
    // m_TextureDescriptors.Update();


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

    // ���W���[���̓p�C�v���C���쐬��͕s�v�ɂȂ�
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

VModelShaderConfiguer* RenderConfig::GetPShaderConfiguer()
{
    return &m_Shader;
}

void RenderConfig::BindRenderingCommand(
    vk::CommandBuffer command,
    std::vector<RenderObject>* pObjects, 
    SceneCamera* pCamera)
{
    // NULL�`�F�b�N
    if (command == VK_NULL_HANDLE || pObjects == nullptr || pCamera == nullptr)
        return;

    auto pipeline = m_GraphicsPipeline.GetPipeline();
    auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();

    // �g�p����p�C�v���C�����o�C���h���܂��B
    command.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

    for (auto& object : *pObjects)
    {
        for (auto& mesh : *object.GetMeshes())
        {
            CreateTextureDescriptor(mesh.GetPMaterial());

            // �v�b�V���萔���V�F�[�_�[�ɓn���܂��B
            command.pushConstants(
                pipelineLayout,
                vk::ShaderStageFlagBits::eVertex,   // �v�b�V���萔���X�V����V�F�[�_�[�X�e�[�W
                0,                                  // �I�t�Z�b�g
                sizeof(Transform),                  // �v�b�V������f�[�^�̃T�C�Y
                object.GetPTransform()              // ���ۂ̃f�[�^
            );

            auto vertexBuffer = mesh.GetPMesh()->GetVertex();
            auto indexBuffer = mesh.GetPMesh()->GetIndex();


            // ���_�o�b�t�@���o�C���h
            command.bindVertexBuffers(0, vertexBuffer.GetBuffer(), {0});

            command.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                pipelineLayout,
                0,
                m_Shader.GetDescriptorSets(),
                nullptr);

            // �C���f�b�N�X�o�b�t�@(���_�����ԏ��Ԃ̒l)�����ѕt���܂��B
            command.bindIndexBuffer(indexBuffer.GetBuffer(), 0, vk::IndexType::eUint32);
            command.drawIndexed(indexBuffer.GetSize(), 1, 0, 0, 0);   // �C���f�b�N�X�ɏ]���ĕ`��

        }
    }
}

void RenderConfig::CreateTextureDescriptor(VMaterial* pMaterial)
{
    //auto material = std::make_shared<VMaterial*>(std::move(pMaterial));
    auto material = std::make_shared<VMaterial*>(pMaterial);
    m_RenderObjects.push_back(material);
    m_TextureDescriptors[material].Initialize(m_pLogicalDevice, m_Shader.GetTextureDescriptorLayout());

    pMaterial->SetMMaterialUpdateObserver([](){});
}

void RenderConfig::UpdateTextureDescriptor()
{

}

//std::vector<vk::DescriptorSet> RenderConfig::GetDescriptorSets()
//{
//    // �f�B�X�N���v�^�Z�b�g���o�C���h
//    std::vector<vk::DescriptorSet> descriptorSets;
//    descriptorSets.push_back(m_CameraDescriptor.GetDescriptorSet());
//    descriptorSets.push_back(m_TextureDescriptors.GetDescriptorSet());
//
//    return descriptorSets;
//}

//std::vector<vk::DescriptorSetLayout> RenderConfig::GetDescriptorSetLayouts()
//{
//    // �f�B�X�N���v�^�Z�b�g���o�C���h
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
//    m_CameraDescriptor.Update(camera->GetProjectionBuffer());
//
//
//    for (auto& model : *objects)
//    {
//        for (auto& mesh : *model.GetMeshes())
//        {
//            m_TextureDescriptors.Update(mesh.GetPMaterial()->GetTextureImageView(), mesh.GetPMaterial()->GetSampler());
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
//        vk::ShaderStageFlagBits::eVertex,	// �n�������V�F�[�_�[�X�e�[�W
//        0,								    // �n�����f�[�^����ǂ̈ʒu�̃f�[�^�����邩
//        sizeof(Transform)					// �n�������f�[�^�̃T�C�Y
//    };
//}

//vk::PipelineVertexInputStateCreateInfo RenderConfig::GetVertexInputState()
//{
//    // �o�C���f�B���O�̒�`   //��Ńe���v���[�g������
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
//    // ���͑����̒�`
//    m_AttributeDescriptions = {
//        // ���W
//      vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },
//
//       // �@��
//      vk::VertexInputAttributeDescription{ 1, 0, vk::Format::eR32G32B32Sfloat,  offsetof(Vertex, normal) },
//
//      // �e�N�X�`�����W
//      vk::VertexInputAttributeDescription{ 2, 0, vk::Format::eR32G32Sfloat,     offsetof(Vertex, textureCoord) }
//    };
//
//    // �p�C�v���C�����_���͏�Ԃ̍쐬
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
