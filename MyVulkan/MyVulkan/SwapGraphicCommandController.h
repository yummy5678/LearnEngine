#pragma once
#include <vulkan/vulkan.hpp>
#include "SwapChainUtility.h"
#include "RenderPassUtility.h"
#include "GraphicsPipelineCreator.h"
#include "FramebufferGenerator.h"
#include "CommandGenerator.h"

class SwapGraphicCommandController
{
public:
	SwapGraphicCommandController(DeviceExtensionManager& deviceExtensionManager);
	~SwapGraphicCommandController();

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
	RenderpassGenerator			m_RenderpassGenerator;
	RenderingPipelineCreator			m_PipelineGenerator;
	FramebufferGenerator		m_FramebufferGenerator;
	SwapChainCommandGenerator	m_CommandGenerator;
	//SynchronizationGenerator	m_SynchroGenerator;


	int							m_CurrentFrame = 0;
};

