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

    // �f�[�^���o�b�t�@�ɃR�s�[���郁�\�b�h
    void CopyDataToBuffer(void* data, VkDeviceSize size);

    void Destroy();



private:
    // ���т����̃f�o�C�X
    vk::Device				m_LogicalDevice;
    vk::PhysicalDevice		m_PhysicalDevice;

    uint32_t m_BufferCount;
    std::vector<vk::Buffer>         m_VertexBuffer;
    std::vector<vk::DeviceMemory>   m_VertexBufferMemory;

    // ���_�f�[�^�̃o�b�t�@���쐬
    vk::BufferCreateInfo CreateVertexBufferInfo(std::vector<Vertex> vertices);

    // ���������蓖�Ċ֐�
    vk::MemoryAllocateInfo AllocateBufferMemoryInfo(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags);
    uint32_t FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType);
};

