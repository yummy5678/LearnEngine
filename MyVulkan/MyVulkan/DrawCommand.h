#pragma once
#include <vulkan/vulkan.hpp>
#include "QueueUtility.h"
#include "RenderObject.h"
#include "SemaphoreGenerator.h"
#include "FenceGenerator.h"
#include "VertexBuffer.h"
#include "ImageSet.h"


class DrawCommand
{
public:
	DrawCommand();
	~DrawCommand();

	// �쐬�֐�
	void Create(vk::Device* pLogicalDevice, vk::PhysicalDevice* pPhysicalDevice, std::vector<ImageViewSet> imageSet);

	// �j���֐�
	void Destroy();

	vk::CommandBuffer GetBuffer();

	void BeginRendering(uint32_t index, vk::Rect2D renderArea);
	void EndRendering();

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
	vk::Device*						m_pLogicalDevice;
	vk::PhysicalDevice*				m_pPhysicalDevice;

	QueueFamilySelector				m_QueueSelector;

	// �摜�̑g������������Ƃ�(��ɃX���b�v�`�F�C��)�̕`��p�C���f�b�N�X
	uint32_t						m_ImageDrawIndex;

	std::vector<ImageViewSet>		m_ImageSet;

	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//�R�}���h�o�b�t�@

	// �����I�u�W�F�N�g
	std::vector<vk::Semaphore>		m_SignalSemaphores;
	std::vector<vk::Semaphore>		m_WaitSemaphores;
	std::vector<vk::Fence>			m_Fences;

	SemaphoreGenerator				m_SemaphoreGenerator;
	FenceGenerator					m_FenceGenerator;



	//�R�}���h�v�[���̍쐬
	void CreateCommandPool(vk::Device* pLogicalDevice);

	//�R�}���h�o�b�t�@�̍쐬(�R�}���h�v�[���̊��蓖��)
	void CreateCommandBuffers(vk::Device* pLogicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo					CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores);
	vk::SubmitInfo					CreateSubmitInfo(vk::CommandBuffer& commandBuffer);

	//uint32_t						AcquireSwapchainNextImage(vk::SwapchainKHR swapchain);









};

