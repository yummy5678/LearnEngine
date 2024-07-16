#include "GraphicsPipelineUtility.h"
#include "SwapChainUtility.h"


vk::UniquePipeline GraphicsPipelineUtility::createGraphicsPipeline(vk::Device logicalDevice, vk::Extent2D extent,vk::PipelineLayout pipelineLayout, vk::RenderPass renderPass)
{
	// シェーダーのSPIR-Vコードを読み込む
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// シェーダーモジュールを作成する
	vk::ShaderModule vertexShaderModule   = ShaderUtility::createShaderModule(vertexShaderCode,logicalDevice);
	vk::ShaderModule fragmentShaderModule = ShaderUtility::createShaderModule(fragmentShaderCode,logicalDevice);

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
	vk::PipelineShaderStageCreateInfo shaderStages[] = { vertexShaderCreateInfo, fragmentShaderCreateInfo };


	// -- 頂点入力 (TODO: リソースが作成されたら頂点の説明を追加する) --
	vk::PipelineVertexInputStateCreateInfo vertexInputCreateInfo;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;			// 頂点バインディングの説明 (データの間隔やストライド情報)
	vertexInputCreateInfo.vertexAttributeDescriptionCount = 0;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;		// 頂点属性の説明 (データフォーマットやバインド先/元)


	// 入力アセンブリステートを設定する
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = vk::StructureType::ePipelineInputAssemblyStateCreateInfo;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;         // 頂点をアセンブルする基本図形の種類
	inputAssembly.primitiveRestartEnable = VK_FALSE;                       // "strip" トポロジーを上書きして新しいプリミティブを開始することを許可するか

	// -- ビューポート & シザー --
	// ビューポート情報の構造体を作成
	vk::Viewport viewport = {};
	viewport.x = 0.0f;									// x 開始座標
	viewport.y = 0.0f;									// y 開始座標
	viewport.width = (float)extent.width;				// ビューポートの幅
	viewport.height = (float)extent.height;				// ビューポートの高さ
	viewport.minDepth = 0.0f;							// フレームバッファの最小深度
	viewport.maxDepth = 1.0f;							// フレームバッファの最大深度

	// シザー情報の構造体を作成
	vk::Rect2D scissor = {};
	scissor.offset = VkOffset2D{ 0,0 };		// 使用する領域のオフセット
	scissor.extent = extent;				// 使用する領域の範囲とオフセットから開始

	// ビューポートステート作成情報を設定する
	vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = vk::StructureType::ePipelineViewportStateCreateInfo;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;



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


// ラスタライザーステートを設定する
	vk::PipelineRasterizationStateCreateInfo rasterizerCreateInfo;
	rasterizerCreateInfo.setDepthClampEnable(VK_FALSE);				// フラグメントが近接/遠隔平面を超えた場合にクリップ (デフォルト) または平面にクランプするかを変更
	rasterizerCreateInfo.setRasterizerDiscardEnable(VK_FALSE);		// データを破棄しラスタライザをスキップするかどうか。フレームバッファ出力なしのパイプラインにのみ適している
	rasterizerCreateInfo.setPolygonMode(vk::PolygonMode::eFill);	// 頂点間のポイントの塗りつぶし方法
	rasterizerCreateInfo.setLineWidth(1.0f);						// 描画時の線の太さ
	rasterizerCreateInfo.setCullMode(vk::CullModeFlagBits::eBack);	// 三角形のどの面をカリングするか
	rasterizerCreateInfo.setFrontFace(vk::FrontFace::eClockwise);	// 前面を決定するための巻き方向
	rasterizerCreateInfo.setDepthBiasEnable(VK_FALSE);				// フラグメントに深度バイアスを追加するか (シャドウマッピングで "影アクネ" を防ぐのに有効)



	// マルチサンプリングステートを設定する
	vk::PipelineMultisampleStateCreateInfo multisamplingCreateInfo;
	multisamplingCreateInfo.setSampleShadingEnable(VK_FALSE);						// マルチサンプルシェーディングを有効にするかどうか
	multisamplingCreateInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// フラグメントごとに使用するサンプル数
	multisamplingCreateInfo.setMinSampleShading(1.0f);								// シェーディングの最小サンプル数
	multisamplingCreateInfo.setPSampleMask(nullptr);								// サンプル マスク
	multisamplingCreateInfo.setAlphaToCoverageEnable(VK_FALSE);						// アルファ マスク
	multisamplingCreateInfo.setAlphaToOneEnable(VK_FALSE);							// アルファ チャネルの最大値


	// -- ブレンディング --
	// ブレンディングは、書き込まれる新しい色と古い値をどのようにブレンドするかを決定する

// ブレンドアタッチメントステートを設定する
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR |
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

	vk::PipelineColorBlendStateCreateInfo colourBlendingCreateInfo;
	colourBlendingCreateInfo.pNext;
	colourBlendingCreateInfo.flags;
	colourBlendingCreateInfo.logicOpEnable = VK_FALSE;				// 計算の代わりに論理演算を使用するかどうか
	colourBlendingCreateInfo.logicOp;
	colourBlendingCreateInfo.attachmentCount = 1;
	colourBlendingCreateInfo.pAttachments = &colorBlendAttachment;
	colourBlendingCreateInfo.blendConstants;


	//
	// ここにあったパイプラインレイアウトを作る処理は別の場所に記載
	//



	// -- 深度ステンシルテスト --
	// TODO: 深度ステンシルテストの設定


	// -- グラフィックスパイプラインの作成 --
	vk::GraphicsPipelineCreateInfo pipelineCreateInfo;

	pipelineCreateInfo.pNext;
	pipelineCreateInfo.flags;
	pipelineCreateInfo.stageCount = 2;// シェーダーステージの数
	pipelineCreateInfo.pStages = shaderStages;						// シェーダーステージのリスト
	pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;	// すべての固定機能パイプラインステート
	pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	pipelineCreateInfo.pTessellationState;
	pipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	pipelineCreateInfo.pRasterizationState = &rasterizerCreateInfo;
	pipelineCreateInfo.pMultisampleState = &multisamplingCreateInfo;
	pipelineCreateInfo.pDepthStencilState = nullptr;
	pipelineCreateInfo.pColorBlendState = &colourBlendingCreateInfo;
	pipelineCreateInfo.pDynamicState = nullptr;
	pipelineCreateInfo.layout = pipelineLayout;// パイプラインが使用するパイプラインレイアウト
	pipelineCreateInfo.renderPass = renderPass;// パイプラインが互換性のあるレンダーパスの説明
	pipelineCreateInfo.subpass = 0;// パイプラインで使用するサブパス

	// パイプラインの派生: 最適化のために相互に派生する複数のパイプラインを作成できる
	pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;// 派生元の既存のパイプライン
	pipelineCreateInfo.basePipelineIndex = -1;// または作成中のパイプラインのインデックス (複数作成する場合)

	// グラフィックスパイプラインを作成
	vk::UniquePipeline graphicsPipeline = logicalDevice.createGraphicsPipelineUnique(VK_NULL_HANDLE, pipelineCreateInfo).value;
	if (!graphicsPipeline) {
		throw std::runtime_error("グラフィックスパイプラインの作成に失敗しました！");
	}

	//パイプラインの作成後に不要になったシェーダーモジュールを破棄
	//vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);

	return graphicsPipeline;

}

vk::UniquePipelineLayout GraphicsPipelineUtility::createPipelineLayout(vk::Device logicalDevice)
{
	// -- パイプラインレイアウト (TODO: 将来のディスクリプタセットレイアウトを適用する) --
	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setLayoutCount = 0;
	pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;


	vk::UniquePipelineLayout pipelineLauout = logicalDevice.createPipelineLayoutUnique(pipelineLayoutCreateInfo);

	// エラーの場合メッセージを投げる
	if (!pipelineLauout)
	{
		throw std::runtime_error("パイプラインレイアウトの作成に失敗しました！");
	}

	return pipelineLauout;
}
