#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "GeneratorBase.h"
#include "VulkanValidation.h"

// �C���X�^���X�g���@�\�̃��X�g���쐬����
class InstanceGenerator : public CGeneratorBase
{
public:
	InstanceGenerator();
	~InstanceGenerator();

	void Create();
	void Destroy();

	vk::Instance GetInstanse();

private:
	vk::ApplicationInfo		m_ApplicationInfo;
	vk::InstanceCreateInfo	m_InstanceInfo;
	vk::Instance			m_Instance;

	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	std::vector<const char*> instanceExtensions;

	//�C���X�^���X�̍쐬
	void CreateInstance();

	// GLFW �ŃT�[�t�F�X�����̂ɕK�v�ȃC���X�^���X�g�����擾
	std::vector<const char*>*	GetRequiredInstanceExtensionsPointer();

	vk::ApplicationInfo				CreateApplicationInfo();
	const vk::InstanceCreateInfo	CreateInstanceInfo(const vk::ApplicationInfo* appInfo = nullptr);

	bool CheckValidationLayerSupport(const std::vector<const char*> validationLayers);
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);

};

//VkInstance�I�u�W�F�N�g�� �A���̃I�u�W�F�N�g�̂����ꂩ����쐬���ꂽ���ׂẴI�u�W�F�N�g���j�����ꂽ��j���ł��܂��B