#include "VTriangleShader.h"
#include "GraphicsDefine.h"

VTriangleShader::VTriangleShader()
{
}

VTriangleShader::~VTriangleShader()
{
}

void VTriangleShader::Create(vk::Device* pLogicalDevice)
{
	m_pLogicalDevice = pLogicalDevice;

	// �V�F�[�_�[�̓Ǎ��ƃ��W���[���̍쐬
	VShaderConfigureBase::CreateShaderModules(
		pLogicalDevice,
		"Shaders/triangle.vert.spv",
		"Shaders/triangle.frag.spv",
		DefaultShaderDefine.EntryName,
		DefaultShaderDefine.EntryName);
}

std::vector<vk::DescriptorSetLayout> VTriangleShader::GetDescriptorSetLayouts()
{
	return std::vector<vk::DescriptorSetLayout>();	// ���e����
}

std::vector<vk::PushConstantRange> VTriangleShader::GetPushConstantRanges()
{
	return std::vector<vk::PushConstantRange>();	// ���e����
}

vk::PipelineVertexInputStateCreateInfo VTriangleShader::GetVertexInputState()
{
	vk::PipelineVertexInputStateCreateInfo noResult;
	noResult.pNext = nullptr;
	noResult.flags = {};
	noResult.pVertexAttributeDescriptions = nullptr;
	noResult.vertexAttributeDescriptionCount = 0;
	noResult.pVertexBindingDescriptions = nullptr;
	noResult.vertexBindingDescriptionCount = 0;
	return noResult;	// ���e����
}
