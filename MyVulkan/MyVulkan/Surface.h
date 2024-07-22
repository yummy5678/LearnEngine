#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"
#include <GLFW/glfw3.h>


class SurfaceGenerator
{
public:
	void CreateWindowSurface(vk::Instance & instance, GLFWwindow * window);
	VkSurfaceKHR* GetSurface();


private:
	vk::UniqueSurfaceKHR m_Surface;
};