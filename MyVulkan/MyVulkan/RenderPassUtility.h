#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "SwapChainUtility.h"

class RenderpassGenerator : public CGeneratorBase
{
public:
	RenderpassGenerator();
	~RenderpassGenerator();

	void Create(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat);
	void Destroy(vk::Device logicalDevice);

	vk::RenderPass							GetRenderpass();

private:
	vk::Device								m_LogicalDevice;
	vk::AttachmentDescription				m_ColorAttachment;
	vk::AttachmentReference					m_ColorAttachmentRef;
	vk::SubpassDescription					m_Subpass;
	std::vector<vk::SubpassDependency>		m_Dependencies;
	vk::RenderPassCreateInfo				m_RenderPassInfo;
	vk::RenderPass							m_RenderPass;

	vk::RenderPass							CreateRenderpass(vk::Device logicalDevice, const vk::SurfaceFormatKHR imageFormat);
	std::vector<vk::AttachmentDescription>	CreateColorAttachment(const vk::SurfaceFormatKHR imageFormat);
	std::vector<vk::SubpassDescription>		CreateSubpass();
	std::vector<vk::SubpassDependency>		CreateDependencies();

	vk::RenderPassCreateInfo				CreateInfo(	std::vector<vk::AttachmentDescription>& colorAttachment, 
														std::vector<vk::SubpassDescription>& subpass, 
														std::vector<vk::SubpassDependency>& dependencies);

};

// �����_�[�p�X�ɂ���
// �`��̏����菇���`����I�u�W�F�N�g
// �A�^�b�`�����g�A�T�u�p�X�A�T�u�p�X�ˑ��֌W��3�̗v�f�ō\������Ă���B

// �A�^�b�`�����g(�摜�f�[�^)�ɂ���
// �����_�[�p�X�ň����摜�f�[�^�̏ڍׂ��`���邽�߂̍\����

// fomat
// �摜�̌`���t�H�[�}�b�g�B(�F�̎�ށA�r�b�g�[�x)
// �`��Ώۂ̉摜�f�[�^�Ɠ������̂�����B
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html

// samples
// �}���`�T���v�����O�̃T���v�����B
// (�摜�̐F�̕��ς��ׂ�������āA���̗̂֊s���ڂ����Z�p�B���l���傫���قǌv�Z�ʂ�������)
// 1�`64�܂�2�ׂ̂����ōD���Ȓl������B�`��Ώۂ̉摜�f�[�^�Ɠ������̂�����B

// loadOp�Ƃ�
// �`����n�߂�O�̉摜���ǂ��������̏��B
// �摜���Y��Ƀ��Z�b�g���ď����������A
// �O�ɏ��������ɏォ�珑�����A�Ƃ��������e�B
// 
// VK_ATTACHMENT_LOAD_OP_LOAD		: �O��̕`��œ����Ă����摜�f�[�^�����̂܂܎g���B
// VK_ATTACHMENT_LOAD_OP_CLEAR		: �l���w�肵���l(�ʏ��0)�ŏ���������B
// VK_ATTACHMENT_LOAD_OP_DONT_CARE	: �O��̕`��œ����Ă����摜�f�[�^�𖳎�����B������Ԃ��֌W�Ȃ����Ɏg���B
// VK_ATTACHMENT_LOAD_OP_NONE_KHR	: �O��̕`��œ����Ă����摜�f�[�^�𖳎�����B(�ڍׂ͒m���)
// ���f�o�C�X��VK_KHR_maintenance2�g���@�\�ŗ��p�ł���
// (VK_ATTACHMENT_LOAD_OP_NONE_EXT)	: �O��̕`��œ����Ă����摜�f�[�^�𖳎�����B(�ڍׂ͒m���)
// ���f�o�C�X��VK_EXT_attachment_feedback_loop�g���@�\�ŗ��p�ł���

// storeOp�Ƃ�
// �`�悵���摜�f�[�^���ǂ��������̏��B
// ���e��ۑ����邩�A���Ȃ����Ƃ��������e�B
// 
// VK_ATTACHMENT_STORE_OP_STORE		:�`�挋�ʂ�ۑ�����
// VK_ATTACHMENT_STORE_OP_DONT_CARE	:�`�挋�ʂ�ۑ����Ȃ��B
// VK_ATTACHMENT_STORE_OP_NONE		:�A�^�b�`�����g�̓��e��ۑ����Ȃ��B(�ڍׂ͒m���)
// VK_ATTACHMENT_STORE_OP_NONE_KHR  :�A�^�b�`�����g�̓��e��ۑ����Ȃ��B(�ڍׂ͒m���)
// ���f�o�C�X��VK_KHR_maintenance2�g���@�\�ŗ��p�ł���
// VK_ATTACHMENT_STORE_OP_NONE_QCOM :�A�^�b�`�����g�̓��e��ۑ����Ȃ��B(�ڍׂ͒m���)
// ���f�o�C�X��VK_QCOM_render_pass_transform�g���@�\�ŗ��p�ł���
// VK_ATTACHMENT_STORE_OP_NONE_EXT  :�A�^�b�`�����g�̓��e��ۑ����Ȃ��B(�ڍׂ͒m���)
// ���f�o�C�X��VK_EXT_attachment_feedback_loop�g���@�\�ŗ��p�ł���

// �X�e���V���Ƃ�
// 3D�`��ɂ����āA�}�X�N�����̂悤��
// �`�悷��ꏊ��A���Ȃ��ꏊ�̉摜���

// ���C�A�E�g�Ƃ�
// �摜�f�[�^���`�悳���O��ŁA�ǂ̂悤�Ƀ������ɔz�u����邩�̒�`���B
// 
// VK_IMAGE_LAYOUT_UNDEFINED				:�g����������`�ȃ��C�A�E�g
// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL	:�`�挋�ʂ�ۑ����郌�C�A�E�g
// VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL	:�V�F�[�_�[���摜��ǂݎ�邾���ȃ��C�A�E�g
// VK_IMAGE_LAYOUT_PRESENT_SRC_KHR			:��ʂɉ摜��\�����郌�C�A�E�g

// 
// 
// 
// 
// �T�u�p�X�ɂ���
// 
// 
// 
// �T�u�p�X�ˑ��֌W�ɂ���
// 
// 
// 
// 
// 
// 







































