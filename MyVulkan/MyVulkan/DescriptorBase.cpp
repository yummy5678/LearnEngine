#include "DescriptorBase.h"
#include <iostream>

VDescriptorBase::VDescriptorBase(vk::DescriptorType type) :
    m_LogicalDevice(VK_NULL_HANDLE),
    m_DescriptorType(type),
    m_SetCount(0),
    m_DescriptorSet(),
    m_DescriptorPool(VK_NULL_HANDLE)
{
}

VDescriptorBase::~VDescriptorBase()
{
}

std::vector<vk::DescriptorSet>* VDescriptorBase::GetPDescriptorSets()
{
	return &m_DescriptorSet;
}

vk::DescriptorPool VDescriptorBase::GetDescriptorPool()
{
	return m_DescriptorPool;
}

void VDescriptorBase::Cleanup()
{
    if (m_LogicalDevice == VK_NULL_HANDLE) return;

    //デスクリプタプールを解放
    if (m_DescriptorPool != VK_NULL_HANDLE)
    {
        m_LogicalDevice.destroyDescriptorPool(m_DescriptorPool, nullptr);
        m_DescriptorPool = VK_NULL_HANDLE;
        m_DescriptorSet.clear();
    }

    m_SetCount = 0;

    m_LogicalDevice == VK_NULL_HANDLE;
}

void VDescriptorBase::CreateDescriptorPool(uint32_t setCount)
{
    if (m_LogicalDevice == VK_NULL_HANDLE)
    {
        std::cerr << "ディスクリプタの作成時にロジカルデバイスが登録されていません！" << std::endl;
        return;
    }


    // ディスクリプタプールの作成
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = 1;

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = setCount;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = m_LogicalDevice.createDescriptorPool(poolInfo);
}

void VDescriptorBase::CreateDescriptorSet(uint32_t setCount, vk::DescriptorSetLayout* pLayout)
{
    if (m_LogicalDevice == VK_NULL_HANDLE)
    {
        std::cerr << "ディスクリプタの作成時にロジカルデバイスが登録されていません！" << std::endl;
        return;
    }

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = setCount;
    allocInfo.pSetLayouts = pLayout;

    m_DescriptorSet = m_LogicalDevice.allocateDescriptorSets(allocInfo);
}
