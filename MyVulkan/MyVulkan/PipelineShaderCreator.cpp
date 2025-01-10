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
	// ���W���[���̓p�C�v���C���쐬��͕s�v�ɂȂ�
	DestroyModule();
}

void PipelineShaderCreator::Create(vk::Device* logicalDevice, std::string sprvVertexShaderPath, std::string sprvFragmentShaderPath)
{
	// SPIR-V �V�F�[�_�[�R�[�h�̓Ǎ�
	auto vertexShaderCode = readFile(sprvVertexShaderPath);
	auto fragmentShaderCode = readFile(sprvFragmentShaderPath);

	// �V�F�[�_�[���W���[���̍쐬
	m_VertexShaderModule	= CreateShaderModule(*logicalDevice, vertexShaderCode);
	m_FragmentShaderModule	= CreateShaderModule(*logicalDevice, fragmentShaderCode);

}

// 
std::vector<vk::PipelineShaderStageCreateInfo>* PipelineShaderCreator::GetPShaderStages()
{
	// ��Ŏ��R�ɃX�e�[�W�ǉ��o����悤�ɂ��邩������
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
	// ���W���[���폜�֐�
	if (m_FragmentShaderModule)	m_pLogicalDevice->destroyShaderModule(m_FragmentShaderModule);
	if(m_VertexShaderModule)	m_pLogicalDevice->destroyShaderModule(m_VertexShaderModule);
}
