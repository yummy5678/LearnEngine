#include "Scene.h"





CScene::CScene():
	m_QueueFamily(m_PhysicalDevice)
{
}

CScene::~CScene()
{
}

void CScene::init(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SwapchainCreateInfoKHR swapchainInfo)
{
	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;

	m_Extent = swapchainInfo.imageExtent;


	uboViewProjection.projection = glm::perspective(glm::radians(45.0f), (float)m_Extent.width / (float)m_Extent.height, 0.1f, 100.0f);
	uboViewProjection.view = glm::lookAt(glm::vec3(10.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	uboViewProjection.projection[1][1] *= -1;

	createDescriptorPool(logicalDevice, swapchainInfo.minImageCount);
	createDescriptorSets(logicalDevice, swapchainInfo.minImageCount);

	createUniformBuffers(logicalDevice, physicalDevice, swapchainInfo.minImageCount);

	createInputDescriptorSets(logicalDevice, swapchainInfo.minImageCount);
}

void CScene::updateModel(int modelId, glm::mat4 newModel)
{
	if (modelId >= modelList.size()) return;

	modelList[modelId].setModel(newModel);
}

int CScene::createMeshModel(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, std::string modelFile)
{
	// Import model "scene"
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		throw std::runtime_error("Failed to load model! (" + modelFile + ")");
	}

	// Get vector of all materials with 1:1 ID placement
	std::vector<std::string> textureNames = MeshModel::LoadMaterials(scene);

	// Conversion from the materials list IDs to our Descriptor Array IDs
	std::vector<int> matToTex(textureNames.size());

	// Loop over textureNames and create textures for them
	for (size_t i = 0; i < textureNames.size(); i++)
	{
		// If material had no texture, set '0' to indicate no texture, texture 0 will be reserved for a default texture
		if (textureNames[i].empty())
		{
			matToTex[i] = 0;
		}
		else
		{
			// Otherwise, create texture and set value to index of new texture
			matToTex[i] = createTexture(logicalDevice, physicalDevice, textureNames[i]);
		}
	}

	// Load in all our meshes
	auto graphicsQueue = logicalDevice.getQueue(m_QueueFamily.GetGraphicIndex(), 0);
	std::vector<Mesh> modelMeshes = MeshModel::LoadNode(physicalDevice,logicalDevice, graphicsQueue, graphicsCommandPool,
		scene->mRootNode, scene, matToTex);

	// Create mesh model and add to list
	MeshModel meshModel = MeshModel(modelMeshes);
	modelList.push_back(meshModel);

	return modelList.size() - 1;
}

VkImage CScene::createImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, VkExtent3D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory)
{
	// CREATE IMAGE
	// Image Creation Info
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;						// Type of image (1D, 2D, or 3D)
	imageCreateInfo.extent = extent;									// Width of image extent
	imageCreateInfo.extent.depth = 1;									// Depth of image (just 1, no 3D aspect)
	imageCreateInfo.mipLevels = 1;										// Number of mipmap levels
	imageCreateInfo.arrayLayers = 1;									// Number of levels in image array
	imageCreateInfo.format = format;									// Format type of image
	imageCreateInfo.tiling = tiling;									// How image data should be "tiled" (arranged for optimal reading)
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;			// Layout of image data on creation
	imageCreateInfo.usage = useFlags;									// Bit flags defining what image will be used for
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;					// Number of samples for multi-sampling
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;			// Whether image can be shared between queues

	// Create image
	VkImage image;
	VkResult result = vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &image);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create an Image!");
	}

	// CREATE MEMORY FOR IMAGE

	// Get memory requirements for a type of image
	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(logicalDevice, image, &memoryRequirements);

	// Allocate memory using image requirements and user defined properties
	VkMemoryAllocateInfo memoryAllocInfo = {};
	memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocInfo.allocationSize = memoryRequirements.size;
	memoryAllocInfo.memoryTypeIndex = findMemoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits, propFlags);

	result = vkAllocateMemory(logicalDevice, &memoryAllocInfo, nullptr, imageMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate memory for image!");
	}

	// Connect memory to image
	vkBindImageMemory(logicalDevice, image, *imageMemory, 0);

	return image;

}

