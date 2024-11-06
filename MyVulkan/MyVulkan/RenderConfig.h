#pragma once
#include "MeshObject.h"
#include "PipelineShaderCreator.h"
#include "GraphicsPipelineCreator.h"

// シーンクラスからVulkanのグラフィックパイプラインへの
// 描画モデルの情報の橋渡しを行うクラス

// シェーダーや描画時の細かい設定も
// ここで決められるようにしたい。

// まだ仮クラス
class RenderConfig
{
public:
	RenderConfig();
	~RenderConfig();

	void Initialize();

	void Destroy();

	vk::Rect2D GetExtent2D();
	vk::Pipeline GetPipeline();
	std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();
	Scene* GetPScene();


private:
	// 画面サイズ
	vk::Offset2D			m_Offset;	//描画範囲の始点
	vk::Extent2D			m_Extent;	//描画範囲の終点

	RenderingPipelineCreator	m_GraphicsPipeline;

	//パイプラインに渡すシェーダー情報の作成クラス
	PipelineShaderCreator	m_Shader;	


	Scene* m_pRenderScene;




};

