#include "PipelineShaderCreator.h"




VModelShaderConfiguer::VModelShaderConfiguer():
	VShaderConfigureBase(),
	m_TextureLauout(),
	m_CameraDescriptor(),
	m_BindingDescriptions(),
	m_AttributeDescriptions()
{
}

VModelShaderConfiguer::~VModelShaderConfiguer()
{
}

void VModelShaderConfiguer::Create(vk::Device* pLogicalDevice)
{
	m_pLogicalDevice = pLogicalDevice;

	m_CameraDescriptor.Initialize(m_pLogicalDevice, 0);
	m_TextureLauout.Initialize(m_pLogicalDevice, 0);

	// シェーダーの読込とモジュールの作成
	VShaderConfigureBase::CreateShaderModules(
		pLogicalDevice, 
		DefaultShaderDefine.VertexShaderPath,
		DefaultShaderDefine.FragmentShaderPath,
		DefaultShaderDefine.EntryName,
		DefaultShaderDefine.EntryName);

	// バインディングの定義
	m_BindingDescriptions =
	{
		vk::VertexInputBindingDescription
		{
		   0,                              // binding  
		   sizeof(Vertex),                 // stride   
		   vk::VertexInputRate::eVertex    // inputRate
		}
	};

	// 入力属性の定義
	m_AttributeDescriptions = {
		// 座標
	  vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },

	  // 法線
	 vk::VertexInputAttributeDescription{ 1, 0, vk::Format::eR32G32B32Sfloat,  offsetof(Vertex, normal) },

	 // テクスチャ座標
	 vk::VertexInputAttributeDescription{ 2, 0, vk::Format::eR32G32Sfloat,     offsetof(Vertex, textureCoord) }
	};
}

std::vector<vk::DescriptorSet> VModelShaderConfiguer::GetDescriptorSets()
{
	// ディスクリプタセットをバインド
	std::vector<vk::DescriptorSet> descriptorSets;
	descriptorSets.push_back(m_CameraDescriptor.GetDescriptorSet());
	descriptorSets.push_back(m_TextureLauout.GetDescriptorSet());

	return descriptorSets;
}

std::vector<vk::DescriptorSetLayout> VModelShaderConfiguer::GetDescriptorSetLayouts()
{
	// ディスクリプタセットレイアウトをバインド
	std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
	descriptorSetLayouts.push_back(m_CameraDescriptor.GetDescriptorSetLayout());
	descriptorSetLayouts.push_back(m_TextureLauout.GetDescriptorSetLayout());

	return descriptorSetLayouts;
}

std::vector<vk::PushConstantRange> VModelShaderConfiguer::GetPushConstantRanges()
{
	return { GetPushConstantModelRange() };
}

vk::PushConstantRange VModelShaderConfiguer::GetPushConstantModelRange()
{
	return vk::PushConstantRange
	{
		vk::ShaderStageFlagBits::eVertex,	// 渡したいシェーダーステージ
		0,								    // 渡したデータからどの位置のデータを見るか
		sizeof(Transform)					// 渡したいデータのサイズ
	};
}

vk::PipelineVertexInputStateCreateInfo VModelShaderConfiguer::GetVertexInputState()
{
	// パイプライン頂点入力状態の作成
	vk::PipelineVertexInputStateCreateInfo resultInfo;
	resultInfo.pVertexBindingDescriptions = m_BindingDescriptions.data();
	resultInfo.vertexBindingDescriptionCount = m_BindingDescriptions.size();
	resultInfo.pVertexAttributeDescriptions = m_AttributeDescriptions.data();
	resultInfo.vertexAttributeDescriptionCount = m_AttributeDescriptions.size();

	return resultInfo;

}

vk::DescriptorSetLayout VModelShaderConfiguer::GetTextureDescriptorLayout()
{
	return m_TextureLauout.GetDescriptorSetLayout();
}

