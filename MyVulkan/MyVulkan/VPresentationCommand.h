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

	// �g�p�ł���X���b�v�`�F�[���̉摜���擾
	const uint32_t	AcquireSwapchainNextImage(vk::Semaphore imageAvailableSemaphore);
private:
	vk::Device						m_LogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::SwapchainKHR				m_Swapchain;
	// �R�}���h�o�b�t�@
	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//�R�}���h�o�b�t�@

	// �����I�u�W�F�N�g
	std::vector<vk::Semaphore>		m_Semaphores;

	uint32_t	m_SwapChainIndex;

	QueueFamilySelector				m_QueueFamily;

	void		CreateCommandPool();
	void		CreateCommandBuffer(size_t commandSize);


};

