#include "Renderer.h"


VulkanRenderer::VulkanRenderer() :
	m_pWindow(nullptr),
	m_InstanceExtension(),
	m_DeviceExtension(),
	m_InstanceGenerator(),
	m_SurfaceGenerator(m_InstanceExtension),
	m_DeviceGenerator(),
	//m_ImageGenerator(),
	m_SwapchainGenerator(m_DeviceExtension),
	m_RenderpassGenerator(),
	m_PipelineGenerator(),
	m_FramebufferGenerator(),
	m_CommandGenerator(),
	m_SynchroGenerator(),
	m_GraphicController(m_DeviceExtension)
{
}

VulkanRenderer::~VulkanRenderer()
{
}


int VulkanRenderer::init(GameWindow renderWindow)
{
	m_pWindow = renderWindow.GetPointer(); //ウィンドウのポインタのセット
	
	try {
		//インスタンスの作成
		m_InstanceGenerator.Create(m_InstanceExtension);
		auto instance = m_InstanceGenerator.GetInstanse();

		createDebugCallback();

		//サーフェスの作成
		m_SurfaceGenerator.CreateWindowSurface(instance,m_pWindow);
		auto surface = m_SurfaceGenerator.GetSurface();


		//物理・論理デバイスの作成
		m_DeviceGenerator.Create(m_DeviceExtension, instance, surface);
		//物理デバイスを取得
		auto physicalDevice			= m_DeviceGenerator.GetPhysicalDevice();
		auto surfaceCapabilities	= m_SurfaceGenerator.GetCapabilities(physicalDevice);
		auto windowExtent			= surfaceCapabilities.currentExtent;
		auto surfaceFomat			= m_SurfaceGenerator.GetFomats(physicalDevice)[0];

		//論理デバイスを取得
		auto logicalDevice	= m_DeviceGenerator.GetLogicalDevice();

		//m_ImageGenerator.CreateForSurface(logicalDevice, physicalDevice, surface);
		//auto imageViews = m_ImageGenerator.GetImageViews();

		////スワップチェーンの作成
		//m_SwapchainGenerator.Create(logicalDevice, physicalDevice, surface);
		//auto swapchainImage = m_SwapchainGenerator.GetImages();

		////コマンドバッファの作成
		//m_CommandGenerator.Create(logicalDevice, physicalDevice, swapchainImage.GetSize());

		////レンダーパスの作成
		//m_RenderpassGenerator.Create(logicalDevice, swapchainImage.GetFomat());
		//auto renderPass = m_RenderpassGenerator.GetRenderpass();

		////パイプラインの作成
		//m_PipelineGenerator.Create(logicalDevice, windowExtent, renderPass);
		//auto graphicsPipeline = m_PipelineGenerator.GetPipeline();
		//
		////フレームバッファの作成
		//m_FramebufferGenerator.Create(logicalDevice, swapchainImage.GetImageViews(), renderPass, windowExtent);
		//auto framebuffers = m_FramebufferGenerator.GetFramebuffers();

		////コマンドの記録
		//m_CommandGenerator.Create(logicalDevice, physicalDevice, 3);
		//auto commandBuffers = m_CommandGenerator.GetCommandBuffers();
		////m_CommandGenerator.RecordGraphicCommands(framebuffers, renderPass, windowExtent, graphicsPipeline);

		//m_CommandGenerator.DrawFrame(commandBuffers[0], renderPass, framebuffers[0], { {0,0},windowExtent }, graphicsPipeline);
		////WriteVulkanImage("../frame0.bmp", swapchainImage.GetImageData()[0], windowExtent);


		//m_SynchroGenerator.Create(logicalDevice);
		////createSynchronisation();

		m_GraphicController.Initialize(logicalDevice, physicalDevice, surface);


		//allocateDynamicBufferTransferSpace();
		//createUniformBuffers();

		createInputDescriptorSets();




		// Create our default "no texture" texture
		createTexture("plain.png");
	}
	catch (const std::runtime_error& e) {
		//エラーメッセージ受け取り
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


//void draw{
//		recordCommands(imageIndex);
//	updateUniformBuffers(imageIndex);
//}


//物理デバイス作成処理に追加
//deviceFeatures.samplerAnisotropy = VK_TRUE;		// Enable Anisotropy



void VulkanRenderer::createPushConstantRange()
{
	// Define push constant values (no 'create' needed!)
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;	// Shader stage push constant will go to
	pushConstantRange.offset = 0;								// Offset into given data to pass to push constant
	pushConstantRange.size = sizeof(Model);						// Size of data being passed
}


void VulkanRenderer::createColourBufferImage()
{
	// Resize supported format for colour attachment
	colourBufferImage.resize(swapChainImages.size());
	colourBufferImageMemory.resize(swapChainImages.size());
	colourBufferImageView.resize(swapChainImages.size());

	// Get supported format for colour attachment
	VkFormat colourFormat = chooseSupportedFormat(
		{ VK_FORMAT_R8G8B8A8_UNORM },
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	);

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		// Create Colour Buffer Image
		colourBufferImage[i] = createImage(swapChainExtent.width, swapChainExtent.height, colourFormat, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &colourBufferImageMemory[i]);

		// Create Colour Buffer Image View
		colourBufferImageView[i] = createImageView(colourBufferImage[i], colourFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}


void VulkanRenderer::createDepthBufferImage()
{
	depthBufferImage.resize(swapChainImages.size());
	depthBufferImageMemory.resize(swapChainImages.size());
	depthBufferImageView.resize(swapChainImages.size());

	// Get supported format for depth buffer
	VkFormat depthFormat = chooseSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		// Create Depth Buffer Image
		depthBufferImage[i] = createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &depthBufferImageMemory[i]);

		// Create Depth Buffer Image View
		depthBufferImageView[i] = createImageView(depthBufferImage[i], depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}
}




void VulkanRenderer::createFramebuffers()
{
	// Resize framebuffer count to equal swap chain image count
	swapChainFramebuffers.resize(swapChainImages.size());

	// Create a framebuffer for each swap chain image
	for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
	{
		std::array<VkImageView, 3> attachments = {	//追加
			swapChainImages[i].imageView,
			colourBufferImageView[i],
			depthBufferImageView[i]
		};

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass;										// Render Pass layout the Framebuffer will be used with
		framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();							// List of attachments (1:1 with Render Pass)
		framebufferCreateInfo.width = swapChainExtent.width;								// Framebuffer width
		framebufferCreateInfo.height = swapChainExtent.height;								// Framebuffer height
		framebufferCreateInfo.layers = 1;													// Framebuffer layers

		VkResult result = vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a Framebuffer!");
		}
	}
}


void VulkanRenderer::recordCommands(uint32_t currentImage)
{
	// Information about how to begin each command buffer
	VkCommandBufferBeginInfo bufferBeginInfo = {};
	bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	// Information about how to begin a render pass (only needed for graphical applications)
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass;							// Render Pass to begin
	renderPassBeginInfo.renderArea.offset = { 0, 0 };						// Start point of render pass in pixels
	renderPassBeginInfo.renderArea.extent = swapChainExtent;				// Size of region to run render pass on (starting at offset)

	std::array<VkClearValue, 3> clearValues = {};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].color = { 0.6f, 0.65f, 0.4f, 1.0f };
	clearValues[2].depthStencil.depth = 1.0f;

	renderPassBeginInfo.pClearValues = clearValues.data();					// List of clear values
	renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());

	renderPassBeginInfo.framebuffer = swapChainFramebuffers[currentImage];

	// Start recording commands to command buffer!
	VkResult result = vkBeginCommandBuffer(commandBuffers[currentImage], &bufferBeginInfo);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to start recording a Command Buffer!");
	}

	// Begin Render Pass
	vkCmdBeginRenderPass(commandBuffers[currentImage], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Bind Pipeline to be used in render pass
	vkCmdBindPipeline(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	for (size_t j = 0; j < modelList.size(); j++)
	{
		MeshModel thisModel = modelList[j];

		vkCmdPushConstants(
			commandBuffers[currentImage],
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT,		// Stage to push constants to
			0,								// Offset of push constants to update
			sizeof(Model),					// Size of data being pushed
			&thisModel.getModel());			// Actual data being pushed (can be array)

		for (size_t k = 0; k < thisModel.getMeshCount(); k++)
		{

			VkBuffer vertexBuffers[] = { thisModel.getMesh(k)->getVertexBuffer() };					// Buffers to bind
			VkDeviceSize offsets[] = { 0 };												// Offsets into buffers being bound
			vkCmdBindVertexBuffers(commandBuffers[currentImage], 0, 1, vertexBuffers, offsets);	// Command to bind vertex buffer before drawing with them

			// Bind mesh index buffer, with 0 offset and using the uint32 type
			vkCmdBindIndexBuffer(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

			// Dynamic Offset Amount
			// uint32_t dynamicOffset = static_cast<uint32_t>(modelUniformAlignment) * j;

			// "Push" constants to given shader stage directly (no buffer)


			std::array<VkDescriptorSet, 2> descriptorSetGroup = { descriptorSets[currentImage],
				samplerDescriptorSets[thisModel.getMesh(k)->getTexId()] };

			// Bind Descriptor Sets
			vkCmdBindDescriptorSets(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
				0, static_cast<uint32_t>(descriptorSetGroup.size()), descriptorSetGroup.data(), 0, nullptr);

			// Execute pipeline
			vkCmdDrawIndexed(commandBuffers[currentImage], thisModel.getMesh(k)->getIndexCount(), 1, 0, 0, 0);
		}
	}

	// Start second subpass
	vkCmdNextSubpass(commandBuffers[currentImage], VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, secondPipeline);
	vkCmdBindDescriptorSets(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, secondPipelineLayout,
		0, 1, &inputDescriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandBuffers[currentImage], 3, 1, 0, 0);

	// End Render Pass
	vkCmdEndRenderPass(commandBuffers[currentImage]);

	// Stop recording to command buffer
	result = vkEndCommandBuffer(commandBuffers[currentImage]);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to stop recording a Command Buffer!");
	}

}


void VulkanRenderer::getPhysicalDevice()
{
	// Enumerate Physical devices the vkInstance can access
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// If no devices available, then none support Vulkan!
	if (deviceCount == 0)
	{
		throw std::runtime_error("Can't find GPUs that support Vulkan Instance!");
	}

	// Get list of Physical Devices
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

	for (const auto& device : deviceList)
	{
		if (checkDeviceSuitable(device))
		{
			mainDevice.physicalDevice = device;
			break;
		}
	}

	// Get properties of our new device
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(mainDevice.physicalDevice, &deviceProperties);

	//minUniformBufferOffset = deviceProperties.limits.minUniformBufferOffsetAlignment;
}

void VulkanRenderer::allocateDynamicBufferTransferSpace()
{
	// Calculate alignment of model data
	/*modelUniformAlignment = (sizeof(UboModel) + minUniformBufferOffset - 1)
							& ~(minUniformBufferOffset - 1);

	// Create space in memory to hold dynamic buffer that is aligned to our required alignment and holds MAX_OBJECTS
	modelTransferSpace = (UboModel *)_aligned_malloc(modelUniformAlignment * MAX_OBJECTS, modelUniformAlignment);*/
}



bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	/*
	// Information about the device itself (ID, name, type, vendor, etc)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	*/

	// Information about what the device can do (geo shader, tess shader, wide lines, etc)
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);


	QueueFamilyIndices indices = getQueueFamilies(device);

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainValid = false;
	if (extensionsSupported)
	{
		SwapChainDetails swapChainDetails = getSwapChainDetails(device);
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();
	}

	return indices.isValid() && extensionsSupported && swapChainValid && deviceFeatures.samplerAnisotropy;	//追加
}


VkFormat VulkanRenderer::chooseSupportedFormat(const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags)
{
	// Loop through options and find compatible one
	for (VkFormat format : formats)
	{
		// Get properties for give format on this device
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(mainDevice.physicalDevice, format, &properties);

		// Depending on tiling choice, need to check for different bit flag
		if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & featureFlags) == featureFlags)
		{
			return format;
		}
	}

	throw std::runtime_error("Failed to find a matching format!");
}




