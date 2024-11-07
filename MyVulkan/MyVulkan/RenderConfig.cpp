#include "RenderConfig.h"


RenderConfig::RenderConfig() : 
	m_Offset(0,0),
	m_Extent(0,0),
	m_Shader(),
	m_pRenderScene(nullptr)
{
}

RenderConfig::~RenderConfig()
{
}

void RenderConfig::Initialize(vk::Device logicalDevice, vk::Extent2D extent)
{
	m_GraphicsPipeline.Create(logicalDevice, extent, );
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

std::vector<vk::PipelineShaderStageCreateInfo> RenderConfig::GetShaderStages()
{
	return m_Shader.GetShaderStages();
}

Scene* RenderConfig::GetPScene()
{
	return m_pRenderScene;
}


