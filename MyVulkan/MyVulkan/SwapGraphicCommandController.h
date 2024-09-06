#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"
#include "RenderPassUtility.h"
#include "GraphicsPipelineUtility.h"
#include "FramebufferGenerator.h"
#include "CommandUtility.h"
#include "SynchronizationGenerator.h"

class SwapGraphicCommandController
{
public:
	SwapGraphicCommandController(CDeviceExtensionManager& deviceExtensionManager);
	~SwapGraphicCommandController();

	void Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void DrawFrame();
	void PresentFrame();



private:
	// オブジェクト
	vk::SurfaceKHR			m_Surface;
	vk::PhysicalDevice		m_PhysicalDevice;
	vk::Device				m_LogicalDevice;


	SwapchainGenerator			m_SwapchainGenerator;
	RenderpassGenerator			m_RenderpassGenerator;
	PipelineGenerator			m_PipelineGenerator;
	FramebufferGenerator		m_FramebufferGenerator;
	CommandGenerator			m_CommandGenerator;
	SynchronizationGenerator	m_SynchroGenerator;


	int							m_CurrentFrame = 0;
};

