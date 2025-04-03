#pragma once
#include "VBufferBase.h"
#include "QueueUtility.h"
#include "Texture.h"

class VImageBufferBase;

class VStagingImageBuffer :
    public VBufferBase
{
public:
    VStagingImageBuffer();
    ~VStagingImageBuffer();

    void Initialize(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight, TextureChannel imageChannel);

    // メモリ(RAM)側のデータをGPU用のメモリ(VRAM)側に移動させるための関数
    void TransferHostDataToImageBuffer(void* transferData, vk::Image toBuffer, vk::Fence fence);

    // GPU用のメモリ(VRAM)側のデータをメモリ(RAM)側に移動させるための関数
    void TransferImageBufferToHostData(VImageBufferBase* transferBuffer, Texture* toData, vk::Fence fence);

private:
    vk::Device          m_LogicalDevice;
    vk::PhysicalDevice  m_PhysicalDevice;

    vk::CommandBuffer   m_CommandBuffer;
    vk::CommandPool	    m_CommandPool;

    uint32_t            m_ImageWidth;
    uint32_t            m_ImageHeight;
    TextureChannel      m_ImageChannel;
    vk::Queue           m_Queue;

    vk::CommandPool CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex);
    vk::CommandBuffer CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool);

    void SetCopyToImageCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Image dstImage, uint32_t imageWidth, uint32_t imageHeight);
    void SetCopyImageToBufferCommand(vk::CommandBuffer commandBuffer, VImageBufferBase* srcImage, vk::Buffer dstBuffer,  uint32_t imageWidth, uint32_t imageHeight);

    // データをステージングバッファにコピー
    //void MapData(VmaAllocator* allocator, void* setData, vk::DeviceSize dataSize);

};

