#include "SwapGraphicCommandController.h"

SwapGraphicCommandController::SwapGraphicCommandController(CDeviceExtensionManager& deviceExtensionManager):
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_Surface(VK_NULL_HANDLE),
	m_SwapchainGenerator(deviceExtensionManager),
	m_RenderpassGenerator(),
	m_FramebufferGenerator(),
	m_PipelineGenerator(),
	m_SynchroGenerator()
{
}

SwapGraphicCommandController::~SwapGraphicCommandController()
{
}

void SwapGraphicCommandController::Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;
	m_Surface = surface;

	//スワップチェーンの作成
	m_SwapchainGenerator.Create(logicalDevice, physicalDevice, surface);
	auto swapchainInfo = m_SwapchainGenerator.GetSwapchainInfo();
	auto extent = swapchainInfo.imageExtent;
	auto swapchainImage = m_SwapchainGenerator.GetImages();


	//コマンドバッファの作成
	m_CommandGenerator.Create(logicalDevice, physicalDevice, swapchainImage.GetSize());

	//レンダーパスの作成
	m_RenderpassGenerator.Create(logicalDevice, swapchainImage.GetFomat());
	auto renderPass = m_RenderpassGenerator.GetRenderpass();

	//パイプラインの作成
	m_PipelineGenerator.Create(logicalDevice, extent, renderPass);
	auto graphicsPipeline = m_PipelineGenerator.GetPipeline();

	//フレームバッファの作成
	m_FramebufferGenerator.Create(logicalDevice, swapchainImage.GetImageViews(), renderPass, extent);
	auto framebuffers = m_FramebufferGenerator.GetFramebuffers();

	//コマンドの記録
	m_CommandGenerator.Create(logicalDevice, physicalDevice, swapchainInfo.minImageCount);
	auto commandBuffers = m_CommandGenerator.GetCommandBuffers();
	//m_CommandGenerator.RecordGraphicCommands(framebuffers, renderPass, windowExtent, graphicsPipeline);

	m_CommandGenerator.DrawFrame(commandBuffers[0], renderPass, framebuffers[0], { {0,0},extent }, graphicsPipeline);
	//WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], windowExtent);


	m_SynchroGenerator.Create(logicalDevice);


}

void SwapGraphicCommandController::DrawFrame()
{

}

void SwapGraphicCommandController::PresentFrame()
{

	// -- 描画したイメージを画面にプレゼント（表示） --
	// 表示のためのプレゼント情報をセット
	vk::PresentInfoKHR presentInfo;
	presentInfo.pNext;
	presentInfo.pResults;
	presentInfo.setImageIndices(imageIndex);	// スワップチェーン内のイメージインデックス
	
	presentInfo.pSwapchains = &swapchain;		// プレゼントするスワップチェーン
	presentInfo.pWaitSemaphores = &renderFinished[currentFrame];// レンダリング完了セマフォを待つ
						
	
	// プレゼントキューを取得して、イメージを表示
	auto presentQueue = logicalDevice.getQueue(queueSelector.GetPresentationIndex(m_SurfaceGenerator.GetSurface()), 0);
	result = presentQueue.presentKHR(presentInfo);
	
	// プレゼントに失敗した場合、エラーメッセージを投げる
	if (result != vk::Result::eSuccess)
	{
		throw std::runtime_error("イメージの表示に失敗しました！");
	}
	
	// フレームインデックスを次に進めます（フレーム数を超えないようにループ）
	currentFrame = (currentFrame + 1) % MAX_FRAME_DRAWS;
}
