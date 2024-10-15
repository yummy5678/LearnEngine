#pragma once
#include "VBufferBase.h"
#include "QueueUtility.h"

// 送信用バッファに設定する予定のフラグ
constexpr vk::BufferUsageFlags stagingImageUsage = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst;

class VStagingImageBuffer :
    public VBufferBase
{
public:
    VStagingImageBuffer();
    ~VStagingImageBuffer();

    void Initialize(VmaAllocator allocator, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageChannel);
    void TransferDataToImageBuffer(void* transfarData, vk::Image toBuffer);

private:
    vk::Device          m_LogicalDevice;
    vk::PhysicalDevice  m_PhysicalDevice;

    vk::CommandBuffer   m_CommandBuffer;
    vk::CommandPool	    m_CommandPool;

    uint32_t            m_ImageWidth;
    uint32_t            m_ImageHeight;
    uint32_t            m_ImageChannel;
    vk::Queue           m_Queue;

    vk::CommandPool CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex);
    vk::CommandBuffer CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool);

    void SetCopyToImageCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Image dstImage, uint32_t imageWidth, uint32_t imageHeight);

    // データをステージングバッファにコピー
    void MapData(VmaAllocator allocator, void* setData, vk::DeviceSize dataSize);

};

