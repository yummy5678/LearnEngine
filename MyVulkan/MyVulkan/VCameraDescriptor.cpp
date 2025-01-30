#include "VCameraDescriptor.h"


VCameraDescriptor::VCameraDescriptor() : 
	VDescriptorLayoutBase(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex)
{
}

VCameraDescriptor::~VCameraDescriptor()
{
}

void VCameraDescriptor::Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber)
{
    m_pLogicalDevice = pLogicalDevice;


    CreateDescriptorSetLayout(bindNumber);
    CreateDescriptorPool();
    CreateDescriptorSet();

}

void VCameraDescriptor::Update(vk::Buffer buffer)
{
    UpdateDescriptorSet(m_DescriptorSet, buffer, sizeof(ViewProjection));
}


void VCameraDescriptor::CreateDescriptorSetLayout(uint32_t bindNumber)
{
    if (m_pLogicalDevice == nullptr ||
        *m_pLogicalDevice == nullptr) throw std::runtime_error("CreateDescriptorSet : 論理デバイスがnullptrです!");
    vk::DescriptorSetLayoutBinding descSetLayoutBinding;
    descSetLayoutBinding.binding = bindNumber;              // シェーダー側の位置
    descSetLayoutBinding.descriptorType = m_DescriptorType; // ユニフォームバッファの情報
    descSetLayoutBinding.descriptorCount = 1;               // 1つのバッファ
    descSetLayoutBinding.stageFlags = m_StageFlags;         // 頂点シェーダーで使う

    vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo;
    descSetLayoutCreateInfo.bindingCount = 1;
    descSetLayoutCreateInfo.pBindings = &descSetLayoutBinding;

    m_DescriptorSetLayout = m_pLogicalDevice->createDescriptorSetLayout(descSetLayoutCreateInfo);
}

void VCameraDescriptor::CreateDescriptorSet()
{
    if (m_pLogicalDevice == nullptr) throw std::runtime_error("CreateDescriptorSet : 論理デバイスがnullptrです!");

    vk::DescriptorSetAllocateInfo setAllocateInfo;
    setAllocateInfo.descriptorPool = m_DescriptorPool;
    setAllocateInfo.descriptorSetCount = 1;
    setAllocateInfo.pSetLayouts = &m_DescriptorSetLayout;

    m_DescriptorSet = m_pLogicalDevice->allocateDescriptorSets(setAllocateInfo).front();
}

void VCameraDescriptor::CreateDescriptorPool()
{
    if (m_pLogicalDevice == nullptr) throw std::runtime_error("CreateDescriptorSet : 論理デバイスがnullptrです!");
    vk::DescriptorPoolSize poolSize;
    poolSize.type = m_DescriptorType;
    poolSize.descriptorCount = 1; // ディスクリプタの数

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    m_DescriptorPool = m_pLogicalDevice->createDescriptorPool(poolInfo);
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
