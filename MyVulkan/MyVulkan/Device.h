#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "QueueUtility.h"
#include "SwapChainUtility.h"

//使用する拡張機能
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace VulkanCreate
{
	//physical
	//物理デバイスの取得
	vk::PhysicalDevice GetPhysicalDevice(vk::Instance& instance, vk::SurfaceKHR surface);


	
	//logical
	vk::DeviceCreateInfo CreateDeviceInfo(std::vector< vk::DeviceQueueCreateInfo > queueCreateInfos);
}

namespace VulkanUtility
{
	//拡張機能が使えるかの確認
	bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);
	bool CheckDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);
}