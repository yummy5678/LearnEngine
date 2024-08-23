#include "QueueUtility.h"



QueueFamilySelector::QueueFamilySelector(vk::PhysicalDevice physicalDevice)
{
	std::cout << "QueueFamilySelectorが作成されました" << std::endl;
	m_PhysicalDevice = physicalDevice;
}

QueueFamilySelector::~QueueFamilySelector()
{
}

uint32_t QueueFamilySelector::GetGraphicIndex()
{	
	if(m_GraphicsFamilyIndex == Number_NoneQueue) m_GraphicsFamilyIndex = SearchGraphicsFamily();
	std::cout << "描画ファミリー" << m_GraphicsFamilyIndex << "を返します" << std::endl;
	return m_GraphicsFamilyIndex;
}

uint32_t QueueFamilySelector::GetComputeIndex()
{
	if (m_ComputeFamilyIndex == Number_NoneQueue) m_ComputeFamilyIndex = SearchComputeFamily();
	std::cout << "計算ファミリー" << m_GraphicsFamilyIndex << "を返します" << std::endl;
	return m_ComputeFamilyIndex;
}

uint32_t QueueFamilySelector::GetTransferIndex()
{
	if (m_TransferFamilyIndex == Number_NoneQueue) m_TransferFamilyIndex = SearchTransferFamily();
	std::cout << "転送ファミリー" << m_GraphicsFamilyIndex << "を返します" << std::endl;
	return m_TransferFamilyIndex;
}

uint32_t QueueFamilySelector::GetPresentationIndex(vk::SurfaceKHR surface)
{
	if (m_PresentationFamilyIndex == Number_NoneQueue) m_PresentationFamilyIndex = SearchPresentationFamily(surface);
	std::cout << "表示ファミリー" << m_GraphicsFamilyIndex << "を返します" << std::endl;
	return m_PresentationFamilyIndex;
}



uint32_t QueueFamilySelector::SearchGraphicsFamily()
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	uint32_t index = Number_NoneQueue;

	for (uint32_t i = 0; i < queueFamilyList.size(); i++)
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

uint32_t QueueFamilySelector::SearchComputeFamily()
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	uint32_t index = Number_NoneQueue;

	for (uint32_t i = 0; i < queueFamilyList.size(); i++)
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

uint32_t QueueFamilySelector::SearchTransferFamily()
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	uint32_t index = Number_NoneQueue;

	for (uint32_t i = 0; i < queueFamilyList.size(); i++)
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

uint32_t QueueFamilySelector::SearchPresentationFamily(vk::SurfaceKHR surface)
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	uint32_t index = Number_NoneQueue;

	for (uint32_t i = 0; i < queueFamilyList.size(); i++)
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

