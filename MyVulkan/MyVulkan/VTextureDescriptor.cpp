#include "VTextureDescriptor.h"

VTextureDescriptor::VTextureDescriptor() :
    VDescriptorBase(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment)
{
}

VTextureDescriptor::~VTextureDescriptor()
{
}

vk::DescriptorSet VTextureDescriptor::CreateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler)
{
    // ディスクリプタセットの割り当て
    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_DescriptorSetLayout;

    vk::DescriptorSet descriptorSet = m_LogicalDevice.allocateDescriptorSets(allocInfo)[0];

    // イメージとサンプラーをディスクリプタセットにバインド
    vk::DescriptorImageInfo imageInfo{};
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;
    imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    
    vk::WriteDescriptorSet descriptorWrite;  
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    m_LogicalDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);

    return descriptorSet;
}

vk::DescriptorSetLayout VTextureDescriptor::GetDescriptorSetLayout() const
{
    return m_DescriptorSetLayout;
}

void VTextureDescriptor::CreateDescriptorSetLayout()
{
    // ディスクリプタセットレイアウトバインディングの設定
    vk::DescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = m_DescriptorType; // テクスチャ用
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = m_StageFlags;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    m_DescriptorSetLayout = m_LogicalDevice.createDescriptorSetLayout(layoutInfo);
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

    m_DescriptorPool = m_LogicalDevice.createDescriptorPool(poolInfo);
}
