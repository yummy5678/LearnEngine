#include "DescriptorBase.h"


VDescriptorBase::VDescriptorBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag) :
    m_DescriptorType(type),
    m_StageFlags(stageFlag)
{
}

VDescriptorBase::~VDescriptorBase()
{
    if (m_DescriptorSetLayout) 
    {
        m_LogicalDevice.destroyDescriptorSetLayout(m_DescriptorSetLayout);
    }

    if (m_DescriptorPool) 
    {
        m_LogicalDevice.destroyDescriptorPool(m_DescriptorPool);
    }

}
