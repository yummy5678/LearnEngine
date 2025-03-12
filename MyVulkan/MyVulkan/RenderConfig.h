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
	//void Initialize(
	//	vk::Device logicalDevice,
	//	vk::Extent2D extent,
	//	vk::Format colorFomat,
	//	vk::Format depthFomat);
	void Initialize(RenderTarget* renderere);

	// ディスクリプタなどを更新したい
	void Update();

	// 破棄
	void Destroy();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline()			override;
	vk::PipelineLayout	GetPipelineLayout()		override;
	//VShaderConfigureBase* GetPShaderConfiguer() override;

	//std::vector<vk::DescriptorSet>			GetDescriptorSets();
	//std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts();
	//std::vector<vk::PushConstantRange>		GetPushConstantRanges();

	//void DrawImage(std::vector<RenderObject>* objects, SceneCamera* camera);
	std::shared_ptr<RenderFunction> GetRenderFunction(RenderingObjects* pObjects, SceneCamera* pCamera);
	//oid BindRenderingCommand(vk::CommandBuffer command, std::vector<RenderObject*>* pObjects, SceneCamera* pCamera);

private:
	//vk::Device* m_pLogicalDevice;
	//vk::PhysicalDevice* m_pPhygicalDevice;

	//// 画面サイズ
	//vk::Offset2D			m_Offset;	//描画範囲の始点
	//vk::Extent2D			m_Extent;	//描画範囲の終点

	//vk::Rect2D				m_RenderArea;	// フレーム内の描画位置の矩形

	//RenderingPipelineCreator m_GraphicsPipeline;

	//パイプラインに渡すシェーダー情報の作成クラス
	VModelShaderConfiguer	m_Shader;

	std::vector<std::weak_ptr<RenderFunction>>		m_RenderFunction;
	std::unordered_map<void*, RenderingObjects*>	m_pObjects;
	std::unordered_map<void*, SceneCamera*>			m_pCamera;

	//DescriptorHandleManeger<VMaterial> m_DescriptorHandleManeger;
	//std::vector<std::pair<std::weak_ptr<VMaterial*>, VSingleTextureDescriptor>>	m_RenderObjects;

	//std::pair<std::weak_ptr<VMaterial*>, VSingleTextureDescriptor>* SearchTextureDeskriptor(VMaterial* target);

	//// デスクリプタ
	//VTextureDescriptorSetLayout		m_TextureDescriptors;
	//VCameraDescriptorSetLayout		m_CameraDescriptor;

	//// 頂点入力情報	//ToDo:後で専用のクラスに分ける
	//std::vector<vk::VertexInputBindingDescription>		m_BindingDescriptions;
	//std::vector<vk::VertexInputAttributeDescription>	m_AttributeDescriptions;
	//vk::PipelineVertexInputStateCreateInfo				m_InputStateInfo;


	//vk::PushConstantRange	GetPushConstantModelRange();
	//vk::PipelineVertexInputStateCreateInfo GetVertexInputState();
	//void CreateTextureDescriptor(VTextureBuffer* pMaterial);
	//void UpdateTextureDescriptor();


};
