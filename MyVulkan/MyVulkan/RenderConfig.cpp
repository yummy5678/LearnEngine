#include "RenderConfig.h"


RenderConfig::RenderConfig()
{
}

RenderConfig::~RenderConfig()
{
}

void RenderConfig::Initialize(vk::Device logicalDevice, vk::Extent2D extent, vk::Format colorFomat, vk::Format depthFomat)
{
	m_GraphicsPipeline.Create(logicalDevice, extent, m_RenderArea, colorFomat, depthFomat);
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

std::vector<vk::PipelineShaderStageCreateInfo> RenderConfig::GetShaderStages()
{
	return m_Shader.GetShaderStages();
}




