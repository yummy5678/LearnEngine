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

    std::vector<vk::Fence> CreateFence(vk::Device logicalDevice, uint32_t semaphoreCount);

};

