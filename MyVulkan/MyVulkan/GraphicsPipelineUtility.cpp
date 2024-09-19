#include "GraphicsPipelineUtility.h"



PipelineGenerator::PipelineGenerator()
{
	m_ClassName = "PipelineGenerator";
}

PipelineGenerator::~PipelineGenerator()
{
}

void PipelineGenerator::Create(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass)
{
	m_bCreated = true;

	// ��������p�ɘ_���f�o�C�X��ێ����Ă���
	m_LogicalDevice = logicalDevice;

	//�p�C�v���C�����C�A�E�g�̍쐬
	m_PipelineLayout = CreatePipelineLayout(logicalDevice);

	//�p�C�v���C���̍쐬
	m_Pipeline = CreateGraphicsPipeline(logicalDevice, extent, renderPass);
	
}

void PipelineGenerator::Destroy(vk::Device logicalDevice)
{
	logicalDevice.destroyPipelineLayout(m_PipelineLayout);
	logicalDevice.destroyPipeline(m_Pipeline);

	//�p�C�v���C���̍쐬��ɕs�v�ɂȂ����V�F�[�_�[���W���[����j��
	//vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);
}

vk::Pipeline PipelineGenerator::GetPipeline()
{
	CheckCreated();
	return m_Pipeline;
}

vk::PipelineLayout PipelineGenerator::GetPipelineLayout()
{
	CheckCreated();
	return m_PipelineLayout;
}


vk::Pipeline PipelineGenerator::CreateGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass)
{
	// Read in SPIR-V code of shaders
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// Create Shader Modules
	VkShaderModule vertexShaderModule = CreateShaderModule(vertexShaderCode);
	VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShaderCode);

	// -- SHADER STAGE CREATION INFORMATION --
	// Vertex Stage creation information
	VkPipelineShaderStageCreateInfo vertexShaderCreateInfo = {};
	vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;				// Shader Stage name
	vertexShaderCreateInfo.module = vertexShaderModule;						// Shader module to be used by stage
	vertexShaderCreateInfo.pName = "main";									// Entry point in to shader

	// Fragment Stage creation information
	VkPipelineShaderStageCreateInfo fragmentShaderCreateInfo = {};
	fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;				// Shader Stage name
	fragmentShaderCreateInfo.module = fragmentShaderModule;						// Shader module to be used by stage
	fragmentShaderCreateInfo.pName = "main";									// Entry point in to shader

	// Put shader stage creation info in to array
	// Graphics Pipeline creation info requires array of shader stage creates
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	// How the data for a single vertex (including info such as position, colour, texture coords, normals, etc) is as a whole
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;									// Can bind multiple streams of data, this defines which one
	bindingDescription.stride = sizeof(Vertex);						// Size of a single vertex object
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;		// How to move between data after each vertex.
	// VK_VERTEX_INPUT_RATE_INDEX		: Move on to the next vertex
	// VK_VERTEX_INPUT_RATE_INSTANCE	: Move to a vertex for the next instance

