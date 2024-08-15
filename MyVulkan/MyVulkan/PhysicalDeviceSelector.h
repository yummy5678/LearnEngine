#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <functional>

class PhysicalDeviceSelector {
public:
    // �R���X�g���N�^
    PhysicalDeviceSelector(vk::Instance instance);

    // �O���t�B�b�N�X�����ɓK�����f�o�C�X��I��
    vk::PhysicalDevice selectGraphicsDevice();

    // �R���s���[�e�B���O�ɓK�����f�o�C�X��I��
    vk::PhysicalDevice selectComputeDevice();

    // ���̑��̗p�r�ɉ������f�o�C�X��I��
    vk::PhysicalDevice selectSwapchainDevice(std::function<bool(const vk::PhysicalDevice&, vk::SurfaceKHR)> criteria);

private:
    std::vector<vk::PhysicalDevice> m_PhysicalDevices;
    vk::Instance m_instance;
    vk::SurfaceKHR m_surface;

    bool CheckGraphics(vk::PhysicalDevice physicalDevice);
    bool CheckSwapchain(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    bool CheckCompute(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    
    std::optional<uint32_t> findQueueFamilies(vk::PhysicalDevice device);
};

