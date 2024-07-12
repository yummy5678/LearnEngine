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
#include "Surface.h"
#include "Device.h"
#include "SwapChainUtility.h"
#include "RenderPassUtility.h"

class VulkanRenderer
{
public:
	VulkanRenderer();
	~VulkanRenderer();


	int init(GameWindow renderWindow);
	//void draw();
	void cleanup();

private:
	GLFWwindow* window;

	int currentFrame = 0;

	// Vulkan Components
	//VkInstance instance;
	vk::UniqueInstance instance;

	//コールバック
	VkDebugReportCallbackEXT callback;

	//メインデバイス
	vk::PhysicalDevice physicalDevice;	//物理デバイス(ただの構造体なので破棄の必要はない)
	vk::UniqueDevice logicalDevice;		//論理デバイス

	//キュー
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

	//スワップチェイン
	vk::UniqueSurfaceKHR			surface;
	vk::UniqueSwapchainKHR			swapchain;
	std::vector<SwapchainImage>		swapChainImages;
	std::vector<VkFramebuffer>		swapChainFramebuffers;
	std::vector<VkCommandBuffer>	commandBuffers;

	// - Pipeline
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	vk::UniqueRenderPass renderPass;

	// - Pools
	VkCommandPool graphicsCommandPool;

	// - Utility
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	// - Synchronisation
	std::vector<VkSemaphore> imageAvailable;
	std::vector<VkSemaphore> renderFinished;
	std::vector<VkFence>	 drawFences;

	// Vulkan Functions
	// - Create Functions
	//インスタンスの作成
	void createInstance();

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
	bool checkDeviceSuitable(VkPhysicalDevice device);

	// -- Getter Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

	// -- Choose Functions
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);

	// -- Create Functions
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};

