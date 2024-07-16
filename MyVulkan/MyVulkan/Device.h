#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "QueueUtility.h"
#include "SwapChainUtility.h"

//使用する拡張機能
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace DeviceUtility
{
	//physical
	//物理デバイスの取得
	vk::PhysicalDevice getPhysicalDevice(vk::Instance& instance, vk::SurfaceKHR surface);

	//拡張機能が使えるかの確認
	bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
	bool checkDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);
	
	//logical
	vk::UniqueDevice createLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
}


