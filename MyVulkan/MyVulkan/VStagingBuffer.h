#pragma once
#include "VBufferBase.h"
#include "QueueUtility.h"

// 送信用バッファに設定する予定のフラグ
constexpr vk::BufferUsageFlags stagingUsage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst;

class VStagingBuffer :
    public VBufferBase
{
public:
    VStagingBuffer();
    ~VStagingBuffer();

    void Initialize(VmaAllocator allocator, vk::DeviceSize dataSize);

    void TransferDataToBuffer(void* transfarData, vk::DeviceSize dataSize, vk::Buffer toBuffer);

private:
    vk::Device          m_LogicalDevice;
    vk::PhysicalDevice  m_PhysicalDevice;

    vk::CommandBuffer   m_CommandBuffer;
    vk::CommandPool	    m_CommandPool;

    vk::CommandPool CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice);
    vk::CommandBuffer CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool);


};

// ステージングバッファとは


