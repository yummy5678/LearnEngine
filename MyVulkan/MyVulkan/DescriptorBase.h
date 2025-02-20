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
	vk::Device m_LogicalDevice;	// 作成＆破棄用のデバイス情報
	const vk::DescriptorType		m_DescriptorType;
	uint32_t						m_SetCount;			// 作成するデスクリプタセットの数
	vk::DescriptorPool				m_DescriptorPool;
	std::vector<vk::DescriptorSet>	m_DescriptorSet;

	virtual void CreateDescriptorPool(uint32_t setCount);
	// ディスクリプタセットの割り当て
	virtual void CreateDescriptorSet(uint32_t setCount, vk::DescriptorSetLayout* pLayout);

};

