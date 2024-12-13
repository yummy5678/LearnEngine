#include "VulkanLayerManager.h"


InstanceLayerManager::InstanceLayerManager()
{
}

InstanceLayerManager::~InstanceLayerManager()
{
}

void InstanceLayerManager::Add(const char* layerName)
{
	if (hasLayer(layerName) == false && CheckLayerSupport(layerName) == true)
	{
		std::cout << "レイヤー:" << layerName << "が追加されました" << std::endl;
		m_ExtensinList.push_back(layerName);
		return;
	}
	else
	{
		std::cout << "レイヤー:" << layerName << "は確認できませんでした" << std::endl;
	}

}

bool InstanceLayerManager::hasLayer(const char* layerName)
{
	// m_ExtensinListにlayerNameが存在するか確認
	return std::find(m_ExtensinList.begin(), m_ExtensinList.end(), layerName) != m_ExtensinList.end();
}

std::vector<const char*>* InstanceLayerManager::GetList()
{
	return &m_ExtensinList;
}


bool InstanceLayerManager::CheckLayerSupport(const char* layerName)
{
	std::cout << "指定されたレイヤーが利用出来るか確認" << std::endl;

	//利用可能なレイヤーのリストを持ってくる
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	// 指定されたバリデーションレイヤーが利用可能なレイヤーリストに含まれているかチェックする

	for (const auto& availableLayer : availableLayers)
	{
		// レイヤー名が一致するかどうかをstrcmpで比較する
		if (strcmp(layerName, availableLayer.layerName) == 0)
		{
			// すべての指定されたバリデーションレイヤーが見つかった場合はtrueを返す
			std::cout << "成功" << std::endl;
			return true;
		}
	}

	// 指定されたレイヤーを全て見つけられなかった場合はfalseを返す
	std::cout << "指定されたレイヤーを見つけられませんでした" << std::endl;
	return false;
}
