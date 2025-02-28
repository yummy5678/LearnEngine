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
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, RenderingImageSet* imageSet);
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, SwapchainRenderer* swapchainRenderer);
	void Destroy();

	vk::CommandBuffer GetBuffer();

	void BeginRendering(vk::Rect2D renderArea);
	void EndRendering();

	vk::Semaphore GetImageAvableSemaphore();
	vk::Fence GetFence();

	uint32_t GetCurrentIndex();

	// GPU���ŉ摜��`��
	//void DrawFrame(		
	//	vk::PipelineLayout pipelineLayout,
	//	std::vector<vk::DescriptorSet>* descriptorSets,
	//	RenderObject* object,
	//	SceneCamera* camera);


	// ���b�V���I�u�W�F�N�g��������d�����ĕ`��R�}���h�𔭍s����܂ł̗����Z�߂�����
	//void RenderMesh(
	//	vk::Pipeline pipeline, 
	//	vk::PipelineLayout pipelineLayout,
	//	std::vector<vk::DescriptorSet>* descriptorSets,
	//	VMeshObject* drawMeshes,
	//	Transform* ObjectTransform);
private:
	vk::Device						m_LogicalDevice;
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::SwapchainKHR				m_Swapchain;

	QueueFamilySelector				m_QueueSelector;

	// �摜�̑g������������Ƃ�(��ɃX���b�v�`�F�C��)�̕`��p�C���f�b�N�X
	uint32_t						m_CurrentIndex;	// ������`�悷��C���[�W�C���f�b�N�X
	uint32_t						m_NextIndex;	// ���t���[���ŕ`�悷��C���[�W�C���f�b�N�X



	std::vector<RenderingImageSet>		m_ImageSet;

	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//�R�}���h�o�b�t�@

	// �����I�u�W�F�N�g
	std::vector<vk::Semaphore>		m_SignalSemaphores;
	std::vector<vk::Semaphore>		m_ImageAvailableSemaphores;
	std::vector<vk::Fence>			m_Fences;

	//SemaphoreGenerator				m_SemaphoreGenerator;
	//FenceGenerator					m_FenceGenerator;

	void CreateSemaphore(vk::Semaphore& semaphore);
	void CreateFence(vk::Fence& fence);

	//�R�}���h�v�[���̍쐬
	void CreateCommandPool();

	//�R�}���h�o�b�t�@�̍쐬(�R�}���h�v�[���̊��蓖��)
	void CreateCommandBuffers(uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo CreateSubmitInfo(std::vector<vk::PipelineStageFlags>* waitStages);

	uint32_t AcquireSwapchainNextImage();
	void ImageMemoryBarrier(vk::ImageLayout oldLayout, vk::ImageLayout newLayout);
	//uint32_t						AcquireSwapchainNextImage(vk::SwapchainKHR swapchain);









};

