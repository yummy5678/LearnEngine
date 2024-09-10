#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "SemaphoreGenerator.h"
#include "FenceGenerator.h"

class CommandGenerator : public CGeneratorBase
{
public:
	CommandGenerator();
	~CommandGenerator();

	// 作成関数
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize);

	// 破棄関数
	void Destroy();

	// コマンドの作成
	void RecordGraphicCommands(std::vector<vk::Framebuffer> framebuffers, vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);
	void RecordGraphicCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);

	// 作成されたコマンドプールの取得
	vk::CommandPool GetCammandPool();

	// 作成されたコマンドバッファ配列の取得
	std::vector<vk::CommandBuffer> GetCommandBuffers();

	// GPU内で画像を描画
	void DrawFrame(vk::CommandBuffer buffer, vk::RenderPass renderpass, vk::Framebuffer framebuffer, vk::Rect2D renderArea, vk::Pipeline graphicsPipeline);

	// スワップチェーンに従って画像を表示
	void PresentFrame(vk::SwapchainKHR swapchain);


private:
	vk::Device						m_LogicalDevice;	
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::CommandPool					m_CommandPool;		//コマンドプール
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//コマンドバッファ

	// 同期オブジェクト
	std::vector<vk::Semaphore>		m_SignalSemaphores;
	std::vector<vk::Semaphore>		m_WaitSemaphores;
	std::vector<vk::Fence>			m_Fences;

	SemaphoreGenerator				m_SemaphoreGenerator;
	FenceGenerator					m_FenceGenerator;

	//コマンドプールの作成
	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	//コマンドバッファの作成(コマンドプールの割り当て)
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo					CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores);
	vk::SubmitInfo					CreateSubmitInfo(vk::CommandBuffer& commandBuffer);

	uint32_t						AcquireSwapchainNextImage(vk::SwapchainKHR swapchain);

};

// コマンドバッファについて
// GPUに送る命令の書かれたオブジェクト。
// GPUにコマンド(命令)を送るには
// まず、コマンドプール(メモリ上のコマンドを置く為の領域)を確保。
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
//