#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <functional>
#include "QueueUtility.h"

struct PhysicalDeviceContainer
{
    vk::PhysicalDevice Handle = VK_NULL_HANDLE;
    std::vector<vk::DeviceQueueCreateInfo> QueueInfo;
};

class PhysicalDeviceSelector 
{
public:
    // コンストラクタ
    PhysicalDeviceSelector(vk::Instance instance);

    // 描画処理に適したデバイスを選択
    PhysicalDeviceContainer SelectGraphicsDevice();

    // 計算処理に適したデバイスを選択
    PhysicalDeviceContainer SelectComputeDevice();

    // データの転送処理に適したデバイスを選択
    PhysicalDeviceContainer SelectTransferDevice();

    // スワップチェーン、画面表示に適したデバイスを選択
    PhysicalDeviceContainer SelectSwapchainDevice();

    //物理デバイスがサーフェスに対応しているか確認
    bool CheckSupportSurface(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    //物理デバイスか指定した拡張機能に対応しているか確認
    bool CheckExtensionNames(vk::PhysicalDevice physicalDevice, std::vector<std::string> extensionNames);

private:
    std::vector<vk::PhysicalDevice> m_PhysicalDevices;
    vk::Instance                    m_instance;
    QueueFamilySelector             m_QueueFamilySelector;

    std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos(std::set<uint32_t> queues);
};

