#include "VTextureDescriptorLayout.h"

VTextureDescriptorLayout::VTextureDescriptorLayout() :
    VDescriptorLayoutBase(vk::DescriptorType::eSampledImage, vk::ShaderStageFlagBits::eFragment)
{
}

VTextureDescriptorLayout::~VTextureDescriptorLayout()
{
}

void VTextureDescriptorLayout::Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber)
{
    m_pLogicalDevice = pLogicalDevice;

    CreateDescriptorSetLayout(bindNumber);
}

void VTextureDescriptorLayout::CreateDescriptorSetLayout(uint32_t bindNumber)
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
