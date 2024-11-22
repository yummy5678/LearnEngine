#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "SemaphoreGenerator.h"
#include "FenceGenerator.h"
#include "VertexBuffer.h"
#include "RenderConfig.h"
#include "RenderScene.h"



class SwapChainCommandGenerator : public GeneratorBase
{
public:
	SwapChainCommandGenerator();
	~SwapChainCommandGenerator();

	// �쐬�֐�
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize);

	// �j���֐�
	void Destroy();


	// �쐬���ꂽ�R�}���h�v�[���̎擾
	vk::CommandPool GetCammandPool();

	// �쐬���ꂽ�R�}���h�o�b�t�@�z��̎擾
	std::vector<vk::CommandBuffer> GetCommandBuffers();

	// GPU���ŉ摜��`��
	void DrawFrame(uint32_t commandIndex, std::vector<std::pair<RenderConfig&, RenderScene&>> renderingUnit, vk::ImageView colorImage, vk::ImageView depthImage);

	// �X���b�v�`�F�[���ɏ]���ĉ摜��\��
	void PresentFrame(vk::SwapchainKHR swapchain, uint32_t commandIndex);


private:
	vk::Device						m_LogicalDevice;	
	vk::PhysicalDevice				m_PhysicalDevice;
	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//�R�}���h�o�b�t�@

	// �����I�u�W�F�N�g
	std::vector<vk::Semaphore>		m_SignalSemaphores;
	std::vector<vk::Semaphore>		m_WaitSemaphores;
	std::vector<vk::Fence>			m_Fences;

	SemaphoreGenerator				m_SemaphoreGenerator;
	FenceGenerator					m_FenceGenerator;



	//�R�}���h�v�[���̍쐬
	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	//�R�}���h�o�b�t�@�̍쐬(�R�}���h�v�[���̊��蓖��)
	std::vector<vk::CommandBuffer>	CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo					CreateSubmitInfo(std::vector<vk::CommandBuffer>& commandBuffers, std::vector<vk::Semaphore>& signalSemaphores, std::vector<vk::Semaphore>& waitSemaphores);
	vk::SubmitInfo					CreateSubmitInfo(vk::CommandBuffer& commandBuffer);

	uint32_t						AcquireSwapchainNextImage(vk::SwapchainKHR swapchain);

	// ���b�V���I�u�W�F�N�g��������d�����ĕ`��R�}���h�𔭍s����܂ł̗����Z�߂�����
	void RenderObjects(
		vk::CommandBuffer commandBuffer, 
		vk::PipelineLayout pipelineLayout, 
		std::vector<SceneObject> drawMeshes, 
		SceneCamera sceneCamera);

};

// �R�}���h�o�b�t�@�ɂ���
// GPU�ɑ��閽�߂̏����ꂽ�I�u�W�F�N�g�B
// GPU�ɃR�}���h(����)�𑗂�ɂ�
// �܂��A�R�}���h�v�[��(��������̃R�}���h��u���ׂ̗̈�)���m�ہB
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















