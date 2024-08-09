#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <set>
#include <algorithm>
#include <array>

#include "GameWindow.h"
#include "Utilities.h"
#include "VulkanValidation.h"
#include "VulkanInstance.h"
#include "Surface.h"
#include "Device.h"
#include "SwapChainUtility.h"
#include "RenderPassUtility.h"
#include "GraphicsPipelineUtility.h"
#include "FramebufferGenerator.h"
#include "CommandUtility.h"
#include "SynchronizationGenerator.h"


class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	int init(GameWindow renderWindow);
	void draw();
	void cleanup();

private:
	GLFWwindow* window;

	int currentFrame = 0;
	InstanceGenerator		m_InstanceGenerator;
	SurfaceGenerator		m_SurfaceGenerator;
	DeviceGenerator			m_DeviceGenerator;
	SwapchainGenerator		m_SwapchainGenerator;
	RenderpassGenerator		m_RenderpassGenerator;
	PipelineGenerator		m_PipelineGenerator;
	FramebufferGenerator	m_FramebufferGenerator;
	CommandGenerator		m_CommandGenerator;


	
	// Vulkan Components
	//VkInstance instance;
	//vk::UniqueInstance instance;

	//�R�[���o�b�N
	VkDebugReportCallbackEXT callback;

	//���C���f�o�C�X
	//vk::PhysicalDevice physicalDevice;	//�����f�o�C�X(�����̍\���̂Ȃ̂Ŕj���̕K�v�͂Ȃ�)
	//vk::UniqueDevice logicalDevice;		//�_���f�o�C�X

	//�L���[
	//VkQueue graphicsQueue;
	//VkQueue presentationQueue;

	//�X���b�v�`�F�C��
	//vk::UniqueSurfaceKHR				surface;
	//vk::UniqueSwapchainKHR			swapchain;
	//std::vector<SwapchainImage>			swapChainImages;
	//std::vector<vk::Framebuffer>		swapChainFramebuffers;
	//std::vector<vk::CommandBuffer>		commandBuffers;

	// - Pipeline
	//vk::UniquePipeline			graphicsPipeline;
	//vk::UniquePipelineLayout	pipelineLayout;
	//vk::UniqueRenderPass		renderPass;

	// - Pools
	//vk::UniqueCommandPool graphicsCommandPool;

	// - Utility
	//VkFormat swapChainImageFormat;
	//vk::Extent2D swapChainExtent = {
	//	windowWidth,
	//	windowHeight
	//};

	// - Synchronisation
	SynchronizationGenerator synchronizationGenerator;
	//std::vector<VkSemaphore> imageAvailable;
	//std::vector<VkSemaphore> renderFinished;
	//std::vector<VkFence>	 drawFences;

	// Vulkan Functions
	// - Create Functions
	//�C���X�^���X�̍쐬
	//void createInstance();
	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	std::vector<const char*> m_InstanceExtensions;

	//�f�o�C�X�̍쐬
	void createDebugCallback();
	void createLogicalDevice();


	//void createSurface();
	void createSwapChain();
	void createRenderPass();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSynchronisation();

	// - Record Functions
	void recordCommands();

	// - Get Functions
	void getPhysicalDevice();

	// - Support Functions
	// -- Checker Functions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkDeviceSuitable(VkPhysicalDevice device) { return false; };

	// -- Getter Functions
	//QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device) { return SwapChainDetails{}; };

	// -- Choose Functions
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

	// -- Create Functions
	//VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	//VkShaderModule createShaderModule(const std::vector<char>& code);
};

