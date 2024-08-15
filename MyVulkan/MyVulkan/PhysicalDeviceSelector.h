#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <functional>

class PhysicalDeviceSelector {
public:
    // コンストラクタ
    PhysicalDeviceSelector(vk::Instance instance);

    // グラフィックス処理に適したデバイスを選択
    vk::PhysicalDevice selectGraphicsDevice();

    // コンピューティングに適したデバイスを選択
    vk::PhysicalDevice selectComputeDevice();

    // その他の用途に応じたデバイスを選択
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

