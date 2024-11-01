#include "PipelineShaderCreator.h"


PipelineShaderCreator::PipelineShaderCreator()
{
}

PipelineShaderCreator::~PipelineShaderCreator()
{
	// モジュールはパイプライン作成後は不要になる
	DestroyModule();
}

void PipelineShaderCreator::Create(vk::Device logicalDevice, std::string vertexShaderPath, std::string flagmentShaderPath)
{
	// SPIR-V シェーダーコードの読込
	auto vertexShaderCode = readFile("Shaders/vert.spv");
	auto fragmentShaderCode = readFile("Shaders/frag.spv");

	// シェーダーモジュールの作成
	m_VertexShaderModule	= CreateShaderModule(logicalDevice, vertexShaderCode);
	m_FragmentShaderModule	= CreateShaderModule(logicalDevice, fragmentShaderCode);



}

// 
std::vector<vk::PipelineShaderStageCreateInfo> PipelineShaderCreator::GetShaderStages()
{
	// 後で自由にステージ追加出来るようにするか検討中
	std::vector<vk::PipelineShaderStageCreateInfo> results;
	if (m_VertexShaderModule) results.push_back(CreateShaderStage(m_VertexShaderModule, vk::ShaderStageFlagBits::eVertex));
	if (m_FragmentShaderModule) results.push_back(CreateShaderStage(m_FragmentShaderModule, vk::ShaderStageFlagBits::eFragment));

	return results;
}

vk::ShaderModule PipelineShaderCreator::CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code)
{
	// Shader Module creation information
	vk:: ShaderModuleCreateInfo shaderModuleCreateInfo;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	try { return logicalDevice.createShaderModule(shaderModuleCreateInfo); }
	catch(const std::exception&){ throw std::runtime_error("シェーダーモジュールの作成に失敗しました!"); }
}

vk::PipelineShaderStageCreateInfo PipelineShaderCreator::CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type)
{
	vk::PipelineShaderStageCreateInfo resultStage;
	resultStage.setModule(module);
	resultStage.setStage(type);
	resultStage.setPName("main");
	return resultStage;
}

void PipelineShaderCreator::DestroyModule()
{
	// モジュール削除関数
	if (m_FragmentShaderModule)	m_LogicalDevice.destroyShaderModule(m_FragmentShaderModule);
	if(m_VertexShaderModule)	m_LogicalDevice.destroyShaderModule(m_VertexShaderModule);
}