int CScene::createTextureImage(vk::Device logicalDevice,vk::PhysicalDevice physicalDevice, std::string fileName)
{
	// Load image file
	int width, height;
	VkDeviceSize imageSize;
	stbi_uc* imageData = loadTextureFile(fileName, &width, &height, &imageSize);

	// Create staging buffer to hold loaded data, ready to copy to device
	VkBuffer imageStagingBuffer;
	VkDeviceMemory imageStagingBufferMemory;
	createBuffer(physicalDevice, logicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&imageStagingBuffer, &imageStagingBufferMemory);

	// Copy image data to staging buffer
	void* data;
	vkMapMemory(logicalDevice, imageStagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, imageData, static_cast<size_t>(imageSize));
	vkUnmapMemory(logicalDevice, imageStagingBufferMemory);

	// Free original image data
	stbi_image_free(imageData);

	// Create image to hold final texture
	VkImage texImage;
	VkDeviceMemory texImageMemory;
	texImage = createImage(logicalDevice, physicalDevice, VkExtent3D{ (uint32_t)width, (uint32_t)height, 0 }, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &texImageMemory);


	// COPY DATA TO IMAGE
	// Transition image to be DST for copy operation
	auto graphicsQueue = logicalDevice.getQueue(m_QueueFamily.GetGraphicIndex(), 0);
	transitionImageLayout(logicalDevice, graphicsQueue, graphicsCommandPool,
		texImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// Copy image data
	copyImageBuffer(logicalDevice, graphicsQueue, graphicsCommandPool, imageStagingBuffer, texImage, width, height);

	// Transition image to be shader readable for shader usage
	transitionImageLayout(logicalDevice, graphicsQueue, graphicsCommandPool,
		texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// Add texture data to vector for reference
	textureImages.push_back(texImage);
	textureImageMemory.push_back(texImageMemory);

	// Destroy staging buffers
	vkDestroyBuffer(logicalDevice, imageStagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, imageStagingBufferMemory, nullptr);

	// Return index of new texture image
	return textureImages.size() - 1;
}

int CScene::createTexture(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, std::string fileName)
{
	// Create Texture Image and get its location in array
	int textureImageLoc = createTextureImage(logicalDevice,physicalDevice, fileName);

	// Create Image View and add to list
	VkImageView imageView = createImageView(logicalDevice, textureImages[textureImageLoc], VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	textureImageViews.push_back(imageView);

	// Create Texture Descriptor
	int descriptorLoc = createTextureDescriptor(logicalDevice, imageView);

	// Return location of set with texture
	return descriptorLoc;
}

int CScene::createTextureDescriptor(vk::Device logicalDevice, VkImageView textureImage)
{
	VkDescriptorSet descriptorSet;

	// Descriptor Set Allocation Info
	VkDescriptorSetAllocateInfo setAllocInfo = {};
	setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocInfo.descriptorPool = samplerDescriptorPool;
	setAllocInfo.descriptorSetCount = 1;
	setAllocInfo.pSetLayouts = &samplerSetLayout;

	// Allocate Descriptor Sets
	VkResult result = vkAllocateDescriptorSets(logicalDevice, &setAllocInfo, &descriptorSet);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Texture Descriptor Sets!");
	}

	// Texture Image Info
	VkDescriptorImageInfo imagedInfo = {};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;	// Image layout when in use
	imageInfo.imageView = textureImage;									// Image to bind to set
	imageInfo.sampler = textureSampler;									// Sampler to use for set

	// Descriptor Write Info
	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	// Update new descriptor set
	vkUpdateDescriptorSets(logicalDevice, 1, &descriptorWrite, 0, nullptr);

	// Add descriptor set to list
	samplerDescriptorSets.push_back(descriptorSet);

	// Return descriptor set location
	return samplerDescriptorSets.size() - 1;
}

stbi_uc* CScene::loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize)
{
	// Number of channels image uses
	int channels;

	// Load pixel data for image
	std::string fileLoc = "Textures/" + fileName;
	stbi_uc* image = stbi_load(fileLoc.c_str(), width, height, &channels, STBI_rgb_alpha);

	if (!image)
	{
		throw std::runtime_error("Failed to load a Texture file! (" + fileName + ")");
	}

	// Calculate image size using given and known data
	*imageSize = *width * *height * 4;

	return image;

}

void CScene::createColourBufferImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t ImageNum, vk::Extent2D extent)
{
	// Get supported format for colour attachment
	std::vector<vk::Format> sourceFomat = { vk::Format::eR8G8B8A8Unorm };
	vk::Format colourFormat = chooseSupportedFormat(
		physicalDevice,
		sourceFomat,
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eColorAttachment);

	vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment;

	colourBufferImage.Create(logicalDevice, physicalDevice, ImageNum, colourFormat, extent, usage, vk::MemoryPropertyFlagBits::eDeviceLocal);

}


