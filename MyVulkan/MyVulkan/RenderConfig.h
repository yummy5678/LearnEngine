#pragma once
#include "MeshObject.h"
#include "PipelineShaderCreator.h"
#include "GraphicsPipelineCreator.h"

// �V�[���N���X����Vulkan�̃O���t�B�b�N�p�C�v���C���ւ�
// �`�惂�f���̏��̋��n�����s���N���X

// �V�F�[�_�[��`�掞�ׂ̍����ݒ��
// �����Ō��߂���悤�ɂ������B

// �܂����N���X
class RenderConfig
{
public:
	RenderConfig();
	~RenderConfig();

	void Initialize();

	void Destroy();

	vk::Rect2D GetExtent2D();
	vk::Pipeline GetPipeline();
	std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();
	Scene* GetPScene();


private:
	// ��ʃT�C�Y
	vk::Offset2D			m_Offset;	//�`��͈͂̎n�_
	vk::Extent2D			m_Extent;	//�`��͈͂̏I�_

	RenderingPipelineCreator	m_GraphicsPipeline;

	//�p�C�v���C���ɓn���V�F�[�_�[���̍쐬�N���X
	PipelineShaderCreator	m_Shader;	


	Scene* m_pRenderScene;




};

