#include "VTextureDescriptorLayout.h"

VTextureDescriptorSetLayout::VTextureDescriptorSetLayout() :
    VDescriptorSetLayoutBase(vk::DescriptorType::eSampledImage, vk::ShaderStageFlagBits::eFragment)
{
}

VTextureDescriptorSetLayout::~VTextureDescriptorSetLayout()
{
}

void VTextureDescriptorSetLayout::Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber)
{
    m_pLogicalDevice = pLogicalDevice;

    CreateDescriptorSetLayout(bindNumber);
}

void VTextureDescriptorSetLayout::CreateDescriptorSetLayout(uint32_t bindNumber)
{
    // �f�B�X�N���v�^�Z�b�g���C�A�E�g�o�C���f�B���O�̐ݒ�
    vk::DescriptorSetLayoutBinding layoutBinding;
    layoutBinding.binding = bindNumber;
    layoutBinding.descriptorType = m_DescriptorType; // �e�N�X�`���p
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = m_StageFlags;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &layoutBinding;

    m_DescriptorSetLayout = std::make_shared<vk::DescriptorSetLayout>(m_pLogicalDevice->createDescriptorSetLayout(layoutInfo));
}
