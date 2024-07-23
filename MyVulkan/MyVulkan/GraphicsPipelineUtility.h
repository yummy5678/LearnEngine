#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"
#include "ShaderUtility.h"

namespace GraphicsPipelineUtility
{
	 vk::UniquePipeline createGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::PipelineLayout pipelineLayout, vk::RenderPass renderPass);

	 vk::UniquePipelineLayout createPipelineLayout(vk::Device logicalDevice);
};

class PipelineGenerator
{
public:
	PipelineGenerator(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);
	~PipelineGenerator();

	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();

	vk::GraphicsPipelineCreateInfo	GetPipelineInfo();
	vk::PipelineLayoutCreateInfo	GetLayoutInfo();

private:
	vk::UniquePipeline				m_Pipeline;
	vk::GraphicsPipelineCreateInfo	m_PipelineCreateInfo;

	vk::UniquePipelineLayout		m_PipelineLayout;
	vk::PipelineLayoutCreateInfo	m_PipelineLayoutCreateInfo;

	void CreateGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::PipelineLayout pipelineLayout, vk::RenderPass renderPass);
	void CreatePipelineLayout(vk::Device logicalDevice);
};