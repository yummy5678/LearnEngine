#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "GeneratorBase.h"
#include "GraphicsDefine.h"
#include "Utilities.h"
#include "InstanceExtensionManager.h"


class SurfaceGenerator : public GeneratorBase
{
public:
	SurfaceGenerator(InstanceExtension& instanceExtension);
	SurfaceGenerator();
	~SurfaceGenerator();

	void CreateWindowSurface(vk::Instance instance, GLFWwindow* m_pWindow);
	VkSurfaceKHR GetSurface();
	// サーフェスの機能を取得
	vk::SurfaceCapabilitiesKHR GetCapabilities(vk::PhysicalDevice physicalDevice);
	std::vector<vk::SurfaceFormatKHR> GetFomats(vk::PhysicalDevice physicalDevice);

	void Destroy(vk::Instance instance);


private:
	vk::Instance				m_Instance;
	vk::SurfaceKHR				m_Surface;
	//vk::SurfaceCapabilitiesKHR	m_Capabilities;

	std::vector<const char*> GetGLFWSurfaceExtensions();
};