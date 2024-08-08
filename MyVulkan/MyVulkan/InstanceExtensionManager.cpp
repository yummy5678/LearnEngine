#include "InstanceExtensionManager.h"
#include <stdexcept>

InstanceLayerManager::InstanceLayerManager()
{
}

InstanceLayerManager::~InstanceLayerManager()
{
}

void InstanceLayerManager::AddLayer(InstanceLayerList layer)
{
	m_LayerList.push_back(gLayerMap.at(layer));
}
