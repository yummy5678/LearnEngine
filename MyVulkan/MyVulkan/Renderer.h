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
	InstanceGenerator		instanceGenerator;
	SurfaceGenerator		surfaceGenerator;
	DeviceGenerator			deviceGenerator;
	SwapchainGenerator		swapchainGenerator;
	RenderpassGenerator		renderpassGenerator;
	PipelineGenerator		pipelineGenerator;
	FramebufferGenerator	framebufferGenerator;
	CommandGenerator		commandGenerator;
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
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device) { return QueueFamilyIndices{}; };
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device) { return SwapChainDetails{}; };

	// -- Choose Functions
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

	// -- Create Functions
	//VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	//VkShaderModule createShaderModule(const std::vector<char>& code);
};

//VulkanRenderer�N���X�ɂ���
//Vulkan�̃C���X�^���X�쐬����`��A�j���܂ł̏������L�q�����N���X
// 
// �E��ʂɐ}�`��\������܂łɕK�v�Ȃ���
// 1. �C���X�^���X�̍쐬
// Vulkan���g�p����ۂ͉������܂��C���X�^���X���쐬����
// �C���X�^���X��Vulkan�̋@�\���g�p����̂ɕK���g�p����
// 
// 2. �����f�o�C�X�̑I��
// Vulkan�����p�ł��镨���f�o�C�X�iGPU�j��I������B
// (GPU�ɂ��Ă̏ڂ���������DeviceGenerator�ɋL��)
// �C���X�^���X����Đڑ�����Ă���GPU��S�Ď擾����
// �g�p�ɓK�������̂������őI��ł����B
// 
// 3. �_���f�o�C�X�ƃL���[�̍쐬
// 
// 
// 
//

//�����ł���Ă��邱��
//��ʂɎO�p�`��\������Ƃ���܂ł��
//�C���X�^���X��T�[�t�F�X�A�f�o�C�X�̍쐬�ɂ��Ă̏ڍׂ͊eGenerator�N���X�ɋL�ځB
// 
// �@�C���X�^���X�̍쐬
// �A
// 
// 
// 
//