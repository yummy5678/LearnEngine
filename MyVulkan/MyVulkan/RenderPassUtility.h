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
// �`��̏����������`������񂪓���I�u�W�F�N�g
// �A�^�b�`�����g�A�T�u�p�X�A�T�u�p�X�ˑ��֌W��3�̗v�f�ō\������Ă���B
// 
// �A�^�b�`�����g�ɂ���
// �쐬�֐��̃A�^�b�`�����g�̒l��nullptr�ɂ��Ă��\��Ȃ����A
// ��{�I�ɂ͉�������̒l������
// fomat
// �摜�̌`���t�H�[�}�b�g�B�`��Ώۂ̉摜�f�[�^�Ɠ������̂�����B
// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkFormat.html
// 
// samples
// �}���`�T���v�����O�̃T���v�����B
// (�摜�̐F�̕��ς��ׂ�������āA���̗̂֊s���ڂ����Z�p�B���l���傫���قǌv�Z�ʂ�������)
// 1�`64�܂�2�ׂ̂����ōD���Ȓl������B�`��Ώۂ̉摜�f�[�^�Ɠ������̂�����B
// 
// loadOp
// �����_�[�p�X�J�n���ɃA�^�b�`�����g���ǂ������������߂�B
// VK_ATTACHMENT_LOAD_OP_LOAD		: �A�^�b�`�����g�̊����̓��e�����̂܂܎g�p���܂��B
// VK_ATTACHMENT_LOAD_OP_CLEAR		: �����_�[�p�X�J�n���ɃA�^�b�`�����g���w�肵���l(�N���A�J���[��[�x�l)�ŃN���A���܂��B
// VK_ATTACHMENT_LOAD_OP_DONT_CARE	: �A�^�b�`�����g�̏������e���C�ɂ��Ȃ����Ƃ������܂��B
// VK_ATTACHMENT_LOAD_OP_NONE_KHR	:�A�^�b�`�����g�̓��e�����킸�A����̏�����������s��Ȃ����Ƃ������܂��B
// ���f�o�C�X��VK_KHR_maintenance2�g���@�\�ŗ��p�ł���
// (VK_ATTACHMENT_LOAD_OP_NONE_EXT)	:�A�^�b�`�����g�̓��e�����킸�A����̏�����������s��Ȃ����Ƃ������܂��B
// ���f�o�C�X��VK_EXT_attachment_feedback_loop�g���@�\�ŗ��p�ł���
// 
// storeOp
// �����_�[�p�X�̏I�����ɕ`�挋�ʂ�ێ����邩�ǂ������߂�
// VK_ATTACHMENT_STORE_OP_STORE		:���e�������_�[�p�X�I�����ɕۑ�����B�o�͂Ƀf�[�^�������p���K�v������B
// VK_ATTACHMENT_STORE_OP_DONT_CARE	:�A�^�b�`�����g�̓��e��ۑ����Ȃ��B�����_�[�p�X�I�����Ƀf�[�^�͖����ɂȂ�B
// VK_ATTACHMENT_STORE_OP_NONE		:�A�^�b�`�����g�̓��e��ۑ����Ȃ��B
// (VK_ATTACHMENT_STORE_OP_NONE_KHR):�A�^�b�`�����g�̓��e��ۑ����Ȃ��B
// ���f�o�C�X��VK_KHR_maintenance2�g���@�\�ŗ��p�ł���
//(VK_ATTACHMENT_STORE_OP_NONE_QCOM):�A�^�b�`�����g�̓��e��ۑ����Ȃ��B
// ���f�o�C�X��VK_QCOM_render_pass_transform�g���@�\�ŗ��p�ł���
// (VK_ATTACHMENT_STORE_OP_NONE_EXT):�A�^�b�`�����g�̓��e��ۑ����Ȃ��B
// ���f�o�C�X��VK_EXT_attachment_feedback_loop�g���@�\�ŗ��p�ł���
// 
// stencilLoadOp
// VK_ATTACHMENT_LOAD_OP_LOAD		:�X�e���V���o�b�t�@�̊����̓��e�����̂܂܎g�p����B
// VK_ATTACHMENT_LOAD_OP_CLEAR		:�X�e���V���o�b�t�@�̓��e�������_�[�p�X�J�n���Ɏw�肵���l�ŏ���������
// VK_ATTACHMENT_LOAD_OP_DONT_CARE	:�X�e���V���o�b�t�@���g�p���Ȃ�
// VK_ATTACHMENT_LOAD_OP_NONE_EXT	:
// ���f�o�C�X��VK_EXT_attachment_feedback_loop�g���@�\�ŗ��p�ł���
// VK_ATTACHMENT_LOAD_OP_NONE_KHR	:
// ���f�o�C�X��VK_KHR_maintenance2�g���@�\�ŗ��p�ł���
// 
// stencilStoreOp
// VK_ATTACHMENT_STORE_OP_STORE		:�����_�[�p�X�I�����ɃX�e���V���o�b�t�@�̓��e�����ɕۑ�����B
// VK_ATTACHMENT_STORE_OP_DONT_CARE	:�X�e���V���o�b�t�@�̓��e�����ɕۑ����Ȃ��B
// VK_ATTACHMENT_STORE_OP_NONE		:�X�e���V���o�b�t�@���g�p���Ȃ��B
// VK_ATTACHMENT_STORE_OP_NONE_KHR	:
// ���f�o�C�X��VK_KHR_maintenance2�g���@�\�ŗ��p�ł���
// VK_ATTACHMENT_STORE_OP_NONE_QCOM	:
// ���f�o�C�X��VK_QCOM_render_pass_transform�g���@�\�ŗ��p�ł���
// VK_ATTACHMENT_STORE_OP_NONE_EXT	:
// ���f�o�C�X��VK_EXT_attachment_feedback_loop�g���@�\�ŗ��p�ł���
// 
// initialLayout
// 
// 
// 
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







































