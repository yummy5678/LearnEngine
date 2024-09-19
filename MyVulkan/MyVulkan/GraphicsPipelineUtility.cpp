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

	// 解放処理用に論理デバイスを保持しておく
	m_LogicalDevice = logicalDevice;

	//パイプラインレイアウトの作成
	m_PipelineLayout = CreatePipelineLayout(logicalDevice);

	//パイプラインの作成
	m_Pipeline = CreateGraphicsPipeline(logicalDevice, extent, renderPass);
	
}

void PipelineGenerator::Destroy(vk::Device logicalDevice)
{
	logicalDevice.destroyPipelineLayout(m_PipelineLayout);
	logicalDevice.destroyPipeline(m_Pipeline);

	//パイプラインの作成後に不要になったシェーダーモジュールを破棄
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
	// -- パイプラインレイアウト (TODO: 将来のディスクリプタセットレイアウトを適用する) --
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	try { return logicalDevice.createPipelineLayout(pipelineLayoutInfo); }
	catch (const std::runtime_error& e) { throw std::runtime_error("パイプラインレイアウトの作成に失敗しました！"); }
}

std::vector<vk::PipelineShaderStageCreateInfo> PipelineGenerator::GetShaderStageInfo(vk::Device logicalDevice)
{
	// シェーダーのSPIR-Vコードを読み込む
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// シェーダーモジュールを作成する
	vk::ShaderModule vertexShaderModule = ShaderUtility::createShaderModule(vertexShaderCode, logicalDevice);
	vk::ShaderModule fragmentShaderModule = ShaderUtility::createShaderModule(fragmentShaderCode, logicalDevice);

	// -- シェーダーステージ作成情報 --
	// 頂点シェーダーステージの作成情報
	vk::PipelineShaderStageCreateInfo vertexShaderCreateInfo;
	vertexShaderCreateInfo.stage = vk::ShaderStageFlagBits::eVertex;		// シェーダーステージの種類
	vertexShaderCreateInfo.module = vertexShaderModule;						// 使用するシェーダーモジュール
	vertexShaderCreateInfo.pName = "main";									// エントリーポイント

	// フラグメントシェーダーステージの作成情報
	vk::PipelineShaderStageCreateInfo fragmentShaderCreateInfo;
	fragmentShaderCreateInfo.stage = vk::ShaderStageFlagBits::eFragment;        // シェーダーステージの種類
	fragmentShaderCreateInfo.module = fragmentShaderModule;						// 使用するシェーダーモジュール
	fragmentShaderCreateInfo.pName = "main";									// エントリーポイント

	// シェーダーステージ作成情報を配列に格納
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = { vertexShaderCreateInfo, fragmentShaderCreateInfo };

	return shaderStages;
}

vk::PipelineVertexInputStateCreateInfo PipelineGenerator::GetVertexInputStateInfo()
{
	// -- 頂点入力 (TODO: リソースが作成されたら頂点の説明を追加する) --
	vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// 頂点バインディングの説明 (データの間隔やストライド情報)
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// 頂点属性の説明 (データフォーマットやバインド先/元)

	return vertexInputCreateInfo;
}

vk::PipelineInputAssemblyStateCreateInfo PipelineGenerator::GetInputAssemblyStateInfo()
{
	// 入力アセンブリステートを設定する
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;         // 頂点をアセンブルする基本図形の種類
	inputAssembly.primitiveRestartEnable = VK_FALSE;                       // "strip" トポロジーを上書きして新しいプリミティブを開始することを許可するか
	
	return inputAssembly;
}

vk::PipelineRasterizationStateCreateInfo PipelineGenerator::GetRasterizationStateInfo()
{
	// ラスタライザーステートを設定する
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.depthClampEnable = VK_FALSE;				// フラグメントが近接/遠隔平面を超えた場合にクリップ (デフォルト) または平面にクランプするかを変更
	rasterizerCreateInfo.rasterizerDiscardEnable = VK_FALSE;		// データを破棄しラスタライザをスキップするかどうか。フレームバッファ出力なしのパイプラインにのみ適している
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;		// 頂点間のポイントの塗りつぶし方法
	rasterizerCreateInfo.lineWidth = 1.0f;							// 描画時の線の太さ
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;	// 三角形のどの面をカリングするか
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;		// 前面を決定するための巻き方向
	rasterizerCreateInfo.depthBiasEnable = VK_FALSE;				// フラグメントに深度バイアスを追加するか (シャドウマッピングで "影アクネ" を防ぐのに有効)

	return rasterizerCreateInfo;
}

vk::PipelineMultisampleStateCreateInfo PipelineGenerator::GetMultisampleStateInfo()
{
	// マルチサンプリングステートを設定する
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// マルチサンプルシェーディングを有効にするかどうか
	multisamplingCreateInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// フラグメントごとに使用するサンプル数
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// シェーディングの最小サンプル数
	multisamplingCreateInfo.setPSampleMask(nullptr);								// サンプル マスク
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// アルファ マスク
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// アルファ チャネルの最大値

	return multisamplingCreateInfo;
}


std::vector<vk::PipelineColorBlendAttachmentState> PipelineGenerator::GetColorBlendAttachmentState()
{
	// ブレンドアタッチメントステートを設定する
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = 
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_TRUE; // ブレンディングを有効にする

	// ブレンドアルファの計算式を設定する
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;

	// アルファブレンドの計算式を設定する
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
	// 要約: (1 * 新しいアルファ) + (0 * 古いアルファ) = 新しいアルファ

	return std::vector<vk::PipelineColorBlendAttachmentState>{colorBlendAttachment};
}

vk::PipelineColorBlendStateCreateInfo PipelineGenerator::GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* pAttachments)
{
	vk::PipelineColorBlendStateCreateInfo colorBlendingInfo;
	colorBlendingInfo.pNext;
	colorBlendingInfo.flags;
	colorBlendingInfo.logicOpEnable = VK_FALSE;				// 計算の代わりに論理演算を使用するかどうか
	colorBlendingInfo.logicOp;
	colorBlendingInfo.attachmentCount = pAttachments->size();
	colorBlendingInfo.pAttachments = pAttachments->data();
	colorBlendingInfo.blendConstants;

	return colorBlendingInfo;
}
