#include "RendererBase.h"


RendererBase::RendererBase(VulkanInitializer& initializer) :
	m_pLogicalDevice(nullptr),
	m_pPhygicalDevice(nullptr),
	m_RenderArea(0),
	m_Offset(0),
	m_Extent(0),
	m_GraphicsPipeline(initializer)
{
}

RendererBase::~RendererBase()
{
}

vk::Rect2D RendererBase::GetSissorRect()
{
	return vk::Rect2D(m_Offset, m_Extent);
}

vk::Rect2D RendererBase::GetRenderRect()
{
	return m_RenderArea;
}


