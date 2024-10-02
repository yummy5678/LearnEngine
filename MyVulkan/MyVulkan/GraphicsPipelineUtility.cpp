#include "GraphicsPipelineUtility.h"


PipelineGenerator::PipelineGenerator()
{
	m_ClassName = "PipelineGenerator";
}

PipelineGenerator::~PipelineGenerator()
{
}

void PipelineGenerator::Create(vk::Device logicalDevice)
{
	m_bCreated = true;

	// 解放処理用に論理デバイスを保持しておく
	m_LogicalDevice = logicalDevice;

	//パイプラインレイアウトの作成
	m_PipelineLayout = logicalDevice.createPipelineLayout(m_PipelineLayoutInfo);

	//パイプラインの作成
	m_Pipeline = logicalDevice.createGraphicsPipeline(m_PipelineInfo);
		//CreateGraphicsPipeline(logicalDevice, extent, renderPass);
	
}

void PipelineGenerator::CreateInfo(vk::Extent2D extent, vk::RenderPass renderPass, std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfo)
{
	// ビューポートの設定情報を作成
	m_ViewportInfo = CreateViewportStateInfo(extent);

	// ラスタライザーの設定
	m_RasterizerInfo = CreateRasterizerStateInfo();

	// マルチサンプリングの設定
	m_MultisamplingInfo = CreateMultisampleStateInfo(vk::SampleCountFlagBits::e1);

	// カラーブレンディングの設定
	m_ColorBlendInfo = CreateColorBlendingStateInfo();

	// -- DEPTH STENCIL TESTING --
	m_DepthStencilInfo = CreateDepthStencilStateInfo(true,false);



	m_PipelineInfo.setStages(shaderStageInfo);							// シェーダーステージ
	m_PipelineInfo.setPVertexInputState(&vertexInputInfo);		// All the fixed function pipeline states
	m_PipelineInfo.setPInputAssemblyState(&inputAssemblyInfo);
	m_PipelineInfo.setPViewportState(&m_ViewportInfo);
	m_PipelineInfo.setPDynamicState(nullptr);						//ダイナミックステートとは:パイプラインを作り直さなくても一部情報を変更できる機能
	m_PipelineInfo.setPRasterizationState(&m_RasterizerInfo);
	m_PipelineInfo.setPMultisampleState(&m_MultisamplingInfo);
	m_PipelineInfo.setPColorBlendState(&m_ColorBlendInfo);
	m_PipelineInfo.setPDepthStencilState(&m_DepthStencilInfo);
	m_PipelineInfo.layout = m_PipelineLayout;							// パイプラインと互換性があるレンダーパス
	m_PipelineInfo.renderPass = renderPass;
	m_PipelineInfo.subpass = 0;										// パイプラインで使用するレンダー パスのサブパス

	// Pipeline Derivatives : Can create multiple pipelines that derive from one another for optimisation
	m_PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;	// 派生元の既存のパイプライン
	m_PipelineInfo.basePipelineIndex = -1;				// または派生元として作成中のパイプラインのインデックス (複数を同時に作成する場合)


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
	// シェーダー読み込み
	//m_ShaderLoader.LoadShader(logicalDevice, "", "");

	/*auto viewportInfo = CreateViewportStateInfo(extent);*/

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


	// ラスタライザーの設定
	//vk::PipelineRasterizationStateCreateInfo rasterizerInfo = CreateRasterizerStateInfo();

	// マルチサンプリングの設定
	//auto multisamplingInfo = CreateMultisampleStateInfo(vk::SampleCountFlagBits::e1);

	// カラーブレンディングの設定
	//auto colorBlendingInfo = CreateColorBlendingStateInfo();


	// -- DEPTH STENCIL TESTING --
	//auto depthStencilInfo = CreateDepthStencilStateInfo(true,false);


	auto shaderStage = shader.GetShaderStages();
	// -- GRAPHICS PIPELINE CREATION --
	vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.setStages(shaderStage);							// シェーダーステージ
	pipelineCreateInfo.setPVertexInputState(&vertexInputInfo);		// All the fixed function pipeline states
	pipelineCreateInfo.setPInputAssemblyState(&inputAssemblyInfo);
	pipelineCreateInfo.setPViewportState(&viewportInfo);
	pipelineCreateInfo.setPDynamicState(nullptr);						//ダイナミックステートとは:パイプラインを作り直さなくても一部情報を変更できる機能
	pipelineCreateInfo.setPRasterizationState(&rasterizerInfo);
	pipelineCreateInfo.setPMultisampleState(&multisamplingInfo);
	pipelineCreateInfo.setPColorBlendState(&colorBlendingInfo);
	pipelineCreateInfo.setPDepthStencilState(&depthStencilInfo);
	pipelineCreateInfo.layout = pipelineLayout;							// パイプラインと互換性があるレンダーパス
	pipelineCreateInfo.renderPass = renderPass;							
	pipelineCreateInfo.subpass = 0;										// パイプラインで使用するレンダー パスのサブパス

	// Pipeline Derivatives : Can create multiple pipelines that derive from one another for optimisation
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;	// 派生元の既存のパイプライン
	pipelineCreateInfo.basePipelineIndex = -1;				// または派生元として作成中のパイプラインのインデックス (複数を同時に作成する場合)

	// Create Graphics Pipeline
	result = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &graphicsPipeline);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Graphics Pipeline!");
	}
}





