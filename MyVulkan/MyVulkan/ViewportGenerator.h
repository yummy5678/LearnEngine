#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"

class ViewportGenerator : public CGeneratorBase
{
public:
	ViewportGenerator();
	~ViewportGenerator();

	void LoadShader(vk::Extent2D extent);


	vk::Viewport	GetViewport();
	vk::Rect2D		GetScissor();
	vk::PipelineViewportStateCreateInfo* GetCreateInfoPointer();
private:
	vk::Viewport	m_Viewport;
	vk::Rect2D		m_Scissor;

	vk::PipelineViewportStateCreateInfo m_ViewportStateCreateInfo;

	vk::PipelineViewportStateCreateInfo CreateInfo(vk::Extent2D extent);
};

