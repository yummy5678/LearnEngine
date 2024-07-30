#include "QueueUtility.h"


/// <summary>
/// キューファミリーのインデックスを取得する
/// </summary>
QueueFamilyIndices VulkanUtility::GetQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	QueueFamilyIndices indices;

	// 物理デバイスに対するすべてのキューファミリープロパティ情報を取得する
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyList.data());

	// 各キューファミリーを調べ、必要なタイプのキューを少なくとも1つ持っているかどうかを確認する
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// キューファミリーが少なくとも1つのキューを持っているか確認する（キューがない可能性もある）
		// キューはビットフィールドで複数のタイプを定義することができる。
		// VK_QUEUE_*_BITとビットごとのAND演算を行い、必要なタイプを持っているか確認する
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// キューファミリーが有効であれば、そのインデックスを取得する
		}

		// キューファミリーがプレゼンテーションをサポートしているか確認する
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationSupport);
		// キューがプレゼンテーションタイプであるかどうかを確認する（グラフィックスとプレゼンテーションの両方になり得る）
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		// キューファミリーのインデックスが有効な状態にあるかどうかを確認し、そうであれば検索を停止する
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}

std::vector<vk::DeviceQueueCreateInfo> VulkanCreate::GetQueueInfos(vk::PhysicalDevice physicalDevice,VkSurfaceKHR surface)
{
	// 選択した物理デバイスのキューファミリーインデックスを取得する
	QueueFamilyIndices queueIndex = VulkanUtility::GetQueueFamilies(physicalDevice, surface);

	// キュー作成情報用のベクター
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	//ファミリーインデックス用のセット
	std::set<int> queueFamilyIndices = { queueIndex.graphicsFamily, queueIndex.presentationFamily };

	// 論理デバイスで作成する必要があるキューとその情報を設定する
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.pNext;
		queueCreateInfo.flags;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;	// キューを作成するファミリーのインデックス
		queueCreateInfo.queueCount = 1;							// 作成するキューの数
		float priority = 1.0f;									// 優先度
		queueCreateInfo.pQueuePriorities = &priority;			// Vulkanは複数のキューをどのように扱うかを知る必要があるため、優先度を指定する (1が最高優先度)

		queueCreateInfos.push_back(queueCreateInfo);
	}

	return queueCreateInfos;
}

