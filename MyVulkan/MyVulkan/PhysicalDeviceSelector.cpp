#include "PhysicalDeviceSelector.h"

PhysicalDeviceSelector::PhysicalDeviceSelector(vk::Instance instance)
{
    //インスタンスから接続されている物理デバイスを全て取得
    m_PhysicalDevices = instance.enumeratePhysicalDevices();

    //物理デバイス自体を見つけられなかった場合
    if (m_PhysicalDevices.empty()) {
        throw std::runtime_error("VulkanをサポートしているGPUが見つかりません!");
    }
}

vk::PhysicalDevice PhysicalDeviceSelector::selectGraphicsDevice()
{
	// 適切なデバイスが見つかるまでループする
	for (const auto& device : m_PhysicalDevices)
	{
		//デバイスが使用する拡張機能、
		if (CheckDeviceSuitable(extensionManager, device, surface))
		{
			// 適切なデバイスが見つかった
			return device;
		}
	}

	// 利用可能なデバイスがない場合
	throw std::runtime_error("VulkanをサポートするGPUが見つかりません！");
}

vk::PhysicalDevice PhysicalDeviceSelector::selectSwapchainDevice(std::function<bool(const vk::PhysicalDevice&, vk::SurfaceKHR)> criteria)
{

    return vk::PhysicalDevice();
}

bool PhysicalDeviceSelector::CheckGraphics(vk::PhysicalDevice physicalDevice)
{
    // デバイスのプロパティと機能を取得
    vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();
    vk::PhysicalDeviceFeatures deviceFeatures = physicalDevice.getFeatures();

    std::optional<uint32_t> graphicsFamily = findQueueFamilies(physicalDevice);

    // アプリケーションの要件に応じてデバイスが適切かどうかを判断
    return graphicsFamily.has_value() && deviceFeatures.geometryShader;
}

std::optional<uint32_t> PhysicalDeviceSelector::findQueueFamilies(vk::PhysicalDevice device)
{
    // デバイスのキューファミリーを取得
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    std::optional<uint32_t> graphicsFamily;

    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            graphicsFamily = i;
            break;
        }
    }

    return graphicsFamily;
}
