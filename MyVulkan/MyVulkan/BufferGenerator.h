#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
//#include "Utilities.h"
#include "ModelLoder.h"

class BufferGenerator :
    public CGeneratorBase
{
public:
    BufferGenerator();
    ~BufferGenerator();

    void CreateMeshObject(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, MeshObject object);

    // データをバッファにコピーするメソッド
    void CopyDataToBuffer(void* data, VkDeviceSize size);

    void Destroy();



private:
    // 結びつける先のデバイス
    vk::Device				m_LogicalDevice;
    vk::PhysicalDevice		m_PhysicalDevice;

    uint32_t m_BufferCount;
    std::vector<vk::Buffer>         m_VertexBuffer;
    std::vector<vk::DeviceMemory>   m_VertexBufferMemory;

    // 頂点データのバッファを作成
    vk::BufferCreateInfo CreateVertexBufferInfo(std::vector<Vertex> vertices);

    // メモリ割り当て関数
    vk::MemoryAllocateInfo AllocateBufferMemoryInfo(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags);
    uint32_t FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);
};

