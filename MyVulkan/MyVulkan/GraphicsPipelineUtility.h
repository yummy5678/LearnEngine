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
	PipelineGenerator();
	~PipelineGenerator();

	void Create(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);

	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();

	vk::GraphicsPipelineCreateInfo	GetPipelineInfo();
	vk::PipelineLayoutCreateInfo	GetLayoutInfo();

private:
	vk::UniquePipeline				m_Pipeline;
	vk::GraphicsPipelineCreateInfo	m_PipelineCreateInfo;

	vk::UniquePipelineLayout		m_PipelineLayout;
	vk::PipelineLayoutCreateInfo	m_PipelineLayoutCreateInfo;

	vk::Pipeline CreateGraphicsPipeline(
		vk::Device										logicalDevice,
		std::vector<vk::PipelineShaderStageCreateInfo>* shaderStageInfos,
		vk::PipelineVertexInputStateCreateInfo*			vertexInputCreateInfo,
		vk::PipelineInputAssemblyStateCreateInfo*		inputAssemblyInfo,
		vk::PipelineTessellationStateCreateInfo*		tessellationStateInfo,
		vk::PipelineViewportStateCreateInfo*			viewportStateCreateInfo,
		vk::PipelineRasterizationStateCreateInfo*		rasterizerCreateInfo,
		vk::PipelineMultisampleStateCreateInfo*			multisampleInfo,
		vk::PipelineDepthStencilStateCreateInfo*		depthStencilInfo,
		vk::PipelineColorBlendStateCreateInfo*			colourBlendingCreateInfo,
		vk::PipelineDynamicStateCreateInfo*				dynamicStateInfo,
		vk::PipelineLayout								pipelineLayout,
		vk::RenderPass									renderPass);


	vk::PipelineLayout CreatePipelineLayout(vk::Device logicalDevice);

	std::vector<vk::PipelineShaderStageCreateInfo>	GetShaderStageInfo(vk::Device logicalDevice);
	vk::PipelineVertexInputStateCreateInfo			GetVertexInputStateInfo();
	vk::PipelineInputAssemblyStateCreateInfo		GetInputAssemblyStateInfo();
	vk::PipelineViewportStateCreateInfo				GetViewportStateInfo(vk::Extent2D extent);
	vk::PipelineRasterizationStateCreateInfo		GetRasterizationStateInfo();

	vk::PipelineMultisampleStateCreateInfo			GetMultisampleStateInfo();
	std::vector<vk::PipelineColorBlendAttachmentState>			GetColorBlendAttachmentState();
	vk::PipelineColorBlendStateCreateInfo			GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* colorBlendAttachment);

};