void CScene::createDepthBufferImage(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t ImageNum, vk::Extent2D extent)
{
	// Get supported format for depth buffer
	std::vector<vk::Format> sourceFomat = { vk::Format::eD32SfloatS8Uint, vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint };
	vk::Format depthFormat = chooseSupportedFormat(
		physicalDevice, // 使用する物理デバイス
		sourceFomat, // サポートされる深度フォーマットのリスト
		vk::ImageTiling::eOptimal, // オプティマルタイリングを指定
		vk::FormatFeatureFlagBits::eDepthStencilAttachment); // 深度・ステンシルアタッチメント用のフォーマット機能を要求

	vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eInputAttachment;

	depthBufferImage.Create(logicalDevice, physicalDevice, ImageNum, depthFormat, extent, usage, vk::MemoryPropertyFlagBits::eDeviceLocal);
}


void CScene::createDescriptorSetLayout(vk::Device logicalDevice)
{
	// UNIFORM VALUES DESCRIPTOR SET LAYOUT
// UboViewProjection Binding Info
	VkDescriptorSetLayoutBinding vpLayoutBinding = {};
	vpLayoutBinding.binding = 0;											// Binding point in shader (designated by binding number in shader)
	vpLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;	// Type of descriptor (uniform, dynamic uniform, image sampler, etc)
	vpLayoutBinding.descriptorCount = 1;									// Number of descriptors for binding
	vpLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;				// Shader stage to bind to
	vpLayoutBinding.pImmutableSamplers = nullptr;							// For Texture: Can make sampler data unchangeable (immutable) by specifying in layout

	// Model Binding Info
	/*VkDescriptorSetLayoutBinding modelLayoutBinding = {};
	modelLayoutBinding.binding = 1;
	modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	modelLayoutBinding.descriptorCount = 1;
	modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	modelLayoutBinding.pImmutableSamplers = nullptr;*/

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings = { vpLayoutBinding };

	// Create Descriptor Set Layout with given bindings
	VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());	// Number of binding infos
	layoutCreateInfo.pBindings = layoutBindings.data();								// Array of binding infos

	// Create Descriptor Set Layout
	VkResult result = vkCreateDescriptorSetLayout(logicalDevice, &layoutCreateInfo, nullptr, &descriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Descriptor Set Layout!");
	}

	// CREATE TEXTURE SAMPLER DESCRIPTOR SET LAYOUT
	// Texture binding info
	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	samplerLayoutBinding.pImmutableSamplers = nullptr;

	// Create a Descriptor Set Layout with given bindings for texture
	VkDescriptorSetLayoutCreateInfo textureLayoutCreateInfo = {};
	textureLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	textureLayoutCreateInfo.bindingCount = 1;
	textureLayoutCreateInfo.pBindings = &samplerLayoutBinding;

	// Create Descriptor Set Layout
	result = vkCreateDescriptorSetLayout(logicalDevice, &textureLayoutCreateInfo, nullptr, &samplerSetLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Descriptor Set Layout!");
	}

	// CREATE INPUT ATTACHMENT IMAGE DESCRIPTOR SET LAYOUT
	// Colour Input Binding
	VkDescriptorSetLayoutBinding colourInputLayoutBinding = {};
	colourInputLayoutBinding.binding = 0;
	colourInputLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	colourInputLayoutBinding.descriptorCount = 1;
	colourInputLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	// Depth Input Binding
	VkDescriptorSetLayoutBinding depthInputLayoutBinding = {};
	depthInputLayoutBinding.binding = 1;
	depthInputLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	depthInputLayoutBinding.descriptorCount = 1;
	depthInputLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	// Array of input attachment bindings
	std::vector<VkDescriptorSetLayoutBinding> inputBindings = { colourInputLayoutBinding, depthInputLayoutBinding };

	// Create a descriptor set layout for input attachments
	VkDescriptorSetLayoutCreateInfo inputLayoutCreateInfo = {};
	inputLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	inputLayoutCreateInfo.bindingCount = static_cast<uint32_t>(inputBindings.size());
	inputLayoutCreateInfo.pBindings = inputBindings.data();

	// Create Descriptor Set Layout
	result = vkCreateDescriptorSetLayout(logicalDevice, &inputLayoutCreateInfo, nullptr, &inputSetLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Descriptor Set Layout!");
	}
}

