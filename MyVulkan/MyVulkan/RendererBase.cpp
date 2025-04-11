#include "RendererBase.h"


RendererBase::RendererBase(VulkanInitializer& initializer) :
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhygicalDevice(VK_NULL_HANDLE),
	m_RenderArea(0),
	m_Offset(0),
	m_Extent(0),
	m_GraphicsPipeline(initializer)
{
}

RendererBase::~RendererBase()
{
}

void RendererBase::Cleanup()
{
	printf("RendererBase‚ð‰ð•ú‚µ‚Ü‚·\n");
	m_GraphicsPipeline.Cleanup();
	m_Extent = 0;
	m_Offset = 0;
	m_RenderArea = { 0,0,0,0 };
	m_PhygicalDevice = VK_NULL_HANDLE;
	m_LogicalDevice = VK_NULL_HANDLE;

}

vk::Rect2D RendererBase::GetSissorRect()
{
	return vk::Rect2D(m_Offset, m_Extent);
}

vk::Rect2D RendererBase::GetRenderRect()
{
	return m_RenderArea;
}


