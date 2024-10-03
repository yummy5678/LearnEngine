#pragma once
#include <vulkan/vulkan.hpp>

#include "MeshObject.h"


class VertexDescription
{
public:
	VertexDescription();
	~VertexDescription();

	



private:
	vk::VertexInputBindingDescription		m_BindingDescription;

	vk::VertexInputAttributeDescription		m_InputDescription;

	vk::PipelineVertexInputStateCreateInfo	m_InputInfo;



};

