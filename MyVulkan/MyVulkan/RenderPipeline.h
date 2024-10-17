#pragma once
#include "MeshObject.h"
#include "PipelineShaderCreator.h"
#include "GraphicsPipelineCreator.h"
#include "DescriptorHandler.h"

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
	void updateModel(MeshObject* mesh, glm::mat4 transform);
	void Destroy();

	vk::Extent2D			GetExtent2D();
	PipelineShaderCreator	GetPipelineShader();



private:
	// ��ʃT�C�Y
	vk::Extent2D			m_Extent;

	//�p�C�v���C���ɓn���V�F�[�_�[���̍쐬�N���X
	PipelineShaderCreator	m_Shader;	




	// �f�X�N���v�^
	VDescriptor m_Descriptor;




};

