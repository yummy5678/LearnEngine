#include "VCameraDescriptorSetLayout.h"



VCameraDescriptorSetLayout::VCameraDescriptorSetLayout() : 
	VDescriptorSetLayoutBase(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eVertex)
{
}

VCameraDescriptorSetLayout::~VCameraDescriptorSetLayout()
{
}

void VCameraDescriptorSetLayout::Initialize(vk::Device logicalDevice, uint32_t bindNumber)
{
    m_LogicalDevice = logicalDevice;


    CreateDescriptorSetLayout(bindNumber);
    //CreateDescriptorPool();
    //CreateDescriptorSet();

}

//void VCameraDescriptorSetLayout::Update(vk::Buffer buffer)
//{
//    UpdateDescriptorSet(m_DescriptorSet, buffer, sizeof(ViewProjection));
//}


void VCameraDescriptorSetLayout::CreateDescriptorSetLayout(uint32_t bindNumber)
{
    if (m_LogicalDevice == nullptr) 
        throw std::runtime_error("CreateDescriptorSet : 論理デバイスがnullptrです!");

    vk::DescriptorSetLayoutBinding descSetLayoutBinding;
    descSetLayoutBinding.binding = bindNumber;              // シェーダー側の位置
    descSetLayoutBinding.descriptorType = m_DescriptorType; // ユニフォームバッファの情報
    descSetLayoutBinding.descriptorCount = 1;               // 1つのバッファ
    descSetLayoutBinding.stageFlags = m_StageFlags;         // 頂点シェーダーで使う

    vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo;
    descSetLayoutCreateInfo.bindingCount = 1;
    descSetLayoutCreateInfo.pBindings = &descSetLayoutBinding;

    m_DescriptorSetLayout = std::make_shared<vk::DescriptorSetLayout>(m_LogicalDevice.createDescriptorSetLayout(descSetLayoutCreateInfo));
}

//void VCameraDescriptorSetLayout::CreateDescriptorSet()
//{
//    if (m_LogicalDevice == nullptr) throw std::runtime_error("CreateDescriptorSet : 論理デバイスがnullptrです!");
//
//    vk::DescriptorSetAllocateInfo setAllocateInfo;
//    setAllocateInfo.descriptorPool = m_DescriptorPool;
//    setAllocateInfo.descriptorSetCount = 1;
//    setAllocateInfo.pSetLayouts = &m_DescriptorSetLayout;
//
//    m_DescriptorSet = m_LogicalDevice->allocateDescriptorSets(setAllocateInfo).front();
//}
//
//void VCameraDescriptorSetLayout::CreateDescriptorPool()
//{
//    if (m_LogicalDevice == nullptr) throw std::runtime_error("CreateDescriptorSet : 論理デバイスがnullptrです!");
//    vk::DescriptorPoolSize poolSize;
//    poolSize.type = m_DescriptorType;
//    poolSize.descriptorCount = 1; // ディスクリプタの数
//
//    vk::DescriptorPoolCreateInfo poolInfo;
//    poolInfo.maxSets = 1;
//    poolInfo.poolSizeCount = 1;
//    poolInfo.pPoolSizes = &poolSize;
//
//    m_DescriptorPool = m_LogicalDevice->createDescriptorPool(poolInfo);
//}

//void VCameraDescriptorSetLayout::UpdateDescriptorSet(vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize)
//{
//    vk::DescriptorBufferInfo bufferInfo;
//    bufferInfo.buffer = buffer; // ユニフォームバッファを指定
//    bufferInfo.offset = 0;
//    bufferInfo.range = bufferSize;
//
//    vk::WriteDescriptorSet descriptorWrite;
//    descriptorWrite.dstSet = descriptorSet;
//    descriptorWrite.dstBinding = 0;
//    descriptorWrite.dstArrayElement = 0;
//    descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
//    descriptorWrite.descriptorCount = 1;
//    descriptorWrite.pBufferInfo = &bufferInfo;
//
//    m_LogicalDevice->updateDescriptorSets(descriptorWrite, nullptr);
//}
