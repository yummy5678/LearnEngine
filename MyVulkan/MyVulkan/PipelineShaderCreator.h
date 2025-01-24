#pragma once
#include "VShaderConfigureBase.h"
#include "GraphicsDefine.h"
#include "MeshObject.h"
#include "VTextureDescriptor.h"
#include "VCameraDescriptor.h"

// シェーダーの読み込みとモジュール、ステージの作成を行うクラス
// 一緒にデスクリプタの管理も行う 

class VModelShaderConfiguer : public VShaderConfigureBase
{
public:
	VModelShaderConfiguer();
	~VModelShaderConfiguer();


	// 作成関数(テッセーションシェーダーは未対応)
	void Create(vk::Device* pLogicalDevice);

	
	//std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();
	std::vector<vk::DescriptorSet>			GetDescriptorSets()			override;
	std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts()	override;
	std::vector<vk::PushConstantRange>		GetPushConstantRanges()		override;
	vk::PipelineVertexInputStateCreateInfo	GetVertexInputState()		override;

protected:

	// デスクリプタ
	VTextureDescriptor		m_TextureDescriptor;
	VCameraDescriptor		m_CameraDescriptor;

	// 頂点入力情報	
	std::vector<vk::VertexInputBindingDescription>		m_BindingDescriptions;
	std::vector<vk::VertexInputAttributeDescription>	m_AttributeDescriptions;

	//vk::ShaderModule CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code);
	//vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type);

	vk::PushConstantRange	GetPushConstantModelRange();

};

