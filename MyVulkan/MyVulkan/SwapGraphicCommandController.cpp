#include "SwapGraphicCommandController.h"


SwapGraphicCommandController::SwapGraphicCommandController(DeviceExtensionManager& deviceExtensionManager):
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_Surface(VK_NULL_HANDLE),
	m_SwapchainGenerator(deviceExtensionManager),
	m_RenderpassGenerator(),
	m_FramebufferGenerator(),
	m_PipelineGenerator()
	//m_SynchroGenerator()
{
}

SwapGraphicCommandController::~SwapGraphicCommandController()
{
}

void SwapGraphicCommandController::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface, RenderConfig* pRenderConfig)
{
	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;
	m_Surface = surface;

	//スワップチェーンの作成
	m_SwapchainGenerator.LoadShader(logicalDevice, physicalDevice, surface);
	auto swapchain = m_SwapchainGenerator.GetSwapchain();
	auto swapchainInfo = m_SwapchainGenerator.GetSwapchainInfo();
	auto extent = swapchainInfo.imageExtent;
	auto swapchainImage = m_SwapchainGenerator.GetImages();


	//コマンドバッファの作成
	m_CommandGenerator.LoadShader(logicalDevice, physicalDevice, swapchainImage.GetSize());

	//レンダーパスの作成
	m_RenderpassGenerator.LoadShader(logicalDevice, swapchainImage.GetFomat());
	auto renderPass = m_RenderpassGenerator.GetRenderpass();

	//パイプラインの作成
	auto shaderStages = pRenderConfig->GetPipelineShader().GetShaderStages();
	m_PipelineGenerator.SetData(logicalDevice, renderPass, extent, shaderStages);
	auto graphicsPipeline = m_PipelineGenerator.GetPipeline();

	//フレームバッファの作成
	m_FramebufferGenerator.LoadShader(logicalDevice, swapchainImage.GetImageViews(), renderPass, extent);
	auto framebuffers = m_FramebufferGenerator.GetFramebuffers();

	//コマンドの記録
	m_CommandGenerator.LoadShader(logicalDevice, physicalDevice, swapchainInfo.minImageCount);
	auto commandBuffers = m_CommandGenerator.GetCommandBuffers();
	//m_CommandGenerator.RecordGraphicCommands(framebuffers, renderPass, windowExtent, graphicsPipeline);

	for (int i = 0; i < swapchainInfo.minImageCount; i++)
	{
		m_CommandGenerator.DrawFrame(commandBuffers[i], renderPass, framebuffers[i], { {0,0}, extent }, graphicsPipeline);
	}
	//WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], extent);
	m_CommandGenerator.PresentFrame(swapchain);
	//WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], windowExtent);

	//m_SynchroGenerator.Create(logicalDevice);

}

void SwapGraphicCommandController::Destroy()
{
}

void SwapGraphicCommandController::DrawFrame()
{
	auto swapchainInfo = m_SwapchainGenerator.GetSwapchainInfo();
	for (int i = 0; i < swapchainInfo.minImageCount; i++)
	{
		m_CommandGenerator.DrawFrame(commandBuffers[i], renderPass, framebuffers[i], { {0,0}, extent }, graphicsPipeline);
	}
}

void SwapGraphicCommandController::PresentFrame()
{
	m_CommandGenerator.PresentFrame(swapchain);
}

















