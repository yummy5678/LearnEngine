#include "SwapGraphicCommandController.h"


SwapchainRenderer::SwapchainRenderer(DeviceExtensionManager& deviceExtensionManager):
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_Surface(VK_NULL_HANDLE),
	m_SwapchainGenerator(deviceExtensionManager),
	m_PipelineGenerator(deviceExtensionManager)
{
}

SwapchainRenderer::~SwapchainRenderer()
{
}

void SwapchainRenderer::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface, RenderConfig* pRenderConfig)
{
	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;
	m_Surface = surface;

	//スワップチェーンの作成
	m_SwapchainGenerator.Create(logicalDevice, physicalDevice, surface);
	auto swapchain = m_SwapchainGenerator.GetSwapchain();
	auto swapchainInfo = m_SwapchainGenerator.GetSwapchainInfo();
	auto extent = swapchainInfo.imageExtent;
	auto swapchainImage = m_SwapchainGenerator.GetImages();





	// グラフィクスパイプラインの作成
	auto shaderStages = pRenderConfig->GetShaderStages();
	m_PipelineGenerator.Create(logicalDevice, extent, pRenderConfig->GetSissorRect(), swapchainInfo.imageFormat, , shaderStages);
	auto graphicsPipeline = m_PipelineGenerator.GetPipeline();


	//コマンドの記録
	m_CommandGenerator.Create(logicalDevice, physicalDevice, swapchainInfo.minImageCount);
	auto commandBuffers = m_CommandGenerator.GetCommandBuffers();
	//m_CommandGenerator.RecordGraphicCommands(framebuffers, renderPass, windowExtent, graphicsPipeline);

	for (int i = 0; i < swapchainInfo.minImageCount; i++)
	{
		m_CommandGenerator.DrawFrame(commandBuffers[i], { {0,0}, extent }, graphicsPipeline);
	}
	//WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], extent);
	m_CommandGenerator.PresentFrame(swapchain);
	//WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], windowExtent);

	//m_SynchroGenerator.Create(logicalDevice);

}

void SwapchainRenderer::Destroy()
{
}


void SwapchainRenderer::PresentFrame()
{
	auto swapchain = m_SwapchainGenerator.GetSwapchain();
	m_CommandGenerator.PresentFrame(swapchain);
}

















