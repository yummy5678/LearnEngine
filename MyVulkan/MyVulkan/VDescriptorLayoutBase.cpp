#include "VDescriptorLayoutBase.h"


VDescriptorLayoutBase::VDescriptorLayoutBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag) :
    m_pLogicalDevice(nullptr),
    m_DescriptorType(type),
    m_StageFlags(stageFlag),
    m_DescriptorSetLayout(VK_NULL_HANDLE)
{
}

VDescriptorLayoutBase::~VDescriptorLayoutBase()
{
    if (m_pLogicalDevice == nullptr) return;

    if (m_DescriptorSetLayout) 
    {
        m_pLogicalDevice->destroyDescriptorSetLayout(m_DescriptorSetLayout);
    }

}

vk::DescriptorSetLayout VDescriptorLayoutBase::GetDescriptorSetLayout()
{
    return m_DescriptorSetLayout;
}

vk::DescriptorType VDescriptorLayoutBase::GetDescriptorType()
{
    return m_DescriptorType;
}

const vk::ShaderStageFlags VDescriptorLayoutBase::GetStageFlag()
{
    return m_StageFlags;
}

