#pragma once
#include "MeshObject.h"
#include "RenderObject.h"
#include "PipelineShaderCreator.h"
#include "RenderingPipelineCreator.h"
#include "VulkanInitializer.h"

// シーンクラスからVulkanのグラフィックパイプラインへの
// 描画モデルの情報の橋渡しを行うクラス

// シェーダーや描画時の細かい設定も
// ここで決められるようにしたい。

// まだ仮クラス
class RenderConfig
{
public:
	RenderConfig(VulkanInitializer& initializer);
	~RenderConfig();

	void Initialize(
		vk::Device logicalDevice,
		vk::Extent2D extent,
		vk::Format colorFomat,
		vk::Format depthFomat);

	void Destroy();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();
	std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();


private:
	// 画面サイズ
	vk::Offset2D			m_Offset;	//描画範囲の始点
	vk::Extent2D			m_Extent;	//描画範囲の終点

	vk::Rect2D				m_RenderArea;	// フレーム内の描画位置の矩形

	RenderingPipelineCreator m_GraphicsPipeline;

	//パイプラインに渡すシェーダー情報の作成クラス
	PipelineShaderCreator	m_Shader;
};