// VulkanRenderer�N���X�ɂ���
// Vulkan�̃C���X�^���X�쐬����`��A�j���܂ł̏������܂Ƃ߂��N���X�B
// �ڍׂ͈ȉ��ɋL�ځB
// 
// �E��ʂɐ}�`��\������܂łɕK�v�Ȃ���
// 1. �C���X�^���X�̍쐬
// Vulkan���g�p����ۂ͉������܂��C���X�^���X���쐬����B
// �C���X�^���X��Vulkan�̋@�\���g�p����̂ɕK�v�Ȃ��̂ł���B
// 
// 2. �T�[�t�F�X�̍쐬
// �T�[�t�F�X(Vulkan�̃C���[�W�̕\����)�̏��𓾂�B
// ����̏ꍇ��GLFW��ʂ��č쐬�����E�B���h�E������𓾂�B
// 
// 3. �����f�o�C�X�̑I��
// Vulkan�����p�ł��镨���f�o�C�X(GPU)��I������B
// (GPU�ɂ��Ă̏ڂ���������DeviceGenerator.h�ɋL��)
// �C���X�^���X����Đڑ�����Ă���GPU��S�Ď擾����
// �g�p�ɓK�������̂������őI��ł����B
// 
// 4. �_���f�o�C�X�Ə�������(�L���[)�̍쐬
// GPU�ƒʐM���邽�߂ɁA�_���f�o�C�X���쐬����B
// �_���f�o�C�X�̓\�t�g�E�F�A����GPU�̏������������߂̂��́B
// ����͕`��ƕ\���������s�����߂̖���(�L���[)���擾����
// �_���f�o�C�X�ɓn���Ă����B
// 
// 5. �X���b�v�`�F�[���̐ݒ�
// �ȒP�ɐ�������ƁA�`�悵���摜���E�B���h�E�ɕ\������܂ł̓����ݒ肷����́B
// ���ꂪ�Ȃ��ƁA�`�挋�ʂ�\���ł��Ȃ��B
// ���e�������̂ŏڍׂ�SwapChainGenerator.h���Q�ƁB
// 
// 6. �����_�[�p�X�̐ݒ�
// �摜���쐬����ۂ̏����菇��ݒ肷�邽�߂̂��́B
// �X���b�v�`�F�C���̍쐬����n���Ă���̂́A
// �摜�f�[�^�̐F�̌`�����X���b�v�`�F�C���ƈ�v�����邽�߁B
// 
// 7. �O���t�B�b�N�X�p�C�v���C���̐ݒ�
// 3D�I�u�W�F�N�g�̃f�[�^��`�悷��ۂɎ{�������̐ݒ�B
// �����ŕ����̃V�F�[�_�[��ʂ��Ē��_���̏���
// �ŏI�I�ɉ摜�f�[�^�Ƃ��ĕϊ��ł���悤�ɂ���B
// �ڍׂ�PipelineGenerator.h���Q�ƁB
// 
// 8. �t���[���o�b�t�@�̍쐬
// �O���t�B�b�N�X�p�C�v���C�����`�悵���摜���Ǘ�������́B
// �ڍׂ�FramebufferGenerator.h���Q�ƁB
// 
// 9. �R�}���h�o�b�t�@�̍쐬�ƋL�^
// GPU�ɑ���p�̕`�施��(�R�}���h�o�b�t�@)��
// �X���b�v�`�F�C���ɕK�v�Ȗ������쐬����B
// ��CPU����GPU�ɏ������߂𓊂��鎞�̖��O���R�}���h�o�b�t�@�AGPU�����߂�ǂݎ��Ƃ��̌Ăѕ����L���[
// 
// 10. �����I�u�W�F�N�g�̍쐬
// �����I�u�W�F�N�g(����̓Z�}�t�H�ƃt�F���X)���쐬���āA�`��̐i�s�󋵂��Ǘ�����B
// �����I�u�W�F�N�g��GPU�̕`�揈���ƃf�B�X�v���C�̕\�����x�����킹�邽�߂̂��́B
// GPU�̌v�Z�͐��������̂ŁA���x�����킹�Ȃ��Ɖ�ʂւ̕\�����ǂ����Ȃ��Ȃ�B
// 
// 11. �`��̎��s
// 
// 
// 
// 
//  

//�����ł���Ă��邱��
//��ʂɎO�p�`��\������Ƃ���܂ł��
//�C���X�^���X��T�[�t�F�X�A�f�o�C�X�̍쐬�ɂ��Ă̏ڍׂ͊eGenerator�N���X�ɋL�ځB
// 
// 
// 
// 
// 
//