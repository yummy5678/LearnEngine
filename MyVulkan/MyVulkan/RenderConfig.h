#pragma once
#include "MeshObject.h"
#include "RenderObject.h"
#include "PipelineShaderCreator.h"
#include "RenderingPipelineCreator.h"
#include "VulkanInitializer.h"
#include "RenderTargetBase.h"
#include "SceneCamera.h"
#include "GraphicWindow.h"
#include "RenderFunction.h"


// �V�[���N���X����Vulkan�̃O���t�B�b�N�p�C�v���C���ւ�
// �`�惂�f���̏��̋��n�����s���N���X

// �V�F�[�_�[��`�掞�ׂ̍����ݒ��
// �����Ō��߂���悤�ɂ������B

using RenderingObjects = std::vector<RenderObject*>;

// �܂����N���X
class RenderConfig : RendererBase
{
public:
	RenderConfig(VulkanInitializer& initializer);
	~RenderConfig();

	// ������
	void Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, RenderingTarget* renderere);

	// �f�B�X�N���v�^�Ȃǂ��X�V������
	void Update();

	// �����o�ϐ��̏�����
	void Cleanup();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline()			override;
	vk::PipelineLayout	GetPipelineLayout()		override;

	
	std::shared_ptr<RenderFunction> GetRenderFunction(RenderingObjects* pObjects, SceneCamera* pCamera);

private:
	//vk::Device* m_LogicalDevice;
	//vk::PhysicalDevice* m_PhygicalDevice;

	VModelShaderConfiguer	m_Shader;

	std::vector<std::weak_ptr<RenderFunction>>		m_RenderFunction;
	std::unordered_map<void*, RenderingObjects*>	m_pObjects;
	std::unordered_map<void*, SceneCamera*>			m_pCamera;


};
