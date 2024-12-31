#include "SceneCamera.h"


SceneCamera::SceneCamera() :
    m_DescriptorPool(VK_NULL_HANDLE),
    m_DescriptorSet(VK_NULL_HANDLE),
    m_ProjectionBuffer()
{
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateBuffer(VmaAllocator* allocator, ViewProjection viewProjection)
{
    m_ProjectionBuffer.Update(allocator, viewProjection);

    // ディスクリプタの作成と更新
    VmaAllocatorInfo allocatorInfo;
    vmaGetAllocatorInfo(*allocator, &allocatorInfo); // アロケータに入っている情報を間接的に取得
    vk::Device logicalDevice(allocatorInfo.device);

    if (!m_DescriptorPool) CreateSingleDescriptorPool(m_DescriptorPool, logicalDevice);
    if (!m_DescriptorSet) CreateSingleDescriptorSet(logicalDevice, m_DescriptorPool);
    UpdateDescriptorSet(logicalDevice, m_DescriptorSet, m_ProjectionBuffer.GetBuffer(), m_ProjectionBuffer.GetDataSize());
}


vk::DescriptorSet SceneCamera::GetDescriptorSet()
{
    return m_DescriptorSet;
}

void SceneCamera::CreateSingleDescriptorSet(vk::Device logicalDevice, vk::DescriptorPool pool)
{
    vk::DescriptorSetLayoutBinding descSetLayoutBinding;
    descSetLayoutBinding.binding = 0;                                            // シェーダー側の位置
    descSetLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;    // ユニフォームバッファの情報
    descSetLayoutBinding.descriptorCount = 1;                                    // 1つのバッファ
    descSetLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;          // 頂点シェーダーで使う

    vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo{};
    descSetLayoutCreateInfo.bindingCount = 1;
    descSetLayoutCreateInfo.pBindings = &descSetLayoutBinding;

    vk::DescriptorSetLayout descriptorLayout;

    vk::DescriptorSetAllocateInfo setAllocateInfo;
    setAllocateInfo.descriptorPool = pool;
    setAllocateInfo.descriptorSetCount = 1;
    setAllocateInfo.pSetLayouts = &descriptorLayout;

    m_DescriptorSet = logicalDevice.allocateDescriptorSets(setAllocateInfo).front();
}

void SceneCamera::CreateSingleDescriptorPool(vk::DescriptorPool& setPool, vk::Device logicalDevice)
{
    vk::DescriptorPoolSize poolSize;
    poolSize.type = vk::DescriptorType::eUniformBuffer;
    poolSize.descriptorCount = 1; // ディスクリプタの数

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    setPool = logicalDevice.createDescriptorPool(poolInfo);
}

void SceneCamera::UpdateDescriptorSet(vk::Device logicalDevice, vk::DescriptorSet descriptorSet, vk::Buffer buffer, vk::DeviceSize bufferSize)
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

    logicalDevice.updateDescriptorSets(descriptorWrite, nullptr);
}
