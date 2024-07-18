#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "QueueUtility.h"
#include "SwapChainUtility.h"

//�g�p����g���@�\
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace VulkanCreate
{
	//physical
	//�����f�o�C�X�̎擾
	vk::PhysicalDevice GetPhysicalDevice(vk::Instance& instance, vk::SurfaceKHR surface);


	
	//logical
	vk::DeviceCreateInfo CreateDeviceInfo(std::vector< vk::DeviceQueueCreateInfo > queueCreateInfos);
}

namespace VulkanUtility
{
	//�g���@�\���g���邩�̊m�F
	bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);
	bool CheckDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);
}