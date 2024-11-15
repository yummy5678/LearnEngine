#pragma once
#include <vulkan/vulkan.hpp> //glfw���vulkan.h����ɗ���悤�ɂ���
//#include <GLFW/glfw3.h>
#include "GeneratorBase.h"
#include "GraphicsDefine.h"
#include "VulkanLayerManager.h"
#include "InstanceExtensionManager.h"
#include "DeviceExtensionManager.h"
#include "VulkanValidation.h"


// �C���X�^���X�g���@�\�̃��X�g���쐬����
class InstanceGenerator : public GeneratorBase
{
public:
	InstanceGenerator();
	~InstanceGenerator();

	void Create(InstanceExtension extensionManager);
	void Destroy();

	vk::Instance GetInstanse();
	

private:
	vk::ApplicationInfo		m_ApplicationInfo;	//���̃A�v���̖��O�Ȃǂ�����\����
	vk::Instance			m_Instance;

	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	std::vector<const char*> m_InstanceExtensions;

	//���C���[�̎擾
	InstanceLayerManager			GetLayers();
		;
	//�A�v���P�[�V�������̎擾
	vk::ApplicationInfo				GetApplicationInfo();

	//�C���X�^���X�̍쐬���̍쐬
	const vk::InstanceCreateInfo	GetInstanceInfo(
		const vk::ApplicationInfo* appInfo,
		InstanceLayerManager& layerManager,
		InstanceExtension& extensionManager);

};

// �C���X�^���X�ɂ���
// Vulkan�̋@�\�𗘗p����̂ɕK�v�ɂȂ����
// �܂��ŏ��ɃC���X�^���X���쐬����K�v������
// �C���X�^���X�͈�̃A�v���P�[�V�����ɂP�Ƃ͌��炸�A�����쐬�ł���B
// ���̌�f�o�C�X�AGPU����Ӌ@��̏������āA�K�v�ȏ����쐬����B
//

// �A�v���P�[�V�����C���t�H(ApplicationInfo)�ɂ���
// ���b�Z�[�W�̕\���p�ɃA�v���P�[�V�����̖��O�Ȃǂ�ݒ�ł���B
// �A�v���P�[�V�����C���t�H�͍쐬���Ȃ��Ă��C���X�^���X���쐬���邱�Ƃ͂ł���
// 

// ���C���[�ɂ���
// ���C���[���I���ɂ��ăG���[���o���Ƃ��ɏ��
// �R�}���h���C���ɕ\���o����悤�ɂȂ�B
// �ڂ�����InstanceLayerManager�̃y�[�W�ɋL�ځB
// 

// �g���@�\�ɂ���
// Vulkan�𓮂����ɂ������ĕK�������K�v�ł͂Ȃ��@�\��
// �C���X�^���X�쐬���ɃI���ɂ���B
// �T�[�t�F�X�A�܂�摜�̉�ʕ\���͊g���@�\�B
// 
