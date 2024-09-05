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
	void Create(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice, uint32_t commandSize);

	// �j���֐�
	void Destroy();

	// �R�}���h�̍쐬
	void RecordGraphicCommands(std::vector<vk::Framebuffer> framebuffers, vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);
	void RecordGraphicCommands(vk::RenderPass renderPass, vk::Extent2D extent, vk::Pipeline graphicsPipeline);

	// �쐬���ꂽ�R�}���h�v�[���̎擾
	vk::CommandPool GetCammandPool();

	// �쐬���ꂽ�R�}���h�o�b�t�@�z��̎擾
	std::vector<vk::CommandBuffer> GetCommandBuffers();


private:
	vk::Device						m_LogicalDevice;	//�j���p�ɘ_���f�o�C�X�̏��������Ă���
	vk::CommandPool					m_CommandPool;		//�R�}���h�v�[��
	std::vector<vk::CommandBuffer>	m_Buffers;			//�R�}���h�o�b�t�@

	//�R�}���h�v�[���̍쐬
	vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice phygicalDevice);

	//�R�}���h�o�b�t�@�̍쐬(�R�}���h�v�[���̊��蓖��)
	std::vector<vk::CommandBuffer> CreateCommandBuffers(vk::Device logicalDevice, uint32_t commandSize, vk::CommandPool commandPool);

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