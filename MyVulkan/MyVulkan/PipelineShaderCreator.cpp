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

	// �V�F�[�_�[�̓Ǎ��ƃ��W���[���̍쐬
	VShaderConfigureBase::CreateShaderModules(
		pLogicalDevice, 
		DefaultShaderDefine.VertexShaderPath,
		DefaultShaderDefine.FragmentShaderPath,
		DefaultShaderDefine.EntryName,
		DefaultShaderDefine.EntryName);

	// �o�C���f�B���O�̒�`
	m_BindingDescriptions =
	{
		vk::VertexInputBindingDescription
		{
		   0,                              // binding  
		   sizeof(Vertex),                 // stride   
		   vk::VertexInputRate::eVertex    // inputRate
		}
	};

	// ���͑����̒�`
	m_AttributeDescriptions = {
		// ���W
	  vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },

	  // �@��
	 vk::VertexInputAttributeDescription{ 1, 0, vk::Format::eR32G32B32Sfloat,  offsetof(Vertex, normal) },

	 // �e�N�X�`�����W
	 vk::VertexInputAttributeDescription{ 2, 0, vk::Format::eR32G32Sfloat,     offsetof(Vertex, textureCoord) }
	};
}

std::vector<vk::DescriptorSet> VModelShaderConfiguer::GetDescriptorSets()
{
	// �f�B�X�N���v�^�Z�b�g���o�C���h
	std::vector<vk::DescriptorSet> descriptorSets;
	descriptorSets.push_back(m_CameraDescriptor.GetDescriptorSet());
	descriptorSets.push_back(m_TextureLauout.GetDescriptorSet());

	return descriptorSets;
}

std::vector<vk::DescriptorSetLayout> VModelShaderConfiguer::GetDescriptorSetLayouts()
{
	// �f�B�X�N���v�^�Z�b�g���C�A�E�g���o�C���h
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
		vk::ShaderStageFlagBits::eVertex,	// �n�������V�F�[�_�[�X�e�[�W
		0,								    // �n�����f�[�^����ǂ̈ʒu�̃f�[�^�����邩
		sizeof(Transform)					// �n�������f�[�^�̃T�C�Y
	};
}

vk::PipelineVertexInputStateCreateInfo VModelShaderConfiguer::GetVertexInputState()
{
	// �p�C�v���C�����_���͏�Ԃ̍쐬
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

