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

PhysicalDeviceContainer PhysicalDeviceSelector::SelectGraphicsDevice()
{
	// 適切なデバイスが見つかるまでループする
	for (auto& device : m_PhysicalDevices)
	{
        QueueFamilySelector queueFamily(&device);
		//デバイスが使用する拡張機能、
		if (queueFamily.GetGraphicIndex() != NoneQueueNumber)
		{
			// 適切なデバイスが見つかった
            return { device, CreateQueueInfos({queueFamily.GetGraphicIndex()}) };
		}
	}

	// 利用可能なデバイスがない場合
	throw std::runtime_error("描画用に使用できるGPUが見つかりません！");
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectComputeDevice()
{
    // 適切なデバイスが見つかるまでループする
    for (auto& device : m_PhysicalDevices)
    {
        QueueFamilySelector queueFamily(&device);
        if (queueFamily.GetComputeIndex() != NoneQueueNumber)
        {
            // 適切なデバイスが見つかった
            return { device, CreateQueueInfos({queueFamily.GetComputeIndex()}) };
        }
    }

    // 利用可能なデバイスがない場合
    throw std::runtime_error("計算用に使用できるGPUが見つかりません！");
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectTransferDevice()
{
    // 適切なデバイスが見つかるまでループする
    for (auto& device : m_PhysicalDevices)
    {
        QueueFamilySelector queueFamily(&device);
        if (queueFamily.GetTransferIndex() != NoneQueueNumber)
        {
            // 適切なデバイスが見つかった
            return { device, CreateQueueInfos({queueFamily.GetComputeIndex()}) };
        }
    }

    // 利用可能なデバイスがない場合
    throw std::runtime_error("データ転送用に使用できるGPUが見つかりません！");
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectSwapchainDevice()
{
    // 適切なデバイスが見つかるまでループする
    for (auto& device : m_PhysicalDevices)
    {
        // 描画用キューと表示用キューが存在し、
        // 拡張機能にも対応している
        QueueFamilySelector queueFamily(&device);
        if (queueFamily.GetGraphicIndex()               != NoneQueueNumber &&
            //queueFamily.GetPresentationIndex(surface)   != NoneQueueNumber &&
            //CheckSupportSurface(device,surface) == true                     &&
            CheckExtensionNames(device, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }) == true)
        {
            // 適切なデバイスが見つかった
            //return { device, CreateQueueInfos({queueFamily.GetGraphicIndex(), queueFamily.GetPresentationIndex(surface)}) };
            return { device, CreateQueueInfos({queueFamily.GetGraphicIndex()}) };
        }
    }

    // 利用可能なデバイスがない場合
    throw std::runtime_error("スワップチェーン用に使用できるGPUが見つかりません！");
}

std::vector<vk::DeviceQueueCreateInfo> PhysicalDeviceSelector::CreateQueueInfos(std::set<uint32_t> queueFamilyIndices)
{
       	// キュー作成情報用のベクター
    	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    
    	// 論理デバイスで作成する必要があるキューとその情報を設定する
    	for (int queueFamilyIndex : queueFamilyIndices)
    	{
    		vk::DeviceQueueCreateInfo queueCreateInfo;
    		queueCreateInfo.pNext;
    		queueCreateInfo.flags;
    		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;	// キューを作成するファミリーのインデックス
    		queueCreateInfo.queueCount = 1;							// 作成するキューの数(1つだけでいい)
    		float priority = 1.0f;									// 優先度
    		queueCreateInfo.pQueuePriorities = &priority;			// Vulkanは複数のキューをどのように扱うかを知る必要があるため、優先度を指定する (1が最高優先度)
    
    		queueCreateInfos.push_back(queueCreateInfo);
    	}
    
    	return queueCreateInfos;
}

bool PhysicalDeviceSelector::CheckSupportSurface(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return !physicalDevice.getSurfaceFormatsKHR(surface).empty() ||
        !physicalDevice.getSurfacePresentModesKHR(surface).empty();
}

bool PhysicalDeviceSelector::CheckExtensionNames(vk::PhysicalDevice physicalDevice, std::vector<std::string> extensionNames)
{
    // 物理デバイスがサポートしている拡張機能を取得
    auto availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

    // 拡張機能の名前をセットにして、存在確認を効率化
    std::set<std::string> availableExtensionsSet;
    for (const auto& extension : availableExtensions) {
        availableExtensionsSet.insert(extension.extensionName);
    }

    // 必要な拡張機能がサポートされているか確認
    for (const auto& extensionName : extensionNames) {
        if (availableExtensionsSet.find(extensionName) == availableExtensionsSet.end()) {
            return false; // 必要な拡張機能が見つからない
        }
    }

    return true; // 全ての必要な拡張機能がサポートされている
}

