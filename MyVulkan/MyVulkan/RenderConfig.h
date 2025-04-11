#pragma once
#include "MeshObject.h"
#include "RenderObject.h"
#include "PipelineShaderCreator.h"
#include "RenderingPipelineCreator.h"
#include "VulkanInitializer.h"
#include "RenderTargetBase.h"
#include "SceneCamera.h"
#include "GraphicWindow.h"
#include "RenderFunction.h"


// シーンクラスからVulkanのグラフィックパイプラインへの
// 描画モデルの情報の橋渡しを行うクラス

// シェーダーや描画時の細かい設定も
// ここで決められるようにしたい。

using RenderingObjects = std::vector<RenderObject*>;

// まだ仮クラス
class RenderConfig : RendererBase
{
public:
	RenderConfig(VulkanInitializer& initializer);
	~RenderConfig();

	// 初期化
	void Initialize(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, RenderingTarget* renderere);

	// ディスクリプタなどを更新したい
	void Update();

	// メンバ変数の初期化
	void Cleanup();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline()			override;
	vk::PipelineLayout	GetPipelineLayout()		override;

	
	std::shared_ptr<RenderFunction> GetRenderFunction(RenderingObjects* pObjects, SceneCamera* pCamera);

private:
	//vk::Device* m_LogicalDevice;
	//vk::PhysicalDevice* m_PhygicalDevice;

	VModelShaderConfiguer	m_Shader;

	std::vector<std::weak_ptr<RenderFunction>>		m_RenderFunction;
	std::unordered_map<void*, RenderingObjects*>	m_pObjects;
	std::unordered_map<void*, SceneCamera*>			m_pCamera;


};
