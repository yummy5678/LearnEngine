#include "VSingleDescriptor.h"

VSingleDescriptor::VSingleDescriptor() : 
    m_pLogicalDevice(nullptr),
    m_DescriptorType(vk::DescriptorType::eCombinedImageSampler),
    m_SetCount(0),
    m_DescriptorSet(VK_NULL_HANDLE),
    m_DescriptorPool(VK_NULL_HANDLE)
{
}

VSingleDescriptor::~VSingleDescriptor()
{
}

void VSingleDescriptor::Initialize(vk::Device* pLogicalDevice, vk::DescriptorSetLayout layout)
{
    m_pLogicalDevice = pLogicalDevice;
    m_SetCount = 1;
    CreateDescriptorPool(m_SetCount);
    CreateDescriptorSet(m_SetCount, &layout);
}

void VSingleDescriptor::Update(vk::ImageView imageView, vk::Sampler sampler)
{
    UpdateDescriptorSet(imageView, sampler);
}

vk::DescriptorSet VSingleDescriptor::GetDescriptorSet()
{
    return m_DescriptorSet;
}

vk::DescriptorPool VSingleDescriptor::GetDescriptorPool()
{
    return m_DescriptorPool;
}

void VSingleDescriptor::CreateDescriptorPool(uint32_t setCount)
{
    // ディスクリプタプールの作成
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = 1;

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = setCount;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = m_pLogicalDevice->createDescriptorPool(poolInfo);
}

void VSingleDescriptor::CreateDescriptorSet(uint32_t setCount, vk::DescriptorSetLayout* pLayout)
{
    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = setCount;
    allocInfo.pSetLayouts = pLayout;

    m_DescriptorSet = m_pLogicalDevice->allocateDescriptorSets(allocInfo).front();
}

void VSingleDescriptor::UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler)
{
    // イメージとサンプラーをディスクリプタセットにバインド
    vk::DescriptorImageInfo imageInfo;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;
    imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = m_DescriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = m_DescriptorType;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    m_pLogicalDevice->updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
}
