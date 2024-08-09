#include "VulkanLayerManager.h"



InstanceLayerManager::InstanceLayerManager()
{
}

InstanceLayerManager::~InstanceLayerManager()
{
}

void InstanceLayerManager::AddLayer(InstanceLayerList layer)
{
	std::cout << "レイヤーのリストに" << gLayerMap.at(layer) << "が追加されました" << std::endl;
	m_LayerList.push_back(gLayerMap.at(layer));
}

std::vector<const char*> InstanceLayerManager::GetLayerList()
{
	return m_LayerList;
}
