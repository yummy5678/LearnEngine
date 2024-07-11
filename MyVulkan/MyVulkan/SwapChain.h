#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"

struct SwapChain
{
	vk::UniqueSwapchainKHR create();

	// - Utility
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
};

