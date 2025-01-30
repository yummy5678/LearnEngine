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
	vk::Device* m_pLogicalDevice;	// 作成＆破棄用のデバイス情報
	const vk::DescriptorType	m_DescriptorType;
	uint32_t					m_SetCount;			// 作成するデスクリプタセットの数
	vk::DescriptorPool			m_DescriptorPool;
	vk::DescriptorSet			m_DescriptorSet;


	void CreateDescriptorPool(uint32_t setCount);
	// ディスクリプタセットの割り当て
	void CreateDescriptorSet(uint32_t setCount, vk::DescriptorSetLayout* pLayout);

	void UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);

};

