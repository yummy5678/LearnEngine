#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"
#include "ShaderUtility.h"

namespace GraphicsPipelineUtility
{
	 vk::UniquePipeline createGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::PipelineLayout pipelineLayout, vk::RenderPass renderPass);

	 vk::UniquePipelineLayout createPipelineLayout(vk::Device logicalDevice);
};

