#include "QueueUtility.h"


QueueFamilyGenerator::QueueFamilyGenerator()
{
	m_ClassName = "QueueFamilyGenerator";
}

QueueFamilyGenerator::~QueueFamilyGenerator()
{
}

void QueueFamilyGenerator::Create(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	m_bCreated = true;	//作成関数を通ったフラグを立てる
	m_GraphicsFamilyIndex = SearchGraphicsFamily(physicalDevice, surface);	// キューファミリーが有効であれば、そのインデックスを取得する

	m_PresentationFamilyIndex = SearchPresentationFamily(physicalDevice, surface);




	m_QueueCreateInfos = CreateQueueInfos(physicalDevice, surface);
}

int QueueFamilyGenerator::GetGraphicIndex()
{
	CheckCreated();	//作成関数を通す前に使用するとエラー
	return m_GraphicsFamilyIndex;
}

int QueueFamilyGenerator::GetPresentationIndex()
{
	CheckCreated();	//作成関数を通す前に使用するとエラー
	return m_PresentationFamilyIndex;
}

std::vector<vk::DeviceQueueCreateInfo> QueueFamilyGenerator::GetQueueInfos()
{
	CheckCreated();	//作成関数を通す前に使用するとエラー
	return m_QueueCreateInfos;
}

int QueueFamilyGenerator::SearchGraphicsFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = physicalDevice.getQueueFamilyProperties();
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

	printf("グラフィックスキューの探索結果： %d\n", index);
	return index;

}

int QueueFamilyGenerator::SearchPresentationFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	// 物理デバイスに備わっているすべてのキューファミリープロパティ情報を取得する
	const auto queueFamilyList = physicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. キューファミリーが1つでもキューを持っているか確認する
		// 2. 画像の表示機能があるか確認する
		if (queueFamilyList[i].queueCount != 0 && physicalDevice.getSurfaceSupportKHR(i, surface))
		{
			index = i;	// キューファミリーが有効であれば、そのインデックスを取得する
			break;
		}
	}

	printf("プレゼンテーションキューの探索結果： %d\n", index);
	return index;
}

std::vector<vk::DeviceQueueCreateInfo> QueueFamilyGenerator::CreateQueueInfos(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	//ファミリーインデックス用のセット
	std::set<int> queueFamilyIndices = { m_GraphicsFamilyIndex, m_PresentationFamilyIndex };

	// キュー作成情報用のベクター
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	// 論理デバイスで作成する必要があるキューとその情報を設定する
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo;
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