void CScene::createDescriptorPool(vk::Device logicalDevice, uint32_t imageCount)
{
	// UNIFORM デスクリプタプールの作成
	// デスクリプタのタイプと、そのデスクリプタの数を指定します
	// ViewProjectionプール
	vk::DescriptorPoolSize vpPoolSize = {};
	vpPoolSize.setType(vk::DescriptorType::eUniformBuffer); // デスクリプタタイプ
	vpPoolSize.setDescriptorCount(static_cast<uint32_t>(vpUniformBuffer.size())); // デスクリプタの数を設定

	// モデルプール（ダイナミック用）
	/*
	vk::DescriptorPoolSize modelPoolSize = {};
	modelPoolSize.setType(vk::DescriptorType::eUniformBufferDynamic);
	modelPoolSize.setDescriptorCount(static_cast<uint32_t>(modelDUniformBuffer.size()));
	*/

	// プールサイズのリストを作成
	std::vector<vk::DescriptorPoolSize> descriptorPoolSizes = { vpPoolSize };

	// デスクリプタプールを作成するためのデータを設定
	vk::DescriptorPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.setMaxSets(static_cast<uint32_t>(imageCount)); // プールから作成可能なデスクリプタセットの最大数
	poolCreateInfo.setPoolSizeCount(static_cast<uint32_t>(descriptorPoolSizes.size())); // プールサイズの数
	poolCreateInfo.setPPoolSizes(descriptorPoolSizes.data()); // プールサイズを設定

	// デスクリプタプールを作成
	auto result = logicalDevice.createDescriptorPool(poolCreateInfo);
	//if (result != vk::Result::eSuccess) // 成功かどうかを確認
	//{
	//	throw std::runtime_error("Failed to create a Descriptor Pool!");
	//}

	// SAMPLER デスクリプタプールの作成
	// テクスチャサンプラープール
	vk::DescriptorPoolSize samplerPoolSize = {};
	samplerPoolSize.setType(vk::DescriptorType::eCombinedImageSampler); // デスクリプタタイプ
	samplerPoolSize.setDescriptorCount(MAX_OBJECTS); // デスクリプタの数を設定

	vk::DescriptorPoolCreateInfo samplerPoolCreateInfo = {};
	samplerPoolCreateInfo.setMaxSets(MAX_OBJECTS); // 最大セット数を設定
	samplerPoolCreateInfo.setPoolSizeCount(1); // プールサイズの数を設定
	samplerPoolCreateInfo.setPPoolSizes(&samplerPoolSize); // プールサイズを設定

	result = logicalDevice.createDescriptorPool(samplerPoolCreateInfo);
	//if (result != vk::Result::eSuccess) // 成功かどうかを確認
	//{
	//	throw std::runtime_error("Failed to create a Descriptor Pool!");
	//}

	// INPUT ATTACHMENT デスクリプタプールの作成
	// カラーアタッチメントプールサイズ
	vk::DescriptorPoolSize colourInputPoolSize = {};
	colourInputPoolSize.setType(vk::DescriptorType::eInputAttachment); // デスクリプタタイプ
	colourInputPoolSize.setDescriptorCount(static_cast<uint32_t>(colourBufferImage.GetSize())); // デスクリプタの数を設定

	// 深度アタッチメントプールサイズ
	vk::DescriptorPoolSize depthInputPoolSize = {};
	depthInputPoolSize.setType(vk::DescriptorType::eInputAttachment); // デスクリプタタイプ
	depthInputPoolSize.setDescriptorCount(static_cast<uint32_t>(depthBufferImage.GetSize())); // デスクリプタの数を設定

	std::vector<vk::DescriptorPoolSize> inputPoolSizes = { colourInputPoolSize, depthInputPoolSize };

	// 入力アタッチメントプールを作成するための情報を設定
	vk::DescriptorPoolCreateInfo inputPoolCreateInfo = {};
	inputPoolCreateInfo.setMaxSets(imageCount); // 最大セット数を設定
	inputPoolCreateInfo.setPoolSizeCount(static_cast<uint32_t>(inputPoolSizes.size())); // プールサイズの数を設定
	inputPoolCreateInfo.setPPoolSizes(inputPoolSizes.data()); // プールサイズを設定

	result = logicalDevice.createDescriptorPool(inputPoolCreateInfo);
	//if (result != vk::Result::eSuccess) // 成功かどうかを確認
	//{
	//	throw std::runtime_error("Failed to create a Descriptor Pool!");
	//}
}

