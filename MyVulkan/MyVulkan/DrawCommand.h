#pragma once
#include <vulkan/vulkan.hpp>
#include "NonCopyable.h"
#include "QueueUtility.h"
#include "RenderObject.h"
//#include "SemaphoreGenerator.h"
//#include "FenceGenerator.h"
#include "VertexBuffer.h"
#include "ImageSet.h"
#include "RendererBase.h"
#include "SwapchainRenderer.h"


class DrawCommand : public NonCopyable
{
public:
	DrawCommand();
	~DrawCommand();
	DrawCommand(DrawCommand&&) noexcept = default;
	DrawCommand& operator=(DrawCommand&&) noexcept = default;

	// 作成関数
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	void Destroy();

	vk::CommandBuffer GetBuffer();

	void BeginRendering(RenderingImageSet* imageSet, vk::Rect2D renderArea);
	void EndRendering(
		vk::Fence submitFence,
		std::vector<vk::Semaphore>* waitSemaphores,
		std::vector<vk::Semaphore>* signalSemaphores,
		vk::ImageLayout newImageLayout);

	vk::Semaphore GetRenderFinishedSemaphore();


private:
	vk::Device						m_LogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::SwapchainKHR				m_Swapchain;

	QueueFamilySelector				m_QueueSelector;

	RenderingImageSet*				m_ImageSet;

	vk::CommandPool					m_CommandPool;		//コマンドプール
	vk::CommandBuffer				m_CommandBuffer;	//コマンドバッファ

	// 同期オブジェクト
	vk::Semaphore					m_RenderFinishedSemaphore;

	// 描画処理の完了を知らせるためのセマフォ
	//vk::Semaphore					m_ImageAvailableSemaphores;
	//vk::Fence						m_Fences;

	void CreateSemaphore(vk::Semaphore& semaphore);
	void CreateFence(vk::Fence& fence);

	//コマンドプールの作成
	void CreateCommandPool();

	//コマンドバッファの作成(コマンドプールの割り当て)
	void CreateCommandBuffers(uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo CreateSubmitInfo(
		std::vector<vk::PipelineStageFlags>* waitStages,
		std::vector<vk::Semaphore>* imageAvailableSemaphore,
		std::vector<vk::Semaphore>* WaitSemaphore);

	void ImageMemoryBarrier(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

};