vk::PipelineLayout PipelineGenerator::CreatePipelineLayout(vk::Device logicalDevice)
{
	// -- PIPELINE LAYOUT --
	std::array<vk::DescriptorSetLayout, 2> descriptorSetLayouts = { descriptorSetLayout, samplerSetLayout };

	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setLayoutCount = descriptorSetLayouts.size();
	pipelineLayoutCreateInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
	pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantModelRange;

	try { return logicalDevice.createPipelineLayout(pipelineLayoutCreateInfo); }
	catch (const std::runtime_error& e) { throw std::runtime_error("パイプラインレイアウトの作成に失敗しました！"); }
}

inline vk::PipelineViewportStateCreateInfo PipelineGenerator::CreateViewportStateInfo(vk::Extent2D extent)
{
	// ビューポートの情報を構築する
	vk::Viewport viewport;
	viewport.setX(0.0f);									// x座標の開始位置
	viewport.setY(0.0f);									// y座標の開始位置
	viewport.setWidth(static_cast<float>(extent.width));	// ビューポートの幅
	viewport.setHeight(static_cast<float>(extent.height));	// ビューポートの高さ
	viewport.setMinDepth(0.0f);								// フレームバッファの最小深度
	viewport.setMaxDepth(1.0f);								// フレームバッファの最大深度

	// シザー(ビューポートの画面を切り抜く領域)の情報を構築する
	vk::Rect2D scissor;
	scissor.setOffset({ 0, 0 });							// 使用する領域のオフセット
	scissor.setExtent(extent);								// 使用する領域のサイズ

	// ビューポートとシザーの状態を設定するための構造体を作成
	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo = {};
	viewportStateCreateInfo.setViewportCount(1);		// ビューポートの数
	viewportStateCreateInfo.setPViewports(&viewport);	// ビューポートのポインタ
	viewportStateCreateInfo.setScissorCount(1);			// シザーの数
	viewportStateCreateInfo.setPScissors(&scissor);		// シザーのポインタ

	return viewportStateCreateInfo;
}

inline vk::PipelineRasterizationStateCreateInfo PipelineGenerator::CreateRasterizerStateInfo()
{
	// ラスタライゼーションステートの設定
	vk::PipelineRasterizationStateCreateInfo rasterizationInfo = {};
	rasterizationInfo.setDepthClampEnable(VK_FALSE);										// 深度クランプを無効
	rasterizationInfo.setRasterizerDiscardEnable(VK_FALSE);									// ラスタライザーを有効
	rasterizationInfo.setPolygonMode(vk::PolygonMode::eFill);								// 塗りつぶしモード
	rasterizationInfo.setCullMode(vk::CullModeFlagBits::eBack);								// 背面カリング
	rasterizationInfo.setFrontFace(vk::FrontFace::eCounterClockwise);						// 順方向を反時計回りに設定
	rasterizationInfo.setDepthBiasEnable(VK_FALSE);											// 深度バイアスを無効
	rasterizationInfo.setDepthBiasConstantFactor(0.0f);										// 深度バイアスの定数因子
	rasterizationInfo.setDepthBiasClamp(0.0f);												// 深度バイアスのクランプ値
	rasterizationInfo.setDepthBiasSlopeFactor(0.0f);										// 深度バイアスのスロープ因子

	return rasterizationInfo;
}

inline vk::PipelineMultisampleStateCreateInfo PipelineGenerator::CreateMultisampleStateInfo(vk::SampleCountFlagBits sampleValue)
{
	// マルチサンプリングの設定
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setRasterizationSamples(sampleValue);					// マルチサンプリングを指定
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// サンプルシェーディングを無効
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// サンプルシェーディングの最小比率
	multisamplingCreateInfo.setPSampleMask(nullptr);								// サンプルマスクを指定（通常はnullptr）
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// アルファカバレッジを無効
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// アルファ値を1に固定を無効

	return multisamplingCreateInfo;
}

