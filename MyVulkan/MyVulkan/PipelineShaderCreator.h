#pragma once
#include "VShaderConfigureBase.h"
#include "GraphicsDefine.h"
#include "MeshObject.h"
#include "VTextureDescriptorLayout.h"
#include "VCameraDescriptorSetLayout.h"




// シェーダーの読み込みとモジュール、ステージの作成を行うクラス
// 一緒にデスクリプタの管理も行う 

class VModelShaderConfiguer : public VShaderConfigureBase
{
public:
	VModelShaderConfiguer();
	~VModelShaderConfiguer();


	// 作成関数(テッセーションシェーダーは未対応)
	void Create(vk::Device pLogicalDevice);

	
	//std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();
	//std::vector<vk::DescriptorSet>			GetDescriptorSets()			override;
	std::vector<vk::DescriptorSetLayout>	GetDescriptorSetLayouts()	override;
	std::vector<vk::PushConstantRange>		GetPushConstantRanges()		override;
	vk::PipelineVertexInputStateCreateInfo	GetVertexInputState()		override;

	std::shared_ptr<vk::DescriptorSetLayout>GetTextureDescriptorLayout();
	std::shared_ptr<vk::DescriptorSetLayout>GetCameraDescriptorLayout();

protected:

	// デスクリプタ
	VTextureDescriptorSetLayout							m_TextureLayout;
	VCameraDescriptorSetLayout							m_CameraDescriptor;

	// 頂点入力情報	
	std::vector<vk::VertexInputBindingDescription>		m_BindingDescriptions;
	std::vector<vk::VertexInputAttributeDescription>	m_AttributeDescriptions;

	//vk::ShaderModule CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code);
	//vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type);

	vk::PushConstantRange	GetPushConstantModelRange();

};

