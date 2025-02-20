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
	// 後で自由にステージ追加出来るようにするか検討中
	std::vector<vk::PipelineShaderStageCreateInfo>  results;
	if (m_VertexShaderModule != VK_NULL_HANDLE)		results.push_back(CreateShaderStage(m_VertexShaderModule,   vk::ShaderStageFlagBits::eVertex,	m_VertexEntryName.c_str()));
	if (m_FragmentShaderModule != VK_NULL_HANDLE)	results.push_back(CreateShaderStage(m_FragmentShaderModule, vk::ShaderStageFlagBits::eFragment, m_FragmentEntryName.c_str()));

	return results;
}

void VShaderConfigureBase::DestroyModule()
{
	//if (m_LogicalDevice == nullptr || *m_LogicalDevice == VK_NULL_HANDLE) return;

	// モジュール削除関数
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

	// SPIR-V シェーダーコードの読込
	auto vertexShaderCode	= readFile(m_VertexShaderPath);
	auto fragmentShaderCode = readFile(m_FragmentShaderPath);

	// シェーダーモジュールの作成
	m_VertexShaderModule	= CreateShaderModule(vertexShaderCode);
	m_FragmentShaderModule	= CreateShaderModule(fragmentShaderCode);

	// エントリポイントの名前を記憶
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
	resultStage.module = module;		// シェーダーステージ
	resultStage.stage = stageType;		// 使用するシェーダーモジュール
	resultStage.pName = entryName;		// シェーダーのエントリポイント名
	return resultStage;
}
