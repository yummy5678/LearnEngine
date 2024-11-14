#pragma once
#include <vulkan/vulkan.h>

// �ŏ��ɍ쐬����E�B���h�E�̃T�C�Y���
constexpr uint32_t windowWidth  = 800;	//�E�B���h�E�̉���
constexpr uint32_t windowHeight = 600;	//�E�B���h�E�̏c��

// �X���b�v�`�F�[���Ŏg�p����C���[�W�̐�
constexpr int MAX_FRAME_DRAWS = 2;

// Vulkan�̐ݒ���
constexpr struct _VulkanDefine
{
	// �f�o�b�O�p�t���O
	bool LogMessageEnabled = true;	//Vulkan�̃I�u�W�F�N�g�쐬���Ƀ��O���o����
	bool ValidationEnabled = true;	//���؃��C���[���g�p���邩

	// �C���X�^���X�쐬�p�t���O
	const char* ApplicationName = "Vulkan App";					// �A�v���P�[�V�����̖��O
	uint32_t	ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);  // �A�v���P�[�V�����̃o�[�W������ 
	const char* EngineName = "No Engine";						// �G���W���̖��O
	uint32_t	EngineVersion = VK_MAKE_VERSION(1, 0, 0);		// �G���W���̃o�[�W������
	uint32_t	ApiVersion = VK_API_VERSION_1_0;                // Vulkan API�̃o�[�W����
} VulkanDefine;





