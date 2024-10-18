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
    // �f�B�X�N���v�^�Z�b�g�̊��蓖��
    vk::DescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_DescriptorSetLayout;

    vk::DescriptorSet descriptorSet = m_LogicalDevice.allocateDescriptorSets(allocInfo)[0];

    // �C���[�W�ƃT���v���[���f�B�X�N���v�^�Z�b�g�Ƀo�C���h
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
    // �f�B�X�N���v�^�Z�b�g���C�A�E�g�o�C���f�B���O�̐ݒ�
    vk::DescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding = 0;
    layoutBinding.descriptorType = m_DescriptorType; // �e�N�X�`���p
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = m_StageFlags;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    m_DescriptorSetLayout = m_LogicalDevice.createDescriptorSetLayout(layoutInfo);
}

void VTextureDescriptor::CreateDescriptorPool()
{
    // �f�B�X�N���v�^�v�[���̍쐬
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = 1;

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = m_LogicalDevice.createDescriptorPool(poolInfo);
}
