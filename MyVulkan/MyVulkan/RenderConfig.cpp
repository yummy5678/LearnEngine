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
    // ディスクリプタセットをバインドします。
    std::vector<vk::DescriptorSet> descriptorSetGroup =
    {
        camera->GetDescriptorSet(),
        objects[0]->GetMaterials()[0].GetDescriptorSet()
    };
    // パイプラインレイアウトの作成	//今は作らなくていいかも
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
//    // ディスクリプタセットをバインドします。
//    std::vector<vk::DescriptorSet> descriptorSetGroup =
//    {
//
//        //descriptorSets[currentImage], //たぶんカメラ情報が入ってる(uboViewProjection)
//        //samplerDescriptorSets[thisModel.getMesh(k)->getTexId()]
//        SceneCamera::GetDescriptorSet(),
//        model.GetMaterials()[0].GetDescriptorSet()
//    };
//    // パイプラインレイアウトの作成	//今は作らなくていいかも
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
    // ディスクリプタセットをバインド
    std::vector<vk::DescriptorSet> descriptorSetGroup;
    descriptorSetGroup.push_back(camera->GetDescriptorSet());

    // 描画するメッシュをループ
    for (auto& model : *objects)
    {
        // 各メッシュをループ
        for (auto& mesh : model.GetMeshes())
        {
            descriptorSetGroup.push_back(mesh.);

        }
    }
}
