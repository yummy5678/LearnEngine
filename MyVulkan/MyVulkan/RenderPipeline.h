#pragma once
#include "MeshObject.h"
#include "PipelineShaderCreator.h"
#include "GraphicsPipelineUtility.h"

// シーンクラスからVulkanのグラフィックパイプラインへの
// 描画モデルの情報の橋渡しを行うクラス

// シェーダーや描画時の細かい設定も
// ここで決められるようにしたい。

// まだ仮クラス
class RenderPipeline
{
public:
	RenderPipeline();
	~RenderPipeline();

	void Initialize();
	void updateModel(MeshObject* mesh, glm::mat4 transform);
	void Destroy();

private:
	// 画面サイズ
	vk::Extent2D			m_Extent;

	//パイプラインに渡すシェーダー情報の作成クラス
	PipelineShaderCreator	m_Shader;	


	// デスクリプタ





};

