#include "DescriptorBase.h"


VDescriptorBase::VDescriptorBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag) :
    m_pLogicalDevice(nullptr),
    m_DescriptorType(type),
    m_StageFlags(stageFlag),
    m_DescriptorPool(VK_NULL_HANDLE),
    m_DescriptorSet(VK_NULL_HANDLE),
    m_DescriptorSetLayout(VK_NULL_HANDLE)
{
}

VDescriptorBase::~VDescriptorBase()
{
    if (m_pLogicalDevice == nullptr) return;

    if (m_DescriptorSetLayout) 
    {
        m_pLogicalDevice->destroyDescriptorSetLayout(m_DescriptorSetLayout);
    }

    if (m_DescriptorPool) 
    {
        m_pLogicalDevice->destroyDescriptorPool(m_DescriptorPool);
    }

}

vk::DescriptorSet VDescriptorBase::GetDescriptorSet()
{
    return m_DescriptorSet;
}

vk::DescriptorSetLayout VDescriptorBase::GetDescriptorSetLayout()
{
    return m_DescriptorSetLayout;
}

// ディスクリプタの全ての要素を持っていたらTRUEを返す
bool VDescriptorBase::HasDescriptor()
{
    return (m_DescriptorSetLayout   != VK_NULL_HANDLE &&
            m_DescriptorPool        != VK_NULL_HANDLE &&
            m_DescriptorSet         != VK_NULL_HANDLE);
}
