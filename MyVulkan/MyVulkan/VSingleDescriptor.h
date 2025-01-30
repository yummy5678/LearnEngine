#pragma once
#include <vulkan/vulkan.hpp>


class VSingleDescriptor
{
public:
	VSingleDescriptor();
	~VSingleDescriptor();

	void Initialize(vk::Device* pLogicalDevice, vk::DescriptorSetLayout layout);
	void Update(vk::ImageView imageView, vk::Sampler sampler);


	virtual vk::DescriptorSet		GetDescriptorSet();
	virtual vk::DescriptorPool		GetDescriptorPool();

private:
	vk::Device* m_pLogicalDevice;	// �쐬���j���p�̃f�o�C�X���
	const vk::DescriptorType	m_DescriptorType;
	uint32_t					m_SetCount;			// �쐬����f�X�N���v�^�Z�b�g�̐�
	vk::DescriptorPool			m_DescriptorPool;
	vk::DescriptorSet			m_DescriptorSet;


	void CreateDescriptorPool(uint32_t setCount);
	// �f�B�X�N���v�^�Z�b�g�̊��蓖��
	void CreateDescriptorSet(uint32_t setCount, vk::DescriptorSetLayout* pLayout);

	void UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);

};

