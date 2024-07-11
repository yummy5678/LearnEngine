#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"
#include <GLFW/glfw3.h>

namespace SurfaceUtilities
{
	vk::UniqueSurfaceKHR createUnique(vk::Instance& instance, GLFWwindow* window);

};

