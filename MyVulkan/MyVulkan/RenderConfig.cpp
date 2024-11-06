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

void RenderConfig::Initialize()
{
	m_GraphicsPipeline.Create();
}

vk::Rect2D RenderConfig::GetExtent2D()
{
	return vk::Rect2D(m_Offset, m_Extent);
}

std::vector<vk::PipelineShaderStageCreateInfo> RenderConfig::GetShaderStages()
{
	return m_Shader.GetShaderStages();
}

Scene* RenderConfig::GetPScene()
{
	return m_pRenderScene;
}


