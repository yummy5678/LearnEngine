#include "DescriptorHandler.h"

VDescriptor::VDescriptor()
{
}

VDescriptor::~VDescriptor()
{
    if (m_DescriptorSetLayout) 
    {
        m_Device.destroyDescriptorSetLayout(m_DescriptorSetLayout);
    }

    if (m_DescriptorPool) 
    {
        m_Device.destroyDescriptorPool(m_DescriptorPool);
    }

}

void VDescriptor::SetData(vk::Device device, vk::DescriptorType descriptorType, vk::ShaderStageFlags stageFlags)
{
    m_Device = device;
    m_DescriptorType = descriptorType;
    m_StageFlags = stageFlags;

    CreateDescriptorSetLayout();

}

void VDescriptor::CreateDescriptorPool(uint32_t descriptorCount)
{
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = descriptorCount;

    vk::DescriptorPoolCreateInfo poolInfo{};
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = descriptorCount;

    m_DescriptorPool = m_Device.createDescriptorPool(poolInfo);
}

vk::DescriptorSet VDescriptor::CreateDescriptorSet(vk::Buffer buffer, vk::DeviceSize bufferSize)
{
    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_DescriptorSetLayout;

    vk::DescriptorSet descriptorSet = m_Device.allocateDescriptorSets(allocInfo)[0];

    vk::DescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = 0;
    bufferInfo.range = bufferSize;

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = m_DescriptorType;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    m_Device.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);

    return descriptorSet;
}

vk::DescriptorSetLayout VDescriptor::GetDescriptorSetLayout()
{
    return m_DescriptorSetLayout;
}

void VDescriptor::CreateDescriptorSetLayout()
{
    vk::DescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = m_DescriptorType;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = m_StageFlags;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    m_DescriptorSetLayout = m_Device.createDescriptorSetLayout(layoutInfo);

}
