#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "QueueUtility.h"
#include "SwapChainUtility.h"

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace DeviceUtility
{

	//physical
	vk::PhysicalDevice getPhysicalDevice(vk::Instance& instance, VkSurfaceKHR surface);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	
	//logical
	vk::UniqueDevice createLogicalDevice(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
}


