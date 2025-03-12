#include "HelloTriangleRenderer.h"


HelloTriangleRenderer::HelloTriangleRenderer(VulkanInitializer& initializer) :
    RendererBase(initializer),
    m_Shader()
{
    m_pLogicalDevice = initializer.GetPLogicalDevice();
    m_pPhygicalDevice = initializer.GetPPhysicalDevice();
}

HelloTriangleRenderer::~HelloTriangleRenderer()
{
}

vk::Pipeline HelloTriangleRenderer::GetPipeline()
{
    return m_GraphicsPipeline.GetPipeline();
}

vk::PipelineLayout HelloTriangleRenderer::GetPipelineLayout()
{
    return m_GraphicsPipeline.GetPipelineLayout();
}

void HelloTriangleRenderer::Initialize(RenderTarget* renderere)
{
    vk::Format colorFomat = renderere->GetColorFormat();
    vk::Format depthFomat = renderere->GetDepthFormat();
    vk::Extent2D extent = renderere->GetExtent();

    m_Shader.Create(m_pLogicalDevice);
    m_RenderArea.setOffset({ 0, 0 });
    m_RenderArea.setExtent(extent);

    auto descriptorSetLayouts = m_Shader.GetDescriptorSetLayouts();
    auto constantRanges = m_Shader.GetPushConstantRanges();
    auto vertexInputState = m_Shader.GetVertexInputState();

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

std::shared_ptr<RenderFunction> HelloTriangleRenderer::GetRenderFunction()
{
    std::shared_ptr<RenderFunction> result = std::make_shared<RenderFunction>(
        [this](vk::CommandBuffer commandBuffer, void* key)
        {
            auto pipeline = m_GraphicsPipeline.GetPipeline();
            auto pipelineLayout = m_GraphicsPipeline.GetPipelineLayout();

            // 使用するパイプラインをバインドします。
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
            commandBuffer.draw(3, 1, 0, 0);
        });
    return result;
}
