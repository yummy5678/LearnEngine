#include "RenderingPipelineCreator.h"


RenderingPipelineCreator::RenderingPipelineCreator(VulkanInitializer& initializer) :
	m_pLogicalDevice(nullptr),
	m_Pipeline(VK_NULL_HANDLE),
	m_PipelineInfo(),
	m_PipelineLayout(VK_NULL_HANDLE),
	m_PipelineLayoutInfo(),
	m_TextureDescriptor()
{
	m_ClassName = "PipelineGenerator";

	if (initializer.IsInitialized() == true) 
		printf("イニシャライザーをイニシャライズする前にパイプラインのコンストラクタを読んでください");
	initializer.GetPDeviceExtension()->UseDynamicRendering();
	m_pLogicalDevice = initializer.GetPLogicalDevice();

}

RenderingPipelineCreator::~RenderingPipelineCreator()
{
}

void RenderingPipelineCreator::Create(
	vk::Device* pLogicalDevice,
	vk::Extent2D extent,
	vk::Rect2D scissor,
	vk::Format colorFormat,
	vk::Format depthFormat,
	vk::PipelineVertexInputStateCreateInfo* pVertexInputState,
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos,
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts,
	std::vector<vk::PushConstantRange> pushConstantRanges)
{
	m_bCreated = true;

	//m_TextureDescriptor.CreateSingleDescriptorSet();
	//パイプラインレイアウトの作成	//今は作らなくていいかも
	//std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = { m_TextureDescriptor.GetDescriptorSetLayout() };
	CreatePipelineLayout(pLogicalDevice, descriptorSetLayouts, pushConstantRanges);

	//パイプラインの作成
	CreateGraphicsPipeline(m_pLogicalDevice, extent, scissor, colorFormat, depthFormat, shaderStageInfos, pVertexInputState);
}

void RenderingPipelineCreator::Destroy()
{
	if (m_pLogicalDevice == nullptr) return;

	m_pLogicalDevice->destroyPipelineLayout(m_PipelineLayout);
	m_pLogicalDevice->destroyPipeline(m_Pipeline);

	//パイプラインの作成後に不要になったシェーダーモジュールを破棄
	//vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
	//vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);
}

vk::Pipeline RenderingPipelineCreator::GetPipeline()
{
	CheckCreated();
	return m_Pipeline;
}

vk::PipelineLayout RenderingPipelineCreator::GetPipelineLayout()
{
	CheckCreated();
	return m_PipelineLayout;
}

void RenderingPipelineCreator::CreatePipelineLayout(vk::Device* pLogicalDevice, std::vector<vk::DescriptorSetLayout> descriptorSetLayouts, std::vector<vk::PushConstantRange> pushConstantRanges)
{
	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setSetLayouts(descriptorSetLayouts);
	pipelineLayoutCreateInfo.setPushConstantRanges(pushConstantRanges);

	m_PipelineLayout = pLogicalDevice->createPipelineLayout(pipelineLayoutCreateInfo); 
}

void RenderingPipelineCreator::CreateGraphicsPipeline(vk::Device* pLogicalDevice, vk::Extent2D extent, vk::Rect2D scissor, vk::Format colorFormat, vk::Format depthFormat, std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos, vk::PipelineVertexInputStateCreateInfo* pVertexInputState)
{
	if (!m_PipelineLayout)	throw std::runtime_error("グラフィクスパイプラインの作成前にパイプラインレイアウトを作成してください!");

	// ビューポートの設定情報を作成
#pragma region viewport
	vk::Viewport viewport;
	viewport.setX(0.0f);									// x座標の開始位置
	viewport.setY(0.0f);									// y座標の開始位置
	viewport.setWidth((float)extent.width);					// ビューポートの幅
	viewport.setHeight((float)extent.height);				// ビューポートの高さ
	viewport.setMinDepth(0.0f);								// フレームバッファの最小深度
	viewport.setMaxDepth(1.0f);								// フレームバッファの最大深度

	// ビューポートとシザーの状態を設定するための構造体を作成
	vk::PipelineViewportStateCreateInfo viewportStateInfo;
	viewportStateInfo.setViewportCount(1);		// ビューポートの数
	viewportStateInfo.setPViewports(&viewport);	// ビューポートのポインタ
	viewportStateInfo.setScissorCount(1);			// シザーの数
	viewportStateInfo.setPScissors(&scissor);		// シザーのポインタ
#pragma endregion viewport

	// ラスタライザーの設定
#pragma region rasterizationInfo
	/* //////////////////////////////////////////////////////
		頂点データをピクセルのデータに変換するときの処理設定
	*/ //////////////////////////////////////////////////////
	vk::PipelineRasterizationStateCreateInfo rasterizationInfo;
	rasterizationInfo.pNext = nullptr;							// 拡張用ポインタ
	rasterizationInfo.flags;									// 予約済みフィールド
	rasterizationInfo.cullMode = vk::CullModeFlagBits::eBack;	// 背面カリング
	rasterizationInfo.depthBiasClamp = 0.0f;					// 深度バイアスのクランプ値
	rasterizationInfo.depthBiasConstantFactor = 0.0f;			// 深度バイアスの定数係数
	rasterizationInfo.depthBiasEnable = VK_FALSE;				// 深度バイアスの有無
	rasterizationInfo.depthBiasSlopeFactor = 0.0f;				// 深度バイアスのスロープ係数
	rasterizationInfo.depthClampEnable = VK_FALSE;				// 深度クランピングの有無
	rasterizationInfo.lineWidth = 1.0f;							// 線の幅
	rasterizationInfo.polygonMode = vk::PolygonMode::eFill;		// ポリゴンの描画モード
	rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;		// ラスタライザーの処理の有無
	rasterizationInfo.frontFace = vk::FrontFace::eCounterClockwise;	// ポリゴンの前面の定義
#pragma endregion rasterizationInfo

	// マルチサンプリングの設定
#pragma region multisamplingInfo
	vk::PipelineMultisampleStateCreateInfo multisamplingInfo;
	multisamplingInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);	// マルチサンプリングを指定
	multisamplingInfo.setSampleShadingEnable(VK_FALSE);		// サンプルシェーディングを無効
	multisamplingInfo.setMinSampleShading(1.0f);				// サンプルシェーディングの最小比率
	multisamplingInfo.setPSampleMask(nullptr);				// サンプルマスクを指定（通常はnullptr）
	multisamplingInfo.setAlphaToCoverageEnable(VK_FALSE);		// アルファカバレッジを無効
	multisamplingInfo.setAlphaToOneEnable(VK_FALSE);			// アルファ値を1に固定を無効
