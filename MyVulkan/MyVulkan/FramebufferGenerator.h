#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "VulkanUtility.h"
#include "SwapChainUtility.h"

class FramebufferGenerator : public CGeneratorBase
{
public:
	FramebufferGenerator();
	~FramebufferGenerator();

	void Create(vk::Device logicalDevice, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);



	std::vector<vk::Framebuffer>	GetFramebuffers();
private:
	vk::Device								m_LogicalDevice;
	std::vector<vk::Framebuffer>			m_Framebuffers;
	std::vector<vk::FramebufferCreateInfo>	m_FramebufferInfos;
	std::array<vk::ImageView, 1>			m_Attachments;

	void Destroy();
	std::vector<vk::FramebufferCreateInfo> CreateFramebufferInfos(std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);

};

// �t���[���o�b�t�@�ɂ���
// �`�挋�ʏo�͐�̏���ێ�����I�u�W�F�N�g�B
// �����_�[�p�X�ł̓A�^�b�`�����g(�������̈�)�ɂǂ̂悤�ȏ������{�������`�������A
// �t���[���o�b�t�@�ł̓A�^�b�`�����g(�������̈�)��
// �����_�[�p�X�ŋL�q��������(�T�u�p�X)�̏������т��A
// ���ۂɕ`�挋�ʂ�ێ�����������ʂ����B
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