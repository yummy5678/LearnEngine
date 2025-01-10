#include "VCameraDescriptor.h"


VCameraDescriptor::VCameraDescriptor() : 
	VDescriptorBase(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex)
{
}

VCameraDescriptor::~VCameraDescriptor()
{
}

void VCameraDescriptor::Initialize(vk::Device* pLogicalDevice)
{
    m_pLogicalDevice = pLogicalDevice;


    if (!m_DescriptorSetLayout) CreateDescriptorSetLayout(pLogicalDevice);
    if (!m_DescriptorPool)      CreateDescriptorPool(pLogicalDevice);
    if (!m_DescriptorSet)       CreateDescriptorSet(pLogicalDevice);

}

void VCameraDescriptor::Update(vk::Buffer buffer)
{
    UpdateDescriptorSet(m_DescriptorSet, buffer, sizeof(ViewProjection));
}


void VCameraDescriptor::CreateDescriptorSetLayout(vk::Device* pLogicalDevice)
{
    vk::DescriptorSetLayoutBinding descSetLayoutBinding;
    descSetLayoutBinding.binding = 0;                       // �V�F�[�_�[���̈ʒu
    descSetLayoutBinding.descriptorType = m_DescriptorType; // ���j�t�H�[���o�b�t�@�̏��
    descSetLayoutBinding.descriptorCount = 1;               // 1�̃o�b�t�@
    descSetLayoutBinding.stageFlags = m_StageFlags;         // ���_�V�F�[�_�[�Ŏg��

    vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo;
    descSetLayoutCreateInfo.bindingCount = 1;
    descSetLayoutCreateInfo.pBindings = &descSetLayoutBinding;

    m_DescriptorSetLayout = pLogicalDevice->createDescriptorSetLayout(descSetLayoutCreateInfo);
}

void VCameraDescriptor::CreateDescriptorSet(vk::Device* pLogicalDevice)
{
    vk::DescriptorSetAllocateInfo setAllocateInfo;
    setAllocateInfo.descriptorPool = m_DescriptorPool;
    setAllocateInfo.descriptorSetCount = 1;
    setAllocateInfo.pSetLayouts = &m_DescriptorSetLayout;

    m_DescriptorSet = pLogicalDevice->allocateDescriptorSets(setAllocateInfo).front();
}

void VCameraDescriptor::CreateDescriptorPool(vk::Device* pLogicalDevice)
{
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = 1; // �f�B�X�N���v�^�̐�

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = pLogicalDevice->createDescriptorPool(poolInfo);
}

void VCameraDescriptor::UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize)
{
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = buffer; // ���j�t�H�[���o�b�t�@���w��
    bufferInfo.offset = 0;
    bufferInfo.range = bufferSize;

    vk::WriteDescriptorSet descriptorWrite;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    m_pLogicalDevice->updateDescriptorSets(descriptorWrite, nullptr);
}
