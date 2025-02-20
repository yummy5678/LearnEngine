#include "SingleTextureDescriptor.h"

VSingleTextureDescriptor::VSingleTextureDescriptor() : 
    VDescriptorBase(vk::DescriptorType::eCombinedImageSampler)
{
    m_SetCount = 1;
}

VSingleTextureDescriptor::~VSingleTextureDescriptor()
{
}

void VSingleTextureDescriptor::Initialize(vk::Device logicalDevice, vk::DescriptorSetLayout layout)
{

    m_LogicalDevice = logicalDevice;    
    VDescriptorBase::CreateDescriptorPool(m_SetCount);
    VDescriptorBase::CreateDescriptorSet(m_SetCount, &layout);
}

void VSingleTextureDescriptor::Update(vk::ImageView imageView, vk::Sampler sampler)
{
    UpdateDescriptorSet(imageView, sampler);
}

vk::DescriptorSet VSingleTextureDescriptor::GetDescriptorSet()
{
    return m_DescriptorSet.front();
}

void VSingleTextureDescriptor::UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler)
{
    // イメージとサンプラーをディスクリプタセットにバインド
    vk::DescriptorImageInfo imageInfo;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;
    imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = m_DescriptorSet.front();
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType  = m_DescriptorType;
    descriptorWrite.descriptorCount = m_SetCount;
    descriptorWrite.pImageInfo = &imageInfo;

    m_LogicalDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
}
