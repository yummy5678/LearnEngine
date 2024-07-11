#pragma once
#include <vulkan/vulkan.hpp>

struct SwapChainDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;		// Surface properties, e.g. image size/extent
	std::vector<VkSurfaceFormatKHR> formats;			// Surface image formats, e.g. RGBA and size of each colour
	std::vector<VkPresentModeKHR> presentationModes;	// How images should be presented to screen
};

namespace SwapChainUtility
{
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

};

