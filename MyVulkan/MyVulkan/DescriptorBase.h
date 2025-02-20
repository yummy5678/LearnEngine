#pragma once
#include <vulkan/vulkan.hpp>


class VDescriptorBase
{
public:
	VDescriptorBase(vk::DescriptorType type);
	~VDescriptorBase();


	virtual std::vector<vk::DescriptorSet>*	GetPDescriptorSets();
	virtual vk::DescriptorPool				GetDescriptorPool();
	virtual void							Cleanup();


protected:
	vk::Device m_LogicalDevice;	// �쐬���j���p�̃f�o�C�X���
	const vk::DescriptorType		m_DescriptorType;
	uint32_t						m_SetCount;			// �쐬����f�X�N���v�^�Z�b�g�̐�
	vk::DescriptorPool				m_DescriptorPool;
	std::vector<vk::DescriptorSet>	m_DescriptorSet;

	virtual void CreateDescriptorPool(uint32_t setCount);
	// �f�B�X�N���v�^�Z�b�g�̊��蓖��
	virtual void CreateDescriptorSet(uint32_t setCount, vk::DescriptorSetLayout* pLayout);

};

