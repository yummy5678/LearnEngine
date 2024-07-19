#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"
#include <GLFW/glfw3.h>

namespace VulkanCreate
{
	VkSurfaceKHR GetWindowSurface(vk::Instance& instance, GLFWwindow* window);

};

