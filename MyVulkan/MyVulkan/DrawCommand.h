#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "RenderObject.h"
#include "SemaphoreGenerator.h"
#include "FenceGenerator.h"
#include "VertexBuffer.h"
#include "ImageSet.h"
#include "RendererBase.h"
#include "SwapchainRenderer.h"


class DrawCommand
{
public:
	DrawCommand();
	~DrawCommand();

	// �쐬�֐�
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	void Destroy();

	vk::CommandBuffer GetBuffer();

	void BeginRendering(RenderingImageSet* imageSet, vk::Semaphore imageAvableSemaphore, vk::Rect2D renderArea);
	void EndRendering(vk::Fence fence, vk::ImageLayout newImageLayout);

	vk::Semaphore GetSignalSemaphore();
	//vk::Fence GetFence();

	//uint32_t GetCurrentIndex();
	//void WaitFence();


private:
	vk::Device						m_LogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::SwapchainKHR				m_Swapchain;

	QueueFamilySelector				m_QueueSelector;

	// �摜�̑g������������Ƃ�(��ɃX���b�v�`�F�C��)�̕`��p�C���f�b�N�X
	// ������`�悷��C���[�W�C���f�b�N�X
	//uint32_t						m_CurrentIndex;	// �p�r : �t�F���X�A�Z�}�t�H
	//// ���t���[���ŕ`�悷��C���[�W�C���f�b�N�X
	//uint32_t						m_NextIndex;	// �p�r : �R�}���h�A�摜



	RenderingImageSet*				m_ImageSet;

	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	vk::CommandBuffer				m_CommandBuffers;	//�R�}���h�o�b�t�@

	// �����I�u�W�F�N�g
	vk::Semaphore					m_RenderFinishedSemaphores;

	// �`�揈���̊�����m�点�邽�߂̃Z�}�t�H
	vk::Semaphore					m_ImageAvailableSemaphores;
	//vk::Fence						m_Fences;

	void CreateSemaphore(vk::Semaphore& semaphore);
	void CreateFence(vk::Fence& fence);

	//�R�}���h�v�[���̍쐬
	void CreateCommandPool();

	//�R�}���h�o�b�t�@�̍쐬(�R�}���h�v�[���̊��蓖��)
	void CreateCommandBuffers(uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo CreateSubmitInfo(std::vector<vk::PipelineStageFlags>* waitStages);

	void ImageMemoryBarrier(vk::Image& image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);





};

