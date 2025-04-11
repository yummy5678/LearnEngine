#include "VDescriptorLayoutBase.h"


VDescriptorSetLayoutBase::VDescriptorSetLayoutBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag) :
    m_LogicalDevice(nullptr),
    m_DescriptorType(type),
    m_StageFlags(stageFlag),
    m_DescriptorSetLayout(VK_NULL_HANDLE)
{
}

VDescriptorSetLayoutBase::~VDescriptorSetLayoutBase()
{
    Cleanup();
}

void VDescriptorSetLayoutBase::Cleanup()
{
    if (m_LogicalDevice == VK_NULL_HANDLE) return;

    vk::DescriptorSetLayout layout = *m_DescriptorSetLayout.get();
    if (layout != VK_NULL_HANDLE)
    {
        m_LogicalDevice.destroyDescriptorSetLayout(layout);
        m_DescriptorSetLayout.reset();
    }

    m_LogicalDevice = VK_NULL_HANDLE;
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