void CScene::createDescriptorSets(vk::Device logicalDevice, uint32_t imageCount)
{
	// Resize Descriptor Set list so one for every buffer
	descriptorSets.resize(imageCount);

	std::vector<VkDescriptorSetLayout> setLayouts(imageCount, descriptorSetLayout);

	// Descriptor Set Allocation Info
	VkDescriptorSetAllocateInfo setAllocInfo = {};
	setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocInfo.descriptorPool = descriptorPool;									// Pool to allocate Descriptor Set from
	setAllocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);			// Number of sets to allocate
	setAllocInfo.pSetLayouts = setLayouts.data();									// Layouts to use to allocate sets (1:1 relationship)

	// Allocate descriptor sets (multiple)
	VkResult result = vkAllocateDescriptorSets(logicalDevice, &setAllocInfo, descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Descriptor Sets!");
	}

	// Update all of descriptor set buffer bindings
	for (size_t i = 0; i < imageCount; i++)
	{
		// VIEW PROJECTION DESCRIPTOR
		// Buffer info and data offset info
		VkDescriptorBufferInfo vpBufferInfo = {};
		vpBufferInfo.buffer = vpUniformBuffer[i];		// Buffer to get data from
		vpBufferInfo.offset = 0;						// Position of start of data
		vpBufferInfo.range = sizeof(UboViewProjection);				// Size of data

		// Data about connection between binding and buffer
		VkWriteDescriptorSet vpSetWrite = {};
		vpSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vpSetWrite.dstSet = descriptorSets[i];								// Descriptor Set to update
		vpSetWrite.dstBinding = 0;											// Binding to update (matches with binding on layout/shader)
		vpSetWrite.dstArrayElement = 0;									// Index in array to update
		vpSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;		// Type of descriptor
		vpSetWrite.descriptorCount = 1;									// Amount to update
		vpSetWrite.pBufferInfo = &vpBufferInfo;							// Information about buffer data to bind

		// MODEL DESCRIPTOR
		// Model Buffer Binding Info
		/*VkDescriptorBufferInfo modelBufferInfo = {};
		modelBufferInfo.buffer = modelDUniformBuffer[i];
		modelBufferInfo.offset = 0;
		modelBufferInfo.range = modelUniformAlignment;

		VkWriteDescriptorSet modelSetWrite = {};
		modelSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		modelSetWrite.dstSet = descriptorSets[i];
		modelSetWrite.dstBinding = 1;
		modelSetWrite.dstArrayElement = 0;
		modelSetWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		modelSetWrite.descriptorCount = 1;
		modelSetWrite.pBufferInfo = &modelBufferInfo;*/

		// List of Descriptor Set Writes
		std::vector<VkWriteDescriptorSet> setWrites = { vpSetWrite };

		// Update the descriptor sets with new buffer/binding info
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(setWrites.size()), setWrites.data(),
			0, nullptr);
	}

}

