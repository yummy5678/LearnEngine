#include "QueueUtility.h"

QueueFamilyIndices QueueUtility::getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	// 物理デバイスに対するすべてのキューファミリープロパティ情報を取得する
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	// 各キューファミリーを調べ、必要なタイプのキューを少なくとも1つ持っているかどうかを確認する
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// まず、キューファミリーが少なくとも1つのキューを持っているか確認する（キューがない可能性もある）
		// キューはビットフィールドで複数のタイプを定義することができる。VK_QUEUE_*_BITとビットごとのAND演算を行い、必要なタイプを持っているか確認する
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// キューファミリーが有効であれば、そのインデックスを取得する
		}

		// キューファミリーがプレゼンテーションをサポートしているか確認する
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
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
