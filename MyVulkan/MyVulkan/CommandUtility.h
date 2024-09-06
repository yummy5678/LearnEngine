#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "QueueUtility.h"

class CommandGenerator : public CGeneratorBase
{
public:
	CommandGenerator();
	~CommandGenerator();

	// �쐬�֐�
	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t commandSize);

	// �j���֐�
	void Destroy();

	// �R�}���h�̍쐬
	void RecordGraphicCommands(std::vector<vk::Framebuffer> framebuffers, vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);
	void RecordGraphicCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);

	// �쐬���ꂽ�R�}���h�v�[���̎擾
	vk::CommandPool GetCammandPool();

	// �쐬���ꂽ�R�}���h�o�b�t�@�z��̎擾
	std::vector<vk::CommandBuffer> GetCommandBuffers();

	void DrawFrame(vk::RenderPass renderpass, vk::Framebuffer framebuffer, vk::Rect2D renderArea, vk::Pipeline graphicsPipeline);


private:
	vk::Device						m_LogicalDevice;	
	vk::PhysicalDevice				m_PhysicalDevice;
	//QueueFamilySelector				m_QueueFamily;
	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	std::vector<vk::CommandBuffer>	m_CommandBuffers;	//�R�}���h�o�b�t�@

	//�R�}���h�v�[���̍쐬
	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);

	//�R�}���h�o�b�t�@�̍쐬(�R�}���h�v�[���̊��蓖��)
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

	vk::SubmitInfo					CreateSubmitInfo(vk::CommandBuffer commandBuffer);
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