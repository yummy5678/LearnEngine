#pragma once
#include "MeshObject.h"
#include "RenderObject.h"
#include "PipelineShaderCreator.h"
#include "RenderingPipelineCreator.h"
#include "VulkanInitializer.h"
#include "RendererBase.h"
#include "SceneCamera.h"
#include "VCameraDescriptor.h"
#include "VSingleDescriptor.h"

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

	// 初期化
	//void Initialize(
	//	vk::Device logicalDevice,
	//	vk::Extent2D extent,
	//	vk::Format colorFomat,
	//	vk::Format depthFomat);
	void Initialize(RendererBase* renderere);

	// ディスクリプタなどを更新したい
	void Update();

	// 破棄
	void Destroy();

	vk::Rect2D			GetSissorRect();
	vk::Rect2D			GetRenderRect();
	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();
	VModelShaderConfiguer* GetPShaderConfiguer();

	//std::vector<vk::DescriptorSet>			GetDescriptorSets();
	//std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts();
	//std::vector<vk::PushConstantRange>		GetPushConstantRanges();

	//void DrawImage(std::vector<RenderObject>* objects, SceneCamera* camera);

	void BindRenderingCommand(vk::CommandBuffer command, std::vector<RenderObject>* pObjects, SceneCamera* pCamera);

private:
	vk::Device* m_pLogicalDevice;
	vk::PhysicalDevice* m_pPhygicalDevice;

	// 画面サイズ
	vk::Offset2D			m_Offset;	//描画範囲の始点
	vk::Extent2D			m_Extent;	//描画範囲の終点

	vk::Rect2D				m_RenderArea;	// フレーム内の描画位置の矩形

	RenderingPipelineCreator m_GraphicsPipeline;

	//パイプラインに渡すシェーダー情報の作成クラス
	VModelShaderConfiguer	m_Shader;

	std::vector<std::weak_ptr<VMaterial*>>				m_RenderObjects;
	std::unordered_map<std::weak_ptr<VMaterial*>, VSingleDescriptor>	m_TextureDescriptors;

	//// デスクリプタ
	//VTextureDescriptorLayout		m_TextureDescriptors;
	//VCameraDescriptor		m_CameraDescriptor;

	//// 頂点入力情報	//ToDo:後で専用のクラスに分ける
	//std::vector<vk::VertexInputBindingDescription>		m_BindingDescriptions;
	//std::vector<vk::VertexInputAttributeDescription>	m_AttributeDescriptions;
	//vk::PipelineVertexInputStateCreateInfo				m_InputStateInfo;


	//vk::PushConstantRange	GetPushConstantModelRange();
	//vk::PipelineVertexInputStateCreateInfo GetVertexInputState();
	void CreateTextureDescriptor(VMaterial* pMaterial);
	void UpdateTextureDescriptor();
};
