#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"

// シェーダーの読み込みとモジュール、ステージの作成を行うクラス
// シェーダーの作成個数に制限はないがパイプライン内において
// シェーダーを通す順番はVulkanAPIの中で自動で整理されるのだろうか？
// 

class PipelineShaderCreator
{
public:
	PipelineShaderCreator();
	~PipelineShaderCreator();


	// 作成関数(テッセーションシェーダーは未対応)
	void Create(vk::Device logicalDevice, std::string vertexShaderPath, std::string flagmentShaderPath);

	
	std::vector<vk::PipelineShaderStageCreateInfo> GetShaderStages();


private:
	vk::Device					m_LogicalDevice;

	// シェーダーモジュール
	vk::ShaderModule			m_VertexShaderModule;
	vk::ShaderModule			m_FragmentShaderModule;



	// 作成情報
	vk::PipelineShaderStageCreateInfo m_VertexStageInfo;
	vk::PipelineShaderStageCreateInfo m_FragmentStageInfo;

	vk::ShaderModule CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code);
	vk::PipelineShaderStageCreateInfo CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type);

	void DestroyModule();



};

