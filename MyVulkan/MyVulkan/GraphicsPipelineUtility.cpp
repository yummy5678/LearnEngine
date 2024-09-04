#include "GraphicsPipelineUtility.h"
#include "SwapChainUtility.h"


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
	//シェーダーステージの作成
	auto shaderStageInfos = GetShaderStageInfo(logicalDevice);

	//vertexInputCreateInfoの作成
	auto vertexInputCreateInfo = GetVertexInputStateInfo();

	//入力アセンブリステートの作成
	auto inputAssemblyInfo = GetInputAssemblyStateInfo();

	//ビューポートステートの作成
	m_viewportGenerator.Create(extent);
	auto viewportStateCreateInfo = m_viewportGenerator.GetCreateInfoPointer();

	//ラスタライザーステートの作成
	auto rasterizerCreateInfo = GetRasterizationStateInfo();

	// -- ダイナミックステート --
	// 有効にするダイナミックステート
	//std::vector<VkDynamicState> dynamicStateEnables;
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_VIEWPORT);	// ダイナミックビューポート: vkCmdSetViewport(commandbuffer, 0, 1, &viewport); でコマンドバッファ内でリサイズ可能
	//dynamicStateEnables.push_back(VK_DYNAMIC_STATE_SCISSOR);	// ダイナミックシザー: vkCmdSetScissor(commandbuffer, 0, 1, &scissor); でコマンドバッファ内でリサイズ可能

	//// ダイナミックステートの作成情報
	//VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	//dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	//dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
	//dynamicStateCreateInfo.pDynamicStates = dynamicStateEnables.data();

	//マルチサンプリングステートの作成
	auto multisamplingInfo = GetMultisampleStateInfo();

	//ブレンドステートの作成
	auto colorBlendAttachment = GetColorBlendAttachmentState();
	auto colorBlendCreateInfo = GetColorBlendStateInfo(&colorBlendAttachment);


	// -- グラフィックスパイプラインの作成 --
	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.pNext;
	pipelineInfo.flags;
	pipelineInfo.stageCount			= shaderStageInfos.size();	// シェーダーステージの数
	pipelineInfo.pStages			= shaderStageInfos.data();	// シェーダーステージのリスト
	pipelineInfo.pVertexInputState	= &vertexInputCreateInfo;		// すべての固定機能パイプラインステート
	pipelineInfo.pInputAssemblyState= &inputAssemblyInfo;
	pipelineInfo.pTessellationState	= nullptr;					//未作成
	pipelineInfo.pViewportState		= viewportStateCreateInfo;
	pipelineInfo.pRasterizationState= &rasterizerCreateInfo;
	pipelineInfo.pMultisampleState	= &multisamplingInfo;
	pipelineInfo.pDepthStencilState	= nullptr;					//未作成
	pipelineInfo.pColorBlendState	= &colorBlendCreateInfo;
	pipelineInfo.pDynamicState		= nullptr;					//未作成
	pipelineInfo.layout				= m_PipelineLayout;			// パイプラインが使用するパイプラインレイアウト
	pipelineInfo.renderPass			= renderPass;				// パイプラインが互換性のあるレンダーパスの説明
	pipelineInfo.subpass			= 0;						// パイプラインで使用するサブパス

	// パイプラインの派生: 最適化のために相互に派生する複数のパイプラインを作成できる
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;// 派生元の既存のパイプライン
	pipelineInfo.basePipelineIndex = -1;// または作成中のパイプラインのインデックス (複数作成する場合)

	// グラフィックスパイプラインを作成
	auto result = logicalDevice.createGraphicsPipeline(nullptr, pipelineInfo);
	if (result.result != vk::Result::eSuccess)
	{
		throw std::runtime_error("グラフィックスパイプラインの作成に失敗しました！");
	}
	return result.value;
}

vk::PipelineLayout PipelineGenerator::CreatePipelineLayout(vk::Device logicalDevice)
{
	// -- パイプラインレイアウト (TODO: 将来のディスクリプタセットレイアウトを適用する) --
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	try
	{
		return logicalDevice.createPipelineLayout(pipelineLayoutInfo);
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("パイプラインレイアウトの作成に失敗しました！");
	}
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
	rasterizerCreateInfo.polygonMode = vk::PolygonMode::eFill;	// 頂点間のポイントの塗りつぶし方法
	rasterizerCreateInfo.lineWidth = 1.0f;						// 描画時の線の太さ
	rasterizerCreateInfo.cullMode = vk::CullModeFlagBits::eBack;	// 三角形のどの面をカリングするか
	rasterizerCreateInfo.frontFace = vk::FrontFace::eClockwise;	// 前面を決定するための巻き方向
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
