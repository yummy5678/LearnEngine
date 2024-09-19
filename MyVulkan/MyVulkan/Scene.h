#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "MeshModel.h"
#include "ImagesGenerator.h"
#include "QueueUtility.h"

// STB
#include "stb/stb_image.h"


class CScene
{
public:
	CScene();
	~CScene();

	void init(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR swapchainInfo);

	void updateModel(int modelId, glm::mat4 newModel);

	int createMeshModel(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, std::string modelFile);


	VkImage createImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkExtent3D extent , VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags,
		VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);
	int createTextureImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, std::string fileName);
	int createTexture(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, std::string fileName);
	int createTextureDescriptor(vk::Device logicalDevice, VkImageView textureImage);

	stbi_uc* loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize);
private:
	vk::Device				m_LogicalDevice;
	vk::PhysicalDevice		m_PhysicalDevice;

	// - Pools
	VkCommandPool graphicsCommandPool;

	ImagesGenerator  colourBufferImage;
	ImagesGenerator  depthBufferImage;

	QueueFamilySelector m_QueueFamily;

	VkSampler textureSampler;

	// Scene Objects
	std::vector<MeshModel> modelList;

	// Scene Settings
	struct UboViewProjection {
		glm::mat4 projection;
		glm::mat4 view;
	} uboViewProjection;

	vk::Extent2D m_Extent;

	// - Descriptors
	VkDescriptorSetLayout	descriptorSetLayout;
	VkDescriptorSetLayout	samplerSetLayout;
	VkDescriptorSetLayout	inputSetLayout;
	VkPushConstantRange		pushConstantRange;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool inputDescriptorPool;

	std::vector<VkDescriptorSet> inputDescriptorSets;

	std::vector<VkDescriptorSet> samplerDescriptorSets;
	VkDescriptorPool samplerDescriptorPool;

	std::vector<VkBuffer> vpUniformBuffer;
	std::vector<VkDeviceMemory> vpUniformBufferMemory;

	void createColourBufferImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t ImageNum, vk::Extent2D extent);
	void createDepthBufferImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t ImageNum, vk::Extent2D extent);

	// - Assets

	std::vector<VkImage> textureImages;
	std::vector<VkDeviceMemory> textureImageMemory;
	std::vector<VkImageView> textureImageViews;

	void createDescriptorSetLayout(vk::Device logicalDevice);
	void createDescriptorPool(vk::Device logicalDevice, uint32_t imageCount);
	void createDescriptorSets(vk::Device logicalDevice, uint32_t imageCount);
	void createUniformBuffers(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t imageCount);
	void createTextureSampler(vk::Device logicalDevice);
	void createInputDescriptorSets(vk::Device logicalDevice, uint32_t imageCount);
	void updateUniformBuffers(vk::Device logicalDevice, uint32_t imageIndex);
	vk::Format chooseSupportedFormat(vk::PhysicalDevice physicalDevice, std::vector<vk::Format>& formats, vk::ImageTiling tiling, vk::FormatFeatureFlags featureFlags);
	VkImageView createImageView(vk::Device logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

};

