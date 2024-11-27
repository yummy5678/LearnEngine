#pragma once
#include "MeshObject.h"
#include "RenderObject.h"
#include "PipelineShaderCreator.h"
#include "RenderingPipelineCreator.h"
#include "VulkanInitializer.h"

// �V�[���N���X����Vulkan�̃O���t�B�b�N�p�C�v���C���ւ�
// �`�惂�f���̏��̋��n�����s���N���X

// �V�F�[�_�[��`�掞�ׂ̍����ݒ��
// �����Ō��߂���悤�ɂ������B

// �܂����N���X
class RenderConfig
{
public:
	RenderConfig(VulkanInitializer& initializer);
	~RenderConfig();

	void Initialize(
		vk::Device logicalDevice,
		vk::Extent2D extent,
		vk::Format colorFomat,
		vk::Format depthFomat);

	void Destroy();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();
	std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();


private:
	// ��ʃT�C�Y
	vk::Offset2D			m_Offset;	//�`��͈͂̎n�_
	vk::Extent2D			m_Extent;	//�`��͈͂̏I�_

	vk::Rect2D				m_RenderArea;	// �t���[�����̕`��ʒu�̋�`

	RenderingPipelineCreator m_GraphicsPipeline;

	//�p�C�v���C���ɓn���V�F�[�_�[���̍쐬�N���X
	PipelineShaderCreator	m_Shader;
};
