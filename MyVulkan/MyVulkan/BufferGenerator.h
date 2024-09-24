#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"

class BufferGenerator :
    public CGeneratorBase
{
public:
    BufferGenerator();
    ~BufferGenerator();

    void CreateVertex(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, uint32_t bufferCount);
    void Destroy();



private:
    vk::Device				m_LogicalDevice;
    vk::PhysicalDevice		m_PhysicalDevice;

    uint32_t m_BufferCount;
    std::vector<vk::Buffer>         m_Buffer;
    std::vector<vk::DeviceMemory>   m_BufferMemory;

    vk::BufferCreateInfo CreateVertexBufferInfo(uint32_t vertexSize, vk::Format fomat, vk::BufferUsageFlags usage);


    vk::MemoryAllocateInfo AllocateBufferMemory(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags);

};

