#include "VShaderConfigureBase.h"

VShaderConfigureBase::VShaderConfigureBase() :
	m_pLogicalDevice(nullptr),
	m_VertexShaderPath(""),
	m_FragmentShaderPath(""),
	m_VertexEntryName(""),
	m_FragmentEntryName(""),
	m_VertexShaderModule(VK_NULL_HANDLE),
	m_FragmentShaderModule(VK_NULL_HANDLE),
	m_VertexStageInfo(),
	m_FragmentStageInfo()
{
}

VShaderConfigureBase::~VShaderConfigureBase()
{
}

std::vector<vk::PipelineShaderStageCreateInfo> VShaderConfigureBase::GetShaderStages()
{
	// ��Ŏ��R�ɃX�e�[�W�ǉ��o����悤�ɂ��邩������
	std::vector<vk::PipelineShaderStageCreateInfo>  results;
	if (m_VertexShaderModule != VK_NULL_HANDLE)		results.push_back(CreateShaderStage(m_VertexShaderModule,   vk::ShaderStageFlagBits::eVertex,	m_VertexEntryName.c_str()));
	if (m_FragmentShaderModule != VK_NULL_HANDLE)	results.push_back(CreateShaderStage(m_FragmentShaderModule, vk::ShaderStageFlagBits::eFragment, m_FragmentEntryName.c_str()));

	return results;
}

void VShaderConfigureBase::DestroyModule()
{
	//if (m_LogicalDevice == nullptr || *m_LogicalDevice == VK_NULL_HANDLE) return;

	// ���W���[���폜�֐�
	if (m_FragmentShaderModule != VK_NULL_HANDLE)	m_pLogicalDevice->destroyShaderModule(m_FragmentShaderModule);
	if (m_FragmentShaderModule != VK_NULL_HANDLE)	m_pLogicalDevice->destroyShaderModule(m_VertexShaderModule);
}

void VShaderConfigureBase::CreateShaderModules(
	vk::Device* pLogicalDevice,
	const char* vertexPath,
	const char* flagmentPath,
	const char* vertexEntry,
	const char* fragmentEntry)
{
	m_pLogicalDevice = pLogicalDevice;
	if (*m_pLogicalDevice == VK_NULL_HANDLE) return;

	m_VertexShaderPath		= vertexPath;
	m_FragmentShaderPath	= flagmentPath;

	// SPIR-V �V�F�[�_�[�R�[�h�̓Ǎ�
	auto vertexShaderCode	= readFile(m_VertexShaderPath);
	auto fragmentShaderCode = readFile(m_FragmentShaderPath);

	// �V�F�[�_�[���W���[���̍쐬
	m_VertexShaderModule	= CreateShaderModule(vertexShaderCode);
	m_FragmentShaderModule	= CreateShaderModule(fragmentShaderCode);

	// �G���g���|�C���g�̖��O���L��
	m_VertexEntryName = vertexEntry;
	m_FragmentEntryName = fragmentEntry;
}

vk::ShaderModule VShaderConfigureBase::CreateShaderModule(const std::vector<char>& code)
{
	vk::ShaderModuleCreateInfo shaderModuleCreateInfo;
	shaderModuleCreateInfo.codeSize = code.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	return m_pLogicalDevice->createShaderModule(shaderModuleCreateInfo);
}

vk::PipelineShaderStageCreateInfo VShaderConfigureBase::CreateShaderStage(vk::ShaderModule module, vk::ShaderStageFlagBits stageType, const char* entryName)
{
	vk::PipelineShaderStageCreateInfo resultStage;
	resultStage.module = module;		// �V�F�[�_�[�X�e�[�W
	resultStage.stage = stageType;		// �g�p����V�F�[�_�[���W���[��
	resultStage.pName = entryName;		// �V�F�[�_�[�̃G���g���|�C���g��
	return resultStage;
}