void CScene::createUniformBuffers(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t imageCount)
{
	// ViewProjection buffer size
	VkDeviceSize vpBufferSize = sizeof(UboViewProjection);

	// Model buffer size
	//VkDeviceSize modelBufferSize = modelUniformAlignment * MAX_OBJECTS;

	// One uniform buffer for each image (and by extension, command buffer)
	vpUniformBuffer.resize(imageCount);
	vpUniformBufferMemory.resize(imageCount);
	//modelDUniformBuffer.resize(swapChainImages.size());
	//modelDUniformBufferMemory.resize(swapChainImages.size());

	// Create Uniform buffers
	for (size_t i = 0; i < imageCount; i++)
	{
		createBuffer(physicalDevice, logicalDevice, vpBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vpUniformBuffer[i], &vpUniformBufferMemory[i]);

		/*createBuffer(mainDevice.physicalDevice, mainDevice.logicalDevice, modelBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &modelDUniformBuffer[i], &modelDUniformBufferMemory[i]);*/
	}
}

void CScene::createTextureSampler(vk::Device logicalDevice)
{
	// Sampler Creation Info
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;						// How to render when image is magnified on screen
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;						// How to render when image is minified on screen
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;	// How to handle texture wrap in U (x) direction
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;	// How to handle texture wrap in V (y) direction
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;	// How to handle texture wrap in W (z) direction
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;	// Border beyond texture (only workds for border clamp)
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;				// Whether coords should be normalized (between 0 and 1)
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;		// Mipmap interpolation mode
	samplerCreateInfo.mipLodBias = 0.0f;								// Level of Details bias for mip level
	samplerCreateInfo.minLod = 0.0f;									// Minimum Level of Detail to pick mip level
	samplerCreateInfo.maxLod = 0.0f;									// Maximum Level of Detail to pick mip level
	samplerCreateInfo.anisotropyEnable = VK_TRUE;						// Enable Anisotropy
	samplerCreateInfo.maxAnisotropy = 16;								// Anisotropy sample level

	VkResult result = vkCreateSampler(logicalDevice, &samplerCreateInfo, nullptr, &textureSampler);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Filed to create a Texture Sampler!");
	}
}

void CScene::createInputDescriptorSets(vk::Device logicalDevice, uint32_t imageCount)
{
	// スワップチェーンの各イメージに対するデスクリプタセットを保持するために配列をリサイズ
	inputDescriptorSets.resize(imageCount);

	// デスクリプタセット作成のためのレイアウトを設定
	std::vector<vk::DescriptorSetLayout> setLayouts(imageCount, inputSetLayout);

	// デスクリプタセットの割り当て情報を作成
	vk::DescriptorSetAllocateInfo setAllocInfo = {};
	setAllocInfo.setDescriptorPool(inputDescriptorPool); // デスクリプタプールを指定
	setAllocInfo.setDescriptorSetCount(imageCount); // 割り当てるデスクリプタセットの数
	setAllocInfo.setPSetLayouts(setLayouts.data()); // レイアウトの配列を指定

	// デスクリプタセットの割り当てを行う
	auto result = logicalDevice.allocateDescriptorSets(setAllocInfo);
	//if (result.result != vk::Result::eSuccess) // 成功かどうかを確認
	//{
	//	throw std::runtime_error("Failed to allocate Input Attachment Descriptor Sets!");
	//}

	// 各デスクリプタセットを入力アタッチメントで更新する
	for (size_t i = 0; i < imageCount; i++)
	{
		// カラーアタッチメントのデスクリプタ情報を作成
		vk::DescriptorImageInfo colourAttachmentDescriptor = {};
		colourAttachmentDescriptor.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal); // 画像のレイアウトを設定
		colourAttachmentDescriptor.setImageView(colourBufferImage.GetImageViews()[i]); // イメージビューを設定
		colourAttachmentDescriptor.setSampler(vk::Sampler()); // サンプラは無効に設定

		// カラーアタッチメントのデスクリプタ書き込み情報を作成
		vk::WriteDescriptorSet colourWrite = {};
		colourWrite.setDstSet(inputDescriptorSets[i]); // 更新するデスクリプタセット
		colourWrite.setDstBinding(0); // バインディングインデックス
		colourWrite.setDescriptorType(vk::DescriptorType::eInputAttachment); // デスクリプタタイプ
		colourWrite.setDescriptorCount(1); // デスクリプタの数
		colourWrite.setPImageInfo(&colourAttachmentDescriptor); // 画像情報のポインタ

		// デプスアタッチメントのデスクリプタ情報を作成
		vk::DescriptorImageInfo depthAttachmentDescriptor = {};
		depthAttachmentDescriptor.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal); // 画像のレイアウトを設定
		depthAttachmentDescriptor.setImageView(depthBufferImage.GetImageViews()[i]); // デプスのイメージビューを設定
		depthAttachmentDescriptor.setSampler(vk::Sampler()); // サンプラは無効に設定

		// デプスアタッチメントのデスクリプタ書き込み情報を作成
		vk::WriteDescriptorSet depthWrite = {};
		depthWrite.setDstSet(inputDescriptorSets[i]); // 更新するデスクリプタセット
		depthWrite.setDstBinding(1); // バインディングインデックス
		depthWrite.setDescriptorType(vk::DescriptorType::eInputAttachment); // デスクリプタタイプ
		depthWrite.setDescriptorCount(1); // デスクリプタの数
		depthWrite.setPImageInfo(&depthAttachmentDescriptor); // 画像情報のポインタ

		// デスクリプタセットの更新リストを作成
		std::vector<vk::WriteDescriptorSet> setWrites = { colourWrite, depthWrite };

		// デスクリプタセットを更新
		logicalDevice.updateDescriptorSets(setWrites, nullptr); // nullptrはデスクリプタの削除情報を無視することを意味する
	}
}

