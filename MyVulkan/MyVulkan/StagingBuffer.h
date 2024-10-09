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

    vk::Queue           m_Queue;

    vk::CommandPool CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex);
    vk::CommandBuffer CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool);

    void SetCopyCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size);

    // データをステージングバッファにコピー
    void MapData(VmaAllocator allocator, void* setData, vk::DeviceSize dataSize);
};

// ステージングバッファとは


