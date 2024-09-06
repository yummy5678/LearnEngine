#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "SemaphoreGenerator.h"

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

	void DrawFrame(vk::CommandBuffer buffer, vk::RenderPass renderpass, vk::Framebuffer framebuffer, vk::Rect2D renderArea, vk::Pipeline graphicsPipeline);


private:
	vk::Device						m_LogicalDevice;	
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::CommandPool					m_CommandPool;		//コマンドプール
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//コマンドバッファ

	SemaphoreGenerator				m_SignalSemaphore;
	SemaphoreGenerator				m_RenderWaitSemaphore;

	//コマンドプールの作成
	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	//コマンドバッファの作成(コマンドプールの割り当て)
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo					CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores);
	vk::SubmitInfo					CreateSubmitInfo(vk::CommandBuffer& commandBuffer);
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