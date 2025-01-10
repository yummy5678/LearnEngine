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
    descSetLayoutBinding.binding = 0;                       // シェーダー側の位置
    descSetLayoutBinding.descriptorType = m_DescriptorType; // ユニフォームバッファの情報
    descSetLayoutBinding.descriptorCount = 1;               // 1つのバッファ
    descSetLayoutBinding.stageFlags = m_StageFlags;         // 頂点シェーダーで使う

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
    poolSize.descriptorCount = 1; // ディスクリプタの数

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = pLogicalDevice->createDescriptorPool(poolInfo);
}

void VCameraDescriptor::UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize)
{
    vk::DescriptorBufferInfo bufferInfo;
    bufferInfo.buffer = buffer; // ユニフォームバッファを指定
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
