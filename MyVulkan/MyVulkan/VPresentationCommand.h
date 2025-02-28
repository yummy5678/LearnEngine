#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "SemaphoreGenerator.h"
#include "VulkanInitializer.h"
//#include "FenceGenerator.h"


class VPresentationCommand
{
public:
	VPresentationCommand();
	~VPresentationCommand();

	void Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainKHR swapchain);
	void RunningCommand(vk::Semaphore imageAvailableSemaphore);
	void Cleanup();

	// 使用できるスワップチェーンの画像を取得
	const uint32_t	AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore);
private:
	vk::Device						m_LogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::SwapchainKHR				m_Swapchain;
	// コマンドバッファ
	vk::CommandPool					m_CommandPool;		//コマンドプール
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//コマンドバッファ

	// 同期オブジェクト
	std::vector<vk::Semaphore>		m_Semaphores;

	uint32_t	m_SwapChainIndex;

	QueueFamilySelector				m_QueueFamily;

	void		CreateCommandPool();
	void		CreateCommandBuffer(size_t commandSize);


};

