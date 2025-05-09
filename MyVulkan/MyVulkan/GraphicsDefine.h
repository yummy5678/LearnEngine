#pragma once
#include <vulkan/vulkan.h>

// �ŏ��ɍ쐬����E�B���h�E�̃T�C�Y���
constexpr uint32_t windowWidth  = 800;	//�E�B���h�E�̉���
constexpr uint32_t windowHeight = 600;	//�E�B���h�E�̏c��

// �X���b�v�`�F�[���Ŏg�p����C���[�W�̐�
constexpr int MAX_FRAME_DRAWS = 2;

constexpr uint64_t MAX_WAIT_TIME = 5'000'000'000;



// Vulkan�̐ݒ���
constexpr struct _VulkanDefine
{
	// �f�o�b�O�p�t���O
	bool LogMessageEnabled = true;	//Vulkan�̃I�u�W�F�N�g�쐬���Ƀ��O���o����
	bool ValidationEnabled = true;	//���؃��C���[���g�p���邩

	// �C���X�^���X�쐬�p�t���O
	const char* ApplicationName = "Graphics App";				// �A�v���P�[�V�����̖��O
	uint32_t	ApplicationVersion = VK_MAKE_VERSION(1, 0, 0);  // �A�v���P�[�V�����̃o�[�W������ 
	const char* EngineName = "Vulkan Engine";					// �G���W���̖��O
	uint32_t	EngineVersion = VK_MAKE_VERSION(1, 0, 0);		// �G���W���̃o�[�W������
	uint32_t	ApiVersion = VK_API_VERSION_1_3;                // Vulkan API�̃o�[�W����

	VkFormat UseColorFormat = VK_FORMAT_R8G8B8A8_UNORM;
	VkFormat UseDepthFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;

} VulkanDefine;

constexpr struct _ShaderDefine
{
	const char* VertexShaderPath	= "Shaders/vert.spv";
	const char* FragmentShaderPath	= "Shaders/frag.spv";

	const char* EntryName			= "main";

} DefaultShaderDefine;



