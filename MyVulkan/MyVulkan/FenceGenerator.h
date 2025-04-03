#pragma once
#include <vulkan/vulkan.hpp>
#include "NonCopyable.h"


class FenceGenerator :
    public NonCopyable
{
public:
    FenceGenerator();
    ~FenceGenerator();

    void Create(vk::Device logicalDevice, size_t fenceCount);
    void Destroy();

    std::vector<vk::Fence> GetFence();

private:
    vk::Device m_LogicalDevice;
    std::vector<vk::Fence> m_Fences;

    std::vector<vk::Fence> CreateFences(vk::Device logicalDevice, size_t semaphoreCount);

};

// �t�F���X�ɂ���
// GPU�̑��삪�������������m�F���邽�߂̓����I�u�W�F�N�g�B