// How the data for an attribute is defined within a vertex
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

	// Position Attribute
	attributeDescriptions[0].binding = 0;							// Which binding the data is at (should be same as above)
	attributeDescriptions[0].location = 0;							// Location in shader where data will be read from
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;	// Format the data will take (also helps define size of data)
	attributeDescriptions[0].offset = offsetof(Vertex, position);		// Where this attribute is defined in the data for a single vertex

	// Colour Attribute
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	// Texture Attribute
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, texture);

	// -- VERTEX INPUT --
	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;											// List of Vertex Binding Descriptions (data spacing/stride information)
	vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();								// List of Vertex Attribute Descriptions (data format and where to bind to/from)


	// -- INPUT ASSEMBLY --
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;		// Primitive type to assemble vertices as
	inputAssembly.primitiveRestartEnable = VK_FALSE;					// Allow overriding of "strip" topology to start new primitives


	// -- VIEWPORT & SCISSOR --
	// Create a viewport info struct
	VkViewport viewport = {};
	viewport.x = 0.0f;									// x start coordinate
	viewport.y = 0.0f;									// y start coordinate
	viewport.width = (float)extent.width;		// width of viewport
	viewport.height = (float)extent.height;	// height of viewport
	viewport.minDepth = 0.0f;							// min framebuffer depth
	viewport.maxDepth = 1.0f;							// max framebuffer depth

	// Create a scissor info struct
	VkRect2D scissor = {};
	scissor.offset = { 0,0 };							// Offset to use region from
	scissor.extent = extent;					// Extent to describe region to use, starting at offset

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;


	// -- DYNAMIC STATES --
	// Dynamic states to enable
	//std::vector<VkDynamicState> dynamicStateEnables;
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	// Dynamic Viewport : Can resize in command buffer with vkCmdSetViewport(commandbuffer, 0, 1, &viewport);
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	// Dynamic Scissor	: Can resize in command buffer with vkCmdSetScissor(commandbuffer, 0, 1, &scissor);

	//// Dynamic State creation info
	//VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	//dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	//dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	//dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();


	// -- RASTERIZER --
	VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = {};
	rasterizerCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;					// Change if fragments beyond near/far planes are clipped (default) or clamped to plane
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;			// Whether to discard data and skip rasterizer. Never creates fragments, only suitable for pipeline without framebuffer output
	rasterizerCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;			// How to handle filling points between vertices
	rasterizerCreateInfo.lineWidth = 1.0f;								// How thick lines should be when drawn
	rasterizerCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;				// Which face of a tri to cull
	rasterizerCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;	// Winding to determine which side is front
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;					// Whether to add depth bias to fragments (good for stopping "shadow acne" in shadow mapping)


	// -- MULTISAMPLING --
	VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo = {};
	multisamplingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingCreateInfo.sampleShadingEnable = VK_FALSE;					// Enable multisample shading or not
	multisamplingCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;	// Number of samples to use per fragment


	// -- BLENDING --
	// Blending decides how to blend a new colour being written to a fragment, with the old value

	// Blend Attachment State (how blending is handled)
	VkPipelineColorBlendAttachmentState colourState = {};
	colourState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT	// Colours to apply blending to
		| VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colourState.blendEnable = VK_TRUE;													// Enable blending

	// Blending uses equation: (srcColorBlendFactor * new colour) colorBlendOp (dstColorBlendFactor * old colour)
	colourState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colourState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colourState.colorBlendOp = VK_BLEND_OP_ADD;

	// Summarised: (VK_BLEND_FACTOR_SRC_ALPHA * new colour) + (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old colour)
	//			   (new colour alpha * new colour) + ((1 - new colour alpha) * old colour)

	colourState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colourState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colourState.alphaBlendOp = VK_BLEND_OP_ADD;
	// Summarised: (1 * new alpha) + (0 * old alpha) = new alpha

	VkPipelineColorBlendStateCreateInfo colourBlendingCreateInfo = {};
	colourBlendingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colourBlendingCreateInfo.logicOpEnable = VK_FALSE;				// Alternative to calculations is to use logical operations
	colourBlendingCreateInfo.attachmentCount = 1;
	colourBlendingCreateInfo.pAttachments = &colourState;

	// -- PIPELINE LAYOUT --
	std::array<VkDescriptorSetLayout, 2> descriptorSetLayouts = { descriptorSetLayout, samplerSetLayout };

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;

	// Create Pipeline Layout
	VkResult result = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Pipeline Layout!");
	}


	// -- DEPTH STENCIL TESTING --
	VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo = {};
	depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilCreateInfo.depthTestEnable = VK_TRUE;				// Enable checking depth to determine fragment write
	depthStencilCreateInfo.depthWriteEnable = VK_TRUE;				// Enable writing to depth buffer (to replace old values)
	depthStencilCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;		// Comparison operation that allows an overwrite (is in front)
	depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;		// Depth Bounds Test: Does the depth value exist between two bounds
	depthStencilCreateInfo.stencilTestEnable = VK_FALSE;			// Enable Stencil Test


	// -- GRAPHICS PIPELINE CREATION --
	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stageCount = 2;									// Number of shader stages
	pipelineCreateInfo.pStages = shaderStages;							// List of shader stages
	pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;		// All the fixed function pipeline states
	pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pDynamicState = nullptr;
	pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	pipelineCreateInfo.pColorBlendState = &colourBlendingCreateInfo;
	pipelineCreateInfo.pDepthStencilState = &depthStencilCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;							// Pipeline Layout pipeline should use
	pipelineCreateInfo.renderPass = renderPass;							// Render pass description the pipeline is compatible with
	pipelineCreateInfo.subpass = 0;										// Subpass of render pass to use with pipeline

	// Pipeline Derivatives : Can create multiple pipelines that derive from one another for optimisation
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;	// Existing pipeline to derive from...
	pipelineCreateInfo.basePipelineIndex = -1;				// or index of pipeline being created to derive from (in case creating multiple at once)

	// Create Graphics Pipeline
	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Graphics Pipeline!");
	}

	// Destroy Shader Modules, no longer needed after Pipeline created
	vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);


	// CREATE SECOND PASS PIPELINE
	// Second pass shaders
	auto secondVertexShaderCode = readFile("Shaders/second_vert.spv");
	auto secondFragmentShaderCode = readFile("Shaders/second_frag.spv");

	// Build shaders
	VkShaderModule secondVertexShaderModule = createShaderModule(secondVertexShaderCode);
	VkShaderModule secondFragmentShaderModule = createShaderModule(secondFragmentShaderCode);

	// Set new shaders
	vertexShaderCreateInfo.module = secondVertexShaderModule;
	fragmentShaderCreateInfo.module = secondFragmentShaderModule;

	VkPipelineShaderStageCreateInfo secondShaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	// No vertex data for second pass
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	// Don't want to write to depth buffer
	depthStencilCreateInfo.depthWriteEnable = VK_FALSE;

	// Create new pipeline layout
	VkPipelineLayoutCreateInfo secondPipelineLayoutCreateInfo = {};
	secondPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	secondPipelineLayoutCreateInfo.setLayoutCount = 1;
	secondPipelineLayoutCreateInfo.pSetLayouts = &inputSetLayout;
	secondPipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	secondPipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	result = vkCreatePipelineLayout(logicalDevice, &secondPipelineLayoutCreateInfo, nullptr, &secondPipelineLayout);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Pipeline Layout!");
	}

	pipelineCreateInfo.pStages = secondShaderStages;	// Update second shader stage list
	pipelineCreateInfo.layout = secondPipelineLayout;	// Change pipeline layout for input attachment descriptor sets
	pipelineCreateInfo.subpass = 1;						// Use second subpass

	// Create second pipeline
	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &secondPipeline);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Graphics Pipeline!");
	}

	// Destroy second shader modules
	vkDestroyShaderModule(logicalDevice, secondFragmentShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, secondVertexShaderModule, nullptr);
}