void CScene::updateUniformBuffers(vk::Device logicalDevice, uint32_t imageIndex)
{
	// Copy VP data
	void* data;
	vkMapMemory(logicalDevice, vpUniformBufferMemory[imageIndex], 0, sizeof(UboViewProjection), 0, &data);
	memcpy(data, &uboViewProjection, sizeof(UboViewProjection));
	vkUnmapMemory(logicalDevice, vpUniformBufferMemory[imageIndex]);

	// Copy Model data
	/*for (size_t i = 0; i < meshList.size(); i++)
	{
		UboModel * thisModel = (UboModel *)((uint64_t)modelTransferSpace + (i * modelUniformAlignment));
		*thisModel = meshList[i].getModel();
	}

	// Map the list of model data
	vkMapMemory(mainDevice.logicalDevice, modelDUniformBufferMemory[imageIndex], 0, modelUniformAlignment * meshList.size(), 0, &data);
	memcpy(data, modelTransferSpace, modelUniformAlignment * meshList.size());
	vkUnmapMemory(mainDevice.logicalDevice, modelDUniformBufferMemory[imageIndex]);*/
}


VkImageView CScene::createImageView(vk::Device logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.image = image;											// Image to create view for
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;						// Type of image (1D, 2D, 3D, Cube, etc)
	viewCreateInfo.format = format;											// Format of image data
	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;			// Allows remapping of rgba components to other rgba values
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	// Subresources allow the view to view only a part of an image
	viewCreateInfo.subresourceRange.aspectMask = aspectFlags;				// Which aspect of image to view (e.g. COLOR_BIT for viewing colour)
	viewCreateInfo.subresourceRange.baseMipLevel = 0;						// Start mipmap level to view from
	viewCreateInfo.subresourceRange.levelCount = 1;							// Number of mipmap levels to view
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;						// Start array level to view from
	viewCreateInfo.subresourceRange.layerCount = 1;							// Number of array levels to view

	// Create image view and return it
	VkImageView imageView;
	VkResult result = vkCreateImageView(logicalDevice, &viewCreateInfo, nullptr, &imageView);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create an Image View!");
	}

	return imageView;
}

vk::Format CScene::chooseSupportedFormat(vk::PhysicalDevice physicalDevice, std::vector<vk::Format>& formats, vk::ImageTiling tiling, vk::FormatFeatureFlags featureFlags)
{
	// 利用可能なフォーマットの中から互換性のあるものを探す
	for (vk::Format format : formats)
	{
		// 指定したフォーマットのプロパティを取得
		vk::FormatProperties properties;
		physicalDevice.getFormatProperties(format, &properties);

		// タイリングの選択に応じて、異なるビットフラグをチェック
		if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & featureFlags) == featureFlags)
		{
			// リニアタイリングがサポートされている場合
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & featureFlags) == featureFlags)
		{
			// オプティマルタイリングがサポートされている場合
			return format;
		}
	}

	// 互換性のあるフォーマットが見つからない場合はエラーをスロー
	throw std::runtime_error("Failed to find a matching format!");
}














