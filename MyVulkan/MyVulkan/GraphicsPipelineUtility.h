#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"
#include "ShaderUtility.h"
#include "ViewportGenerator.h"

namespace GraphicsPipelineUtility
{
	 vk::UniquePipeline createGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::PipelineLayout pipelineLayout, vk::RenderPass renderPass);

	 vk::UniquePipelineLayout createPipelineLayout(vk::Device logicalDevice);
};

class PipelineGenerator : CGeneratorBase
{
public:
	PipelineGenerator();
	~PipelineGenerator();

	void Create(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);
	void Destroy(vk::Device logicalDevice);

	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();

	vk::GraphicsPipelineCreateInfo	GetPipelineInfo();
	vk::PipelineLayoutCreateInfo	GetLayoutInfo();

private:
	vk::Pipeline					m_Pipeline;
	vk::GraphicsPipelineCreateInfo	m_PipelineCreateInfo;

	vk::PipelineLayout				m_PipelineLayout;
	vk::PipelineLayoutCreateInfo	m_PipelineLayoutCreateInfo;


	std::vector<vk::PipelineShaderStageCreateInfo>		m_ShaderStageInfos;
	vk::PipelineMultisampleStateCreateInfo				m_MultisamplingInfo;
	vk::PipelineVertexInputStateCreateInfo				m_VertexInputCreateInfo;
	vk::PipelineInputAssemblyStateCreateInfo			m_InputAssemblyInfo;
	ViewportGenerator									m_viewportGenerator;
	vk::PipelineRasterizationStateCreateInfo			m_RasterizerCreateInfo;
	std::vector<vk::PipelineColorBlendAttachmentState>	m_ColorBlendAttachment;
	vk::PipelineColorBlendStateCreateInfo				m_ColorBlendCreateInfo;

	vk::GraphicsPipelineCreateInfo CreateGraphicsPipelineInfo(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);


	vk::PipelineLayout CreatePipelineLayout(vk::Device logicalDevice);

	std::vector<vk::PipelineShaderStageCreateInfo>		GetShaderStageInfo(vk::Device logicalDevice);
	vk::PipelineVertexInputStateCreateInfo				GetVertexInputStateInfo();
	vk::PipelineInputAssemblyStateCreateInfo			GetInputAssemblyStateInfo();
	vk::PipelineRasterizationStateCreateInfo			GetRasterizationStateInfo();

	vk::PipelineMultisampleStateCreateInfo				GetMultisampleStateInfo();
	std::vector<vk::PipelineColorBlendAttachmentState>	GetColorBlendAttachmentState();
	vk::PipelineColorBlendStateCreateInfo				GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* colorBlendAttachment);

};