vk::PipelineLayout PipelineGenerator::CreatePipelineLayout(vk::Device logicalDevice)
{
	// -- �p�C�v���C�����C�A�E�g (TODO: �����̃f�B�X�N���v�^�Z�b�g���C�A�E�g��K�p����) --
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	try { return logicalDevice.createPipelineLayout(pipelineLayoutInfo); }
	catch (const std::runtime_error& e) { throw std::runtime_error("�p�C�v���C�����C�A�E�g�̍쐬�Ɏ��s���܂����I"); }
}

std::vector<vk::PipelineShaderStageCreateInfo> PipelineGenerator::GetShaderStageInfo(vk::Device logicalDevice)
{
	// �V�F�[�_�[��SPIR-V�R�[�h��ǂݍ���
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// �V�F�[�_�[���W���[�����쐬����
	vk::ShaderModule vertexShaderModule = ShaderUtility::createShaderModule(vertexShaderCode, logicalDevice);
	vk::ShaderModule fragmentShaderModule = ShaderUtility::createShaderModule(fragmentShaderCode, logicalDevice);

	// -- �V�F�[�_�[�X�e�[�W�쐬��� --
	// ���_�V�F�[�_�[�X�e�[�W�̍쐬���
	vk::PipelineShaderStageCreateInfo vertexShaderCreateInfo;
	vertexShaderCreateInfo.stage = vk::ShaderStageFlagBits::eVertex;		// �V�F�[�_�[�X�e�[�W�̎��
	vertexShaderCreateInfo.module = vertexShaderModule;						// �g�p����V�F�[�_�[���W���[��
	vertexShaderCreateInfo.pName = "main";									// �G���g���[�|�C���g

	// �t���O�����g�V�F�[�_�[�X�e�[�W�̍쐬���
	vk::PipelineShaderStageCreateInfo fragmentShaderCreateInfo;
	fragmentShaderCreateInfo.stage = vk::ShaderStageFlagBits::eFragment;        // �V�F�[�_�[�X�e�[�W�̎��
	fragmentShaderCreateInfo.module = fragmentShaderModule;						// �g�p����V�F�[�_�[���W���[��
	fragmentShaderCreateInfo.pName = "main";									// �G���g���[�|�C���g

	// �V�F�[�_�[�X�e�[�W�쐬����z��Ɋi�[
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	return shaderStages;
}

