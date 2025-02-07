#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"


// シェーダー管理の基底クラス

class VShaderConfigureBase
{
public:
	//コンストラクタ(テッセーションシェーダーは未対応)
	VShaderConfigureBase();
	~VShaderConfigureBase();
	
	virtual std::vector<vk::PipelineShaderStageCreateInfo>	GetShaderStages();
	//virtual std::vector<vk::DescriptorSet>					GetDescriptorSets() = 0;
	virtual std::vector<vk::DescriptorSetLayout>			GetDescriptorSetLayouts() = 0;
	virtual std::vector<vk::PushConstantRange>				GetPushConstantRanges() = 0;
	virtual vk::PipelineVertexInputStateCreateInfo			GetVertexInputState() = 0;

	virtual void DestroyModule();

protected:
	vk::Device* m_pLogicalDevice;

	// シェーダーパス
	std::string m_VertexShaderPath;	// 頂点シェーダーのパス
	std::string m_FragmentShaderPath; // ピクセルシェーダーのパス
	std::string m_VertexEntryName;	// 頂点シェーダーのエントリーポイント名
	std::string m_FragmentEntryName; // ピクセルシェーダーのエントリーポイント名

	// シェーダーモジュール
	vk::ShaderModule			m_VertexShaderModule;
	vk::ShaderModule			m_FragmentShaderModule;

	// 作成情報
	vk::PipelineShaderStageCreateInfo m_VertexStageInfo;
	vk::PipelineShaderStageCreateInfo m_FragmentStageInfo;

	void CreateShaderModules(
		vk::Device* pLogicalDevice, 
		const char* vertexPath, 
		const char* flagmentPath, 
		const char* vertexEntry, 
		const char* fragmentEntry);	// 作成関数
private:
	vk::ShaderModule CreateShaderModule(const std::vector<char>& code);
	vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type, const char* shaderPath);

};

