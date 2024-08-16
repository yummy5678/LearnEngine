#include "QueueUtility.h"


QueueFamilySelector::QueueFamilySelector(vk::PhysicalDevice physicalDevice)
{
	m_PhysicalDevice = physicalDevice;
}

QueueFamilySelector::~QueueFamilySelector()
{
}

int QueueFamilySelector::GetGraphicIndex()
{
	if(m_GraphicsFamilyIndex == Number_NoneQueue) SearchGraphicsFamily();
	return m_GraphicsFamilyIndex;
}

int QueueFamilySelector::GetComputeIndex()
{
	if (m_ComputeFamilyIndex == Number_NoneQueue) SearchComputeFamily();
	return m_ComputeFamilyIndex;
}

int QueueFamilySelector::GetTransferIndex()
{
	if (m_TransferFamilyIndex == Number_NoneQueue) SearchTransferFamily();
	return m_TransferFamilyIndex;
}

int QueueFamilySelector::GetPresentationIndex(vk::SurfaceKHR surface)
{
	if (m_PresentationFamilyIndex == Number_NoneQueue) SearchPresentationFamily(surface);
	return m_PresentationFamilyIndex;
}



int QueueFamilySelector::SearchGraphicsFamily()
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. キューファミリーが1つでもキューを持っているか確認する
		// 2. レンダリングができるキューか、AND演算を行い確認する
		if (queueFamilyList[i].queueCount != 0 && 
			queueFamilyList[i].queueFlags & vk::QueueFlagBits::eGraphics)
		{
			index = i;	// キューファミリーが有効であれば、そのインデックスを取得する
			break;
		}
	}

	printf("描画用キューの探索結果： %d\n", index);
	return index;
}

int QueueFamilySelector::SearchComputeFamily()
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. キューファミリーが1つでもキューを持っているか確認する
		// 2. 計算に使用できるキューか、AND演算を行い確認する
		if (queueFamilyList[i].queueCount != 0 &&
			queueFamilyList[i].queueFlags & vk::QueueFlagBits::eCompute)
		{
			index = i;	// キューファミリーが有効であれば、そのインデックスを取得する
			break;
		}
	}

	printf("計算用キューの探索結果： %d\n", index);
	return index;
}

int QueueFamilySelector::SearchTransferFamily()
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. キューファミリーが1つでもキューを持っているか確認する
		// 2. 転送用に使用できるキューか、AND演算を行い確認する
		if (queueFamilyList[i].queueCount != 0 &&
			queueFamilyList[i].queueFlags & vk::QueueFlagBits::eTransfer)
		{
			index = i;	// キューファミリーが有効であれば、そのインデックスを取得する
			break;
		}
	}

	printf("転送用キューの探索結果： %d\n", index);
	return index;
}

int QueueFamilySelector::SearchPresentationFamily(vk::SurfaceKHR surface)
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. キューファミリーを1つでもキューを持っているか確認する
		// 2. 画像の表示機能があるか確認する
		if (queueFamilyList[i].queueCount > 0 && m_PhysicalDevice.getSurfaceSupportKHR(i, surface))
		{
			index = i;	// キューファミリーが有効であれば、そのインデックスを取得する
			break;
		}
	}

	printf("プレゼンテーションキューの探索結果： %d\n", index);
	return index;
}

//std::vector<vk::DeviceQueueCreateInfo> QueueFamilySelector::CreateQueueInfos()
//{
//	//ファミリーインデックス用のセット
//	std::set<int> queueFamilyIndices = { m_GraphicsFamilyIndex, m_PresentationFamilyIndex };
//
//	// キュー作成情報用のベクター
//	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
//
//	// 論理デバイスで作成する必要があるキューとその情報を設定する
//	for (int queueFamilyIndex : queueFamilyIndices)
//	{
//		vk::DeviceQueueCreateInfo queueCreateInfo;
//		queueCreateInfo.pNext;
//		queueCreateInfo.flags;
//		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;	// キューを作成するファミリーのインデックス
//		queueCreateInfo.queueCount = 1;							// 作成するキューの数
//		float priority = 1.0f;									// 優先度
//		queueCreateInfo.pQueuePriorities = &priority;			// Vulkanは複数のキューをどのように扱うかを知る必要があるため、優先度を指定する (1が最高優先度)
//
//		queueCreateInfos.push_back(queueCreateInfo);
//	}
//
//	return queueCreateInfos;
//}

