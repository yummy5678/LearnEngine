#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"
#include "ShaderUtility.h"
#include "ViewportGenerator.h"

class PipelineGenerator : CGeneratorBase
{
public:
	PipelineGenerator();
	~PipelineGenerator();

	void Create(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);
	void Destroy(vk::Device logicalDevice);

	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();

private:
	vk::Device						m_LogicalDevice;

	vk::Pipeline					m_Pipeline;
	//vk::GraphicsPipelineCreateInfo	m_PipelineCreateInfo;

	vk::PipelineLayout				m_PipelineLayout;
	//vk::PipelineLayoutCreateInfo	m_PipelineLayoutCreateInfo;


	//std::vector<vk::PipelineShaderStageCreateInfo>	m_ShaderStageInfos;
	//vk::PipelineMultisampleStateCreateInfo			m_MultisamplingInfo;
	//vk::PipelineVertexInputStateCreateInfo			m_VertexInputCreateInfo;
	//vk::PipelineInputAssemblyStateCreateInfo			m_InputAssemblyInfo;
	ViewportGenerator									m_viewportGenerator;
	//vk::PipelineRasterizationStateCreateInfo			m_RasterizerCreateInfo;
	//std::vector<vk::PipelineColorBlendAttachmentState>m_ColorBlendAttachment;
	//vk::PipelineColorBlendStateCreateInfo				m_ColorBlendCreateInfo;

	vk::Pipeline CreateGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);


	vk::PipelineLayout CreatePipelineLayout(vk::Device logicalDevice);

	std::vector<vk::PipelineShaderStageCreateInfo>		GetShaderStageInfo(vk::Device logicalDevice);
	vk::PipelineVertexInputStateCreateInfo				GetVertexInputStateInfo();
	vk::PipelineInputAssemblyStateCreateInfo			GetInputAssemblyStateInfo();
	vk::PipelineRasterizationStateCreateInfo			GetRasterizationStateInfo();

	vk::PipelineMultisampleStateCreateInfo				GetMultisampleStateInfo();
	std::vector<vk::PipelineColorBlendAttachmentState>	GetColorBlendAttachmentState();
	vk::PipelineColorBlendStateCreateInfo				GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* colorBlendAttachment);

};

// �O���t�B�b�N�X�p�C�v���C���ɂ���
// ���_������͂��Ă���摜�̐F���ɕϊ�����܂ł̏����̗�������߂�I�u�W�F�N�g�B
// �O���t�B�b�N�X�p�C�v���C���̍쐬�ɂ̓����_�[�p�X���K�{�B
// �����̃T�u�p�X��g�ݍ��킹�ĕ`���g�ݍ��킹�邱�Ƃ��ł���B
// �ȉ��̗���ŏ���ϊ������B
// 
// 1.���_���̓���(���̓A�Z���u��)
// ���_�̃f�[�^��ǂݍ��ށB
// �C���f�b�N�X�o�b�t�@�[�Ɋi�[����B
// 
// 2.���_�V�F�[�_�[
// ���͂������_���(�C���f�b�N�X�o�b�t�@�[)�����ʏ�̕\���ʒu���v�Z����B
// 3D�I�u�W�F�N�g�̈ړ����]�A�g��k���Ȃǂ̌v�Z�͂����ōs���B
// ���̃V�F�[�_�[�͕K���p�ӂ���K�v������B
// 
// 3.�e�b�Z���[�V�����E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
// �W�I���g��(���̂̌`��)���ו������āA�I�u�W�F�N�g�̕\�ʂ����炩�ɂ�����A���������B
// �e�b�Z���[�V��������V�F�[�_: �e�b�Z���[�V�����ׂ̍��������߂�B
// �e�b�Z���[�V�����G���W��: �p�b�`���ׂ�����������B(���̏����̓n�[�h�E�F�A(GPU)���Ŏ����I�ɂ���Ă����B)
// �e�b�Z���[�V�����]���V�F�[�_(�h���C���V�F�[�_): �V�������_�̈ʒu���v�Z����B
// ���̏����̓I�v�V�����Ȃ̂Ŏg�p���Ȃ����Ƃ��ł���B
// 
// 4.�W�I���g���[�V�F�[�_�[
// �v���~�e�B�u(�O�p�`�Ȃǂ̌v�Z�Ɏg���₷����{�}�`)�ɑ΂��Ēǉ��ŉ��H��������B
// ���_���𑝌������āA�V�����`��ɂ�����ł���B
// �d���������Ȃ̂ł��܂葽�p����Ȃ��B
// ���̏����̓I�v�V�����Ȃ̂Ŏg�p���Ȃ����Ƃ��ł���B
// 
// 5.���X�^�[��
// 3D��ԂŒ�`���ꂽ�W�I���g��(���_��|���S���Ȃ�)��
// 2D�̃t���O�����g(�s�N�Z���ɑΉ�����f�[�^)�ɕϊ�����B
// ���̏����̓n�[�h�E�F�A(GPU)���Ŏ����I�ɂ���Ă����B
// 
// 6.�t���O�����g�V�F�[�_�[(�s�N�Z���V�F�[�_�[)
// �_(�s�N�Z��)���ꂼ��ɕt����F���v�Z����V�F�[�_�[�B
// ��{�I�ɖʂ̕�����ގ��A���_����̈ʒu���̏����l�����Čv�Z����B
// ���̃V�F�[�_�[�͕K���p�ӂ���K�v������B
// 
// 7.�J���[�o�� (Color Output)
// ��̃t���O�����g�V�F�[�_�[�ŏo�������ʂ�
// �t���[���o�b�t�@�̃f�[�^�Ɏ��ۂɏ������ށB
// ���̏����̓n�[�h�E�F�A(GPU)���Ŏ����I�ɂ���Ă����B
// 


// �V�F�[�_�[�Ƃ�
// GPU�œ������p�̃v���O�����B
// ���X�͉A�e�̌v�Z�Ɏg�����߂̕��������ׁA���̂悤�Ȗ��O�ɂȂ��Ă���B
// Vulkan�ł�HLSL���GLSL���̃V�F�[�_�[�p�̌���ŏ������v���O������
// SPIR-V�Ƃ������Ԍ���(�o�C�i���`���̃R�[�h)�ɃR���p�C�����Ďg�p����B
// �O���t�B�b�N�X�p�C�v���C���ł͒��_�E�s�N�Z���V�F�[�_�[��2��
// �Œ���A�����ŗp�ӂ��Ȃ���΂Ȃ�Ȃ��B
// �f�t�H���g�̃V�F�[�_�[�Ƃ��͗p�ӂ���Ă��Ȃ��B


// SPIR-V�Ƃ�
//�uStandard Portable Intermediate Representation - Version V�v
// (�W���I�ňڐA�\�Ȓ��ԕ\��)�̗��B
// Vulkan�ł̓V�F�[�_�[�̃R�[�h�����̂܂܎g�p���邱�Ƃ͏o���Ȃ����߁A
// �R���p�C������K�v������B
// �R���p�C���������̂�SPIR-V�B
// Vulkan SDK�ɕt����glslangValidator.exe����R���p�C���ł���B
// SPIRV-Reflect : https://github.com/KhronosGroup/SPIRV-Reflect
// SPIRV-Cross	 : https://github.com/KhronosGroup/SPIRV-Cross
// 