#pragma endregion multisamplingInfo

	// カラーブレンディングの設定
#pragma region colorBlendingInfo
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
	vk::PipelineColorBlendStateCreateInfo colorBlendingInfo;	// ブレンド状態の初期化
	colorBlendingInfo.logicOpEnable = VK_FALSE;				// 論理演算を無効にする
	colorBlendingInfo.attachmentCount = 1;					// アタッチメントの数を1に設定
	colorBlendingInfo.pAttachments = &colorState;				// 設定したブレンド状態を指定
#pragma endregion colorBlendingInfo

	// -- DEPTH STENCIL TESTING --
#pragma region depthStencilInfo
	auto depthStencilInfo = CreateDepthStencilStateInfo(true, false);
#pragma endregion depthStencilInfo

	// ダイナミックレンダリングの設定
#pragma region pipelineRenderingInfo
	// 4. Dynamic Rendering用のPipelineRenderingCreateInfoの設定
	vk::PipelineRenderingCreateInfo pipelineRenderingInfo;
	pipelineRenderingInfo.colorAttachmentCount = 1;
	pipelineRenderingInfo.pColorAttachmentFormats = &colorFormat;  // カラーアタッチメントのフォーマット
	pipelineRenderingInfo.depthAttachmentFormat = depthFormat;     // 深度アタッチメントのフォーマット
#pragma endregion pipelineRenderingInfo


	// 入力アセンブリステートの設定
#pragma region assemblyStateInfo
	vk::PipelineInputAssemblyStateCreateInfo assemblyStateInfo;
	assemblyStateInfo.setPNext(nullptr);
	//assemblyStateInfo.setFlags((vk::PipelineInputAssemblyStateCreateFlags)0);	// 初期化のため0を入れる
	assemblyStateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);   // トポロジー(三角形リスト)
	assemblyStateInfo.primitiveRestartEnable = VK_FALSE;                 // プリミティブ再開を無効にする
#pragma endregion assemblyStateInfo

	m_PipelineInfo.setStages(shaderStageInfos);						// シェーダーステージ
	m_PipelineInfo.setPVertexInputState(pVertexInputState);	// All the fixed function pipeline states
	m_PipelineInfo.setPInputAssemblyState(&assemblyStateInfo);
	m_PipelineInfo.setPViewportState(&viewportStateInfo);
	m_PipelineInfo.setPDynamicState(nullptr);						//ダイナミックステートとは:パイプラインを作り直さなくても一部情報を変更できる機能
	m_PipelineInfo.setPRasterizationState(&rasterizationInfo);
	m_PipelineInfo.setPMultisampleState(&multisamplingInfo);
	m_PipelineInfo.setPColorBlendState(&colorBlendingInfo);
	m_PipelineInfo.setPDepthStencilState(&depthStencilInfo);
	m_PipelineInfo.layout = m_PipelineLayout;
	m_PipelineInfo.pNext = &pipelineRenderingInfo;					// ダイナミックレンダリングを使用するための値


	// Create Graphics Pipeline
	auto result = m_pLogicalDevice->createGraphicsPipeline(nullptr, m_PipelineInfo);

	m_Pipeline = result.value;
}

vk::PipelineDepthStencilStateCreateInfo RenderingPipelineCreator::CreateDepthStencilStateInfo(bool depth, bool stencil)
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

// 内容は後で検証する
vk::Format RenderingPipelineCreator::FindSupportedDepthFormat(vk::PhysicalDevice physicalDevice)
{
	vk::Format depthFormats[] = { vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint, vk::Format::eD16Unorm };
	for (auto format : depthFormats) {
		vk::FormatProperties props = physicalDevice.getFormatProperties(format);
		if (props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
			return format;
		}
	}
	throw std::runtime_error("Failed to find a supported depth format!");
}

vk::PipelineInputAssemblyStateCreateInfo& RenderingPipelineCreator::GetInputAssemblyInfo()
{
	vk::PipelineInputAssemblyStateCreateInfo assemblyStateInfo;
	assemblyStateInfo.setPNext(nullptr);
	//assemblyStateInfo.setFlags((vk::PipelineInputAssemblyStateCreateFlags)0);	// 初期化のため0を入れる
	assemblyStateInfo.setTopology(vk::PrimitiveTopology::eTriangleList);   // トポロジー(三角形リスト)
	assemblyStateInfo.primitiveRestartEnable = VK_FALSE;                 // プリミティブ再開を無効にする
	return assemblyStateInfo;
}

//vk::PushConstantRange RenderingPipelineCreator::GetPushConstantModelRange()
//{
//	return vk::PushConstantRange
//	{
//		vk::ShaderStageFlagBits::eVertex,	// 渡したいシェーダーステージ
//		0,								    // 渡したデータからどの位置のデータを見るか
//		sizeof(Transform)					// 渡したいデータのサイズ
//	};
//}















