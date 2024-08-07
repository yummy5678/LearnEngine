#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "GeneratorBase.h"
#include "GraphicsDefine.h"
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
	std::vector<std::string>* GetExtensionsPointer();
private:
	vk::ApplicationInfo		m_ApplicationInfo;	//���̃A�v���̖��O�Ȃǂ�����\����
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

// �C���X�^���X�ɂ���
// Vulkan�̋@�\�𗘗p����̂ɕK�v�ɂȂ����
// �܂��ŏ��ɃC���X�^���X���쐬����K�v������
// �C���X�^���X�͈�̃A�v���P�[�V�����ɂP�Ƃ͌��炸�A�����쐬�ł���B
// ���̌�f�o�C�X�AGPU����Ӌ@��̏������āA�K�v�ȏ����쐬����B
//

// �A�v���P�[�V�����C���t�H(ApplicationInfo)�ɂ���
// ���b�Z�[�W�̕\���p�ɃA�v���P�[�V�����̖��O�Ȃǂ�ݒ�ł���B
// ������쐬���Ȃ��Ă��C���X�^���X���쐬���邱�Ƃ͂ł��邪�A
// ���O��t�����ق������C�ɂȂ��̂ō�邱�Ƃ𐄏�����B
// 



// VkInstance�I�u�W�F�N�g�� �A
// ���̃I�u�W�F�N�g�̂����ꂩ����쐬���ꂽ
// ���ׂẴI�u�W�F�N�g���j�����ꂽ��j���ł��܂��B