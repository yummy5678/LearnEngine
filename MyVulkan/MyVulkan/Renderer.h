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

	//コールバック
	VkDebugReportCallbackEXT callback;

	//メインデバイス
	//vk::PhysicalDevice physicalDevice;	//物理デバイス(ただの構造体なので破棄の必要はない)
	//vk::UniqueDevice logicalDevice;		//論理デバイス

	//キュー
	//VkQueue graphicsQueue;
	//VkQueue presentationQueue;

	//スワップチェイン
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
	//インスタンスの作成
	//void createInstance();

	//デバイスの作成
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

//VulkanRendererクラスについて
//Vulkanのインスタンス作成から描画、破棄までの処理を記述したクラス
// 
// ・画面に図形を表示するまでに必要なこと
// 1. インスタンスの作成
// Vulkanを使用する際は何よりもまずインスタンスを作成する
// インスタンスはVulkanの機能を使用するのに必ず使用する
// 
// 2. 物理デバイスの選択
// Vulkanが利用できる物理デバイス（GPU）を選択する。
// (GPUについての詳しい説明はDeviceGeneratorに記載)
// インスタンスを介して接続ざれているGPUを全て取得して
// 使用に適したものをここで選んでおく。
// 
// 3. 論理デバイスとキューの作成
// 
// 
// 
//

//ここでやっていること
//画面に三角形を表示するところまでやる
//インスタンスやサーフェス、デバイスの作成についての詳細は各Generatorクラスに記載。
// 
// ①インスタンスの作成
// ②
// 
// 
// 
//