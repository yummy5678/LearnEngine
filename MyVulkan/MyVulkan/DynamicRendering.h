#pragma once
#include "vulkan/vulkan.hpp"
#include "DeviceExtensionManager.h"
#include "RenderPipeline.h"


class VDynamicRenderingCommand
{
public:
	VDynamicRenderingCommand(DeviceExtensionManager& deviceExtensionManager);
	~VDynamicRenderingCommand();

	void RenderWithDynamicRendering(
		vk::ImageView				colorImageView, 
		vk::Extent2D				extent, 
		std::vector<RenderConfig>	config
		);


private:
	vk::CommandBuffer m_CommandBuffer;





};

