#include "SingleBufferDescriptor.h"

VSingleBufferDescriptor::VSingleBufferDescriptor() :
	VDescriptorBase(vk::DescriptorType::eUniformBuffer)
{
	m_SetCount = 1;
}

VSingleBufferDescriptor::~VSingleBufferDescriptor()
{
}

void VSingleBufferDescriptor::Initialize(vk::Device* pLogicalDevice, vk::DescriptorSetLayout layout)
{
	m_pLogicalDevice = pLogicalDevice;
	VDescriptorBase::CreateDescriptorPool(m_SetCount);
	VDescriptorBase::CreateDescriptorSet(m_SetCount, &layout);
}

void VSingleBufferDescriptor::Update(vk::Buffer buffer, vk::DeviceSize bufferSize)
{
	UpdateDescriptorSet(buffer, bufferSize);
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

    m_pLogicalDevice->updateDescriptorSets(descriptorWrite, nullptr);
}
