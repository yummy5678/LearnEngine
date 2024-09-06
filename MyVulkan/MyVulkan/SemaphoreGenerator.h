#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"

class SemaphoreGenerator :
    public CGeneratorBase
{
public:
    SemaphoreGenerator();
    ~SemaphoreGenerator();

    void Create(vk::Device logicalDevice, uint32_t semaphoreCount);
    void Destroy();

    std::vector<vk::Semaphore> GetSemaphore();

private:
    vk::Device m_LogicalDevice;
    std::vector<vk::Semaphore> m_Semaphores;

    std::vector<vk::Semaphore> CreateSemaphore(vk::Device logicalDevice, uint32_t semaphoreCount);
};

