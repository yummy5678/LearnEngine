#include "VDescriptorLayoutBase.h"


VDescriptorSetLayoutBase::VDescriptorSetLayoutBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag) :
    m_pLogicalDevice(nullptr),
    m_DescriptorType(type),
    m_StageFlags(stageFlag),
    m_DescriptorSetLayout(VK_NULL_HANDLE)
{
}

VDescriptorSetLayoutBase::~VDescriptorSetLayoutBase()
{
    if (*m_pLogicalDevice == VK_NULL_HANDLE) return;

    vk::DescriptorSetLayout layout = *m_DescriptorSetLayout.get();
    if (layout != VK_NULL_HANDLE)
    {
        m_pLogicalDevice->destroyDescriptorSetLayout(layout);
    }

}

std::shared_ptr<vk::DescriptorSetLayout> VDescriptorSetLayoutBase::GetDescriptorSetLayout()
{
    return m_DescriptorSetLayout;
}

vk::DescriptorType VDescriptorSetLayoutBase::GetDescriptorType()
{
    return m_DescriptorType;
}

const vk::ShaderStageFlags VDescriptorSetLayoutBase::GetStageFlag()
{
    return m_StageFlags;
}

