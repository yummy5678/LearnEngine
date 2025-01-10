#include "PipelineShaderCreator.h"



PipelineShaderCreator::PipelineShaderCreator():
	m_pLogicalDevice(nullptr),
	m_FragmentStageInfo(),
	m_FragmentShaderModule(VK_NULL_HANDLE),
	m_VertexStageInfo(),
	m_VertexShaderModule(VK_NULL_HANDLE)
{
}

PipelineShaderCreator::~PipelineShaderCreator()
{
	// モジュールはパイプライン作成後は不要になる
	DestroyModule();
}

void PipelineShaderCreator::Create(vk::Device* logicalDevice, std::string sprvVertexShaderPath, std::string sprvFragmentShaderPath)
{
	// SPIR-V シェーダーコードの読込
	auto vertexShaderCode = readFile(sprvVertexShaderPath);
	auto fragmentShaderCode = readFile(sprvFragmentShaderPath);

	// シェーダーモジュールの作成
	m_VertexShaderModule	= CreateShaderModule(*logicalDevice, vertexShaderCode);
	m_FragmentShaderModule	= CreateShaderModule(*logicalDevice, fragmentShaderCode);

}

// 
std::vector<vk::PipelineShaderStageCreateInfo>* PipelineShaderCreator::GetPShaderStages()
{
	// 後で自由にステージ追加出来るようにするか検討中
	std::vector<vk::PipelineShaderStageCreateInfo> results;
	if (m_VertexShaderModule) results.push_back(CreateShaderStage(m_VertexShaderModule, vk::ShaderStageFlagBits::eVertex));
	if (m_FragmentShaderModule) results.push_back(CreateShaderStage(m_FragmentShaderModule, vk::ShaderStageFlagBits::eFragment));

	return &results;
}

vk::ShaderModule PipelineShaderCreator::CreateShaderModule(vk::Device logicalDevice, const std::vector<char>& code)
{
	// Shader Module creation information
	vk:: ShaderModuleCreateInfo shaderModuleCreateInfo;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	return logicalDevice.createShaderModule(shaderModuleCreateInfo);
}

vk::PipelineShaderStageCreateInfo PipelineShaderCreator::CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits type)
{
	vk::PipelineShaderStageCreateInfo resultStage;
	resultStage.setModule(module);
	resultStage.setStage(type);
	resultStage.setPName(DefaultShaderDefine.EntryName);
	return resultStage;
}

void PipelineShaderCreator::DestroyModule()
{
	// モジュール削除関数
	if (m_FragmentShaderModule)	m_pLogicalDevice->destroyShaderModule(m_FragmentShaderModule);
	if(m_VertexShaderModule)	m_pLogicalDevice->destroyShaderModule(m_VertexShaderModule);
}
