#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "GraphicsDefine.h"
#include "Utilities.h"
#include <GLFW/glfw3.h>


class SurfaceGenerator : public CGeneratorBase
{
public:
	SurfaceGenerator();
	~SurfaceGenerator();

	void CreateWindowSurface(vk::Instance instance, GLFWwindow* window);
	VkSurfaceKHR GetSurface();
	// サーフェスの機能を取得
	vk::SurfaceCapabilitiesKHR GetCapabilities(vk::PhysicalDevice physicalDevice);

	void Destroy(vk::Instance instance);


private:
	vk::Instance				m_Instance;
	vk::SurfaceKHR				m_Surface;
	//vk::SurfaceCapabilitiesKHR	m_Capabilities;
};