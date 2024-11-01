#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"


class FenceGenerator :
    public CGeneratorBase
{
public:
    FenceGenerator();
    ~FenceGenerator();

    void Create(vk::Device logicalDevice, uint32_t fenceCount);
    void Destroy();

    std::vector<vk::Fence> GetFence();

private:
    vk::Device m_LogicalDevice;
    std::vector<vk::Fence> m_Fences;

    std::vector<vk::Fence> CreateFences(vk::Device logicalDevice, uint32_t semaphoreCount);

};

// �t�F���X�ɂ���
// GPU�̑��삪�������������m�F���邽�߂̓����I�u�W�F�N�g�B