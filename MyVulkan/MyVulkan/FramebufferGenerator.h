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

	void LoadShader(vk::Device logicalDevice, std::vector<vk::ImageView> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);
	void LoadShader(vk::Device logicalDevice, std::vector<std::vector<vk::ImageView>> imageViews, vk::RenderPass renderPass, vk::Extent2D extent);



	std::vector<vk::Framebuffer>	GetFramebuffers();
private:
	vk::Device								m_LogicalDevice;
	std::vector<vk::Framebuffer>			m_Framebuffers;
	std::vector<vk::FramebufferCreateInfo>	m_FramebufferInfos;

	void Destroy();
	std::vector<std::vector<vk::ImageView>> CreateAttachments(std::vector<vk::ImageView> imageViews);
	std::vector<std::vector<vk::ImageView>> CreateAttachments(std::vector<std::vector<vk::ImageView>> imageViews);
	std::vector<vk::FramebufferCreateInfo> CreateFramebufferInfos(std::vector<std::vector<vk::ImageView>>& attachments, vk::RenderPass renderPass, vk::Extent2D extent);

};

// �t���[���o�b�t�@�ɂ���
// �`�挋�ʏo�͐�̏���ێ�����I�u�W�F�N�g�B
// �����_�[�p�X�ł̓A�^�b�`�����g(�������̈�)�ɂǂ̂悤�ȏ������{�������`�������A
// �t���[���o�b�t�@�ł̓A�^�b�`�����g(�������̈�)��
// �����_�[�p�X�ŋL�q��������(�T�u�p�X)�̏������т��A
// ���ۂɕ`�挋�ʂ�ێ�����������ʂ����B
//  
// renderPass�ɂ���
// �ǂ̃����_�[�p�X�ƕR�Â������Ȃ̂��ݒ肷��ׂ̒l�B
//  
// �A�^�b�`�����g(�f�[�^)�ɂ���
// �f�[�^�̏o�͐�B
// �ŏI�I�ɕ\������p�̐F�f�[�^�A���̕\���p�̐F�f�[�^�A
// �[�x���A�}�X�N��񓙂̂悤��
// �����̃A�^�b�`�����g��ݒ肷�邱�Ƃ��ł���B
// ����͍ŏI�\���p�̐F�f�[�^��������΂����̂�
// �A�^�b�`�����g�͈�����B
// 
// width�Eheight�ɂ���
// �t���[���o�b�t�@�̕��ƍ����B
// �C���[�W�̕��ƍ����Ɠ����łȂ���΂Ȃ�Ȃ��B
// 
// layers�ɂ���
// �`�悷�鎋�_�̐�(?)
// VR�Q�[���ō��E�̎��_�����ꍇ�Ȃǂ́u2�v���������肷��B
// 2D��P����3D�\���Ȃ�u1�v�ł����B
// 
// 
// 
// 
// 