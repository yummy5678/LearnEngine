#include "SingleBufferDescriptor.h"

VSingleBufferDescriptor::VSingleBufferDescriptor() :
	VDescriptorBase(vk::DescriptorType::eUniformBuffer)
{
	m_SetCount = 1;
}

VSingleBufferDescriptor::~VSingleBufferDescriptor()
{
    Cleanup();
}

void VSingleBufferDescriptor::Initialize(vk::Device logicalDevice, vk::DescriptorSetLayout layout)
{
	m_LogicalDevice = logicalDevice;
	VDescriptorBase::CreateDescriptorPool(m_SetCount);
	VDescriptorBase::CreateDescriptorSet(m_SetCount, &layout);
}

void VSingleBufferDescriptor::Update(vk::Buffer buffer, vk::DeviceSize bufferSize)
{
	UpdateDescriptorSet(buffer, bufferSize);
}

void VSingleBufferDescriptor::Cleanup()
{
    printf("VSingleBufferDescriptorを解放します");
    VDescriptorBase::Cleanup();
}

vk::DescriptorSet VSingleBufferDescriptor::GetDescriptorSet()
{
    return m_DescriptorSet.front();
}

void VSingleBufferDescriptor::UpdateDescriptorSet(vk::Buffer buffer, vk::DeviceSize bufferSize)
{
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = buffer; // ユニフォームバッファを指定
    bufferInfo.offset = 0;
    bufferInfo.range = bufferSize;

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = m_DescriptorSet.front();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    m_LogicalDevice.updateDescriptorSets(descriptorWrite, nullptr);
}