vk::PipelineVertexInputStateCreateInfo PipelineGenerator::GetVertexInputStateInfo()
{
	// -- ���_���� (TODO: ���\�[�X���쐬���ꂽ�璸�_�̐�����ǉ�����) --
	vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// ���_�o�C���f�B���O�̐��� (�f�[�^�̊Ԋu��X�g���C�h���)
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// ���_�����̐��� (�f�[�^�t�H�[�}�b�g��o�C���h��/��)

	return vertexInputCreateInfo;
}

vk::PipelineInputAssemblyStateCreateInfo PipelineGenerator::GetInputAssemblyStateInfo()
{
	// ���̓A�Z���u���X�e�[�g��ݒ肷��
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;         // ���_���A�Z���u�������{�}�`�̎��
	inputAssembly.primitiveRestartEnable = VK_FALSE;                       // "strip" �g�|���W�[���㏑�����ĐV�����v���~�e�B�u���J�n���邱�Ƃ������邩
	
	return inputAssembly;
}

vk::PipelineRasterizationStateCreateInfo PipelineGenerator::GetRasterizationStateInfo()
{
	// ���X�^���C�U�[�X�e�[�g��ݒ肷��
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;				// �t���O�����g���ߐ�/���u���ʂ𒴂����ꍇ�ɃN���b�v (�f�t�H���g) �܂��͕��ʂɃN�����v���邩��ύX
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;		// �f�[�^��j�������X�^���C�U���X�L�b�v���邩�ǂ����B�t���[���o�b�t�@�o�͂Ȃ��̃p�C�v���C���ɂ̂ݓK���Ă���
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;		// ���_�Ԃ̃|�C���g�̓h��Ԃ����@
	rasterizerCreateInfo.lineWidth = 1.0f;							// �`�掞�̐��̑���
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;	// �O�p�`�̂ǂ̖ʂ��J�����O���邩
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;		// �O�ʂ����肷�邽�߂̊�������
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;				// �t���O�����g�ɐ[�x�o�C�A�X��ǉ����邩 (�V���h�E�}�b�s���O�� "�e�A�N�l" ��h���̂ɗL��)

	return rasterizerCreateInfo;
}

vk::PipelineMultisampleStateCreateInfo PipelineGenerator::GetMultisampleStateInfo()
{
	// �}���`�T���v�����O�X�e�[�g��ݒ肷��
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// �}���`�T���v���V�F�[�f�B���O��L���ɂ��邩�ǂ���
	multisamplingCreateInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// �t���O�����g���ƂɎg�p����T���v����
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// �V�F�[�f�B���O�̍ŏ��T���v����
	multisamplingCreateInfo.setPSampleMask(nullptr);								// �T���v�� �}�X�N
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// �A���t�@ �}�X�N
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// �A���t�@ �`���l���̍ő�l

	return multisamplingCreateInfo;
}


std::vector<vk::PipelineColorBlendAttachmentState> PipelineGenerator::GetColorBlendAttachmentState()
{
	// �u�����h�A�^�b�`�����g�X�e�[�g��ݒ肷��
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = 
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_TRUE; // �u�����f�B���O��L���ɂ���

	// �u�����h�A���t�@�̌v�Z����ݒ肷��
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;

	// �A���t�@�u�����h�̌v�Z����ݒ肷��
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
	// �v��: (1 * �V�����A���t�@) + (0 * �Â��A���t�@) = �V�����A���t�@

	return std::vector<vk::PipelineColorBlendAttachmentState>{colorBlendAttachment};
}

vk::PipelineColorBlendStateCreateInfo PipelineGenerator::GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* pAttachments)
{
	vk::PipelineColorBlendStateCreateInfo colorBlendingInfo;
	colorBlendingInfo.pNext;
	colorBlendingInfo.flags;
	colorBlendingInfo.logicOpEnable = VK_FALSE;				// �v�Z�̑���ɘ_�����Z���g�p���邩�ǂ���
	colorBlendingInfo.logicOp;
	colorBlendingInfo.attachmentCount = pAttachments->size();
	colorBlendingInfo.pAttachments = pAttachments->data();
	colorBlendingInfo.blendConstants;

	return colorBlendingInfo;
}
