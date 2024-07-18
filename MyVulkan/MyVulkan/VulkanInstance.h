#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "VulkanValidation.h"


namespace VulkanCreate
{
	vk::ApplicationInfo		GetApplicationInfo();
	VkInstanceCreateInfo	GetInstanceInfo(vk::ApplicationInfo appInfo = {});
}

namespace VulkanUtility
{
	bool CheckValidationLayerSupport(const std::vector<const char*> validationLayers);
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
};

