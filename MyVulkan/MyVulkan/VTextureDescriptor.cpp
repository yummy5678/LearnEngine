#include "VTextureDescriptor.h"

VTextureDescriptor::VTextureDescriptor() :
    VDescriptorBase(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment)
{
}

VTextureDescriptor::~VTextureDescriptor()
{
}

void VTextureDescriptor::Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber)
{
    m_pLogicalDevice = pLogicalDevice;

    CreateDescriptorSetLayout(bindNumber);
    CreateDescriptorPool();
    CreateDescriptorSet();
}

void VTextureDescriptor::Update(vk::ImageView imageView, vk::Sampler sampler)
{
    UpdateDescriptorSet(imageView, sampler);
}

void VTextureDescriptor::CreateDescriptorSet()
{
    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_DescriptorSetLayout;

   m_DescriptorSet = m_pLogicalDevice->allocateDescriptorSets(allocInfo).front();
}

void VTextureDescriptor::UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler)
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


void VTextureDescriptor::CreateDescriptorSetLayout(uint32_t bindNumber)
{
    // ディスクリプタセットレイアウトバインディングの設定
    vk::DescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding = bindNumber;
    layoutBinding.descriptorType = m_DescriptorType; // テクスチャ用
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = m_StageFlags;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    m_DescriptorSetLayout = m_pLogicalDevice->createDescriptorSetLayout(layoutInfo);
}

void VTextureDescriptor::CreateDescriptorPool()
{
    // ディスクリプタプールの作成
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = 1;

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = m_pLogicalDevice->createDescriptorPool(poolInfo);
}
