#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "QueueUtility.h"
#include "SwapChainUtility.h"

//�g�p����g���@�\
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace DeviceUtility
{
	//physical
	//�����f�o�C�X�̎擾
	vk::PhysicalDevice getPhysicalDevice(vk::Instance& instance, VkSurfaceKHR surface);

	//�g���@�\���g���邩�̊m�F
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	
	//logical
	vk::UniqueDevice createLogicalDevice(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
}