inline vk::PipelineColorBlendStateCreateInfo PipelineGenerator::CreateColorBlendingStateInfo()
{
	// カラーブレンディングの設定

	// ブレンドアタッチメントステート (どのようにブレンディングを処理するかを設定)
	vk::PipelineColorBlendAttachmentState colorState; // ブレンド状態を初期化

	// 書き込みマスクを設定
	// どの色成分にブレンディングを適用するかを指定
	colorState.colorWriteMask = 
		vk::ColorComponentFlagBits::eR |	// 赤
		vk::ColorComponentFlagBits::eG |	// 緑
		vk::ColorComponentFlagBits::eB |	// 青		
		vk::ColorComponentFlagBits::eA;		// アルファ(透明度)

	colorState.blendEnable = VK_TRUE; // ブレンディングを有効にする

	// ブレンディングの計算に使用する要素を設定
	// 新しい色と古い色をどのように合成するかを定義
	colorState.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha; // 新しい色のアルファ値を使用
	colorState.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha; // 古い色のアルファ値を反転

	// ブレンド演算を加算に設定
	colorState.colorBlendOp = vk::BlendOp::eAdd; // 新しい色と古い色を足し合わせる
	// 新しい色のアルファ値 * 新しい色 + (1 - 新しい色のアルファ値) * 古い色

	// アルファブレンドの設定
	colorState.srcAlphaBlendFactor = vk::BlendFactor::eOne;		// 新しいアルファをそのまま使用
	colorState.dstAlphaBlendFactor = vk::BlendFactor::eZero;	// 古いアルファは無視

	// アルファブレンド演算も加算に設定
	colorState.alphaBlendOp = vk::BlendOp::eAdd;				// 新しいアルファと古いアルファを足し合わせる
	// 1 * 新しいアルファ + 0 * 古いアルファ = 新しいアルファ

	// 全体のブレンディング設定をまとめる
	vk::PipelineColorBlendStateCreateInfo colorBlendingCreateInfo;	// ブレンド状態の初期化
	colorBlendingCreateInfo.logicOpEnable = VK_FALSE;				// 論理演算を無効にする
	colorBlendingCreateInfo.attachmentCount = 1;					// アタッチメントの数を1に設定
	colorBlendingCreateInfo.pAttachments = &colorState;				// 設定したブレンド状態を指定

	return colorBlendingCreateInfo;

}

inline vk::PipelineDepthStencilStateCreateInfo PipelineGenerator::CreateDepthStencilStateInfo(bool depth, bool stencil)
{
	// 深度およびステンシルステートの設定

	// 深度およびステンシルステートの作成情報を初期化
	vk::PipelineDepthStencilStateCreateInfo depthStencilCreateInfo;
	depthStencilCreateInfo.depthTestEnable = depth;					// 深度テスト
	depthStencilCreateInfo.depthWriteEnable = VK_TRUE;				// 深度バッファへの書き込み
	depthStencilCreateInfo.depthCompareOp = vk::CompareOp::eLess;	// 深度比較オペレーションの設定
	depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;		// 深度バウンズテスト
	depthStencilCreateInfo.stencilTestEnable = stencil;				// ステンシルテストを無効にする

	if(stencil == true)
	{
		// ステンシルオペレーションの設定例（ステンシルテストを有効にした場合）
		 depthStencilCreateInfo.front.failOp = vk::StencilOp::eKeep;			// フロント面での失敗時のオペレーション
		 depthStencilCreateInfo.front.passOp = vk::StencilOp::eReplace;			// フロント面での成功時のオペレーション
		 depthStencilCreateInfo.front.depthFailOp = vk::StencilOp::eReplace;	// フロント面での深度失敗時のオペレーション
		 depthStencilCreateInfo.front.compareOp = vk::CompareOp::eAlways;		// フロント面での比較オペレーション
		 depthStencilCreateInfo.front.compareMask = 0xFF;						// フロント面の比較マスク
		 depthStencilCreateInfo.front.writeMask = 0xFF;							// フロント面の書き込みマスク
		 depthStencilCreateInfo.front.reference = 1;							// フロント面のリファレンス値
		
		 depthStencilCreateInfo.back.failOp = vk::StencilOp::eKeep;				// バック面での失敗時のオペレーション
		 depthStencilCreateInfo.back.passOp = vk::StencilOp::eReplace;			// バック面での成功時のオペレーション
		 depthStencilCreateInfo.back.depthFailOp = vk::StencilOp::eReplace;		// バック面での深度失敗時のオペレーション
		 depthStencilCreateInfo.back.compareOp = vk::CompareOp::eAlways;		// バック面での比較オペレーション
		 depthStencilCreateInfo.back.compareMask = 0xFF;						// バック面の比較マスク
		 depthStencilCreateInfo.back.writeMask = 0xFF;							// バック面の書き込みマスク
		 depthStencilCreateInfo.back.reference = 1;								// バック面のリファレンス値
	}

	return depthStencilCreateInfo;
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




















