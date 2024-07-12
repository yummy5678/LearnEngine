#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"

namespace RenderPassUtility
{
	vk::UniqueRenderPass createRenderPass(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

};

