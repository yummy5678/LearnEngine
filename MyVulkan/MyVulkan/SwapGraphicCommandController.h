#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"
#include "RenderingPipelineCreator.h"
#include "CommandGenerator.h"
#include "RenderConfig.h"

class SwapchainRenderer
{
public:
	SwapchainRenderer(DeviceExtensionManager& deviceExtensionManager);
	~SwapchainRenderer();

	void Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface, RenderConfig* pRenderConfig);
	void Destroy();
	void DrawFrame(Scene scene, vk::Rect2D renderArea);
	void PresentFrame();



private:
	// オブジェクト
	vk::SurfaceKHR				m_Surface;
	vk::PhysicalDevice			m_PhysicalDevice;
	vk::Device					m_LogicalDevice;
	RenderConfig*				m_pRenderConfig;

	SwapchainGenerator			m_SwapchainGenerator;
	RenderingPipelineCreator	m_PipelineGenerator;
	SwapChainCommandGenerator	m_CommandGenerator;


	int							m_CurrentFrame = 0;
};

