#pragma once
#include <vector>
#include <unordered_map>
#include <string>

enum InstanceLayerList
{
	LAYER_VALIDATION,
	//Synchronization,
	//API_Dump,
	//Device_Simulation,
	//Screenshot
	LAYER_LIST_MAX_NUMBER
};

const std::unordered_map<InstanceLayerList, std::string> gLayerMap
{
	//LAYER_VALIDATION,	"VK_LAYER_LUNARG_standard_validation"
	{ LAYER_VALIDATION, "VK_LAYER_KHRONOS_validation"},

};


class InstanceLayerManager
{
public:
	InstanceLayerManager();
	~InstanceLayerManager();

	void AddLayer(InstanceLayerList layer);
	bool hasLayer(InstanceLayerList layer);
	bool DeleteLayer(InstanceLayerList layer);

	std::vector<const char*>* GetRequiredInstanceExtensionsPointer();

private:

	std::vector<std::string> m_LayerList;





};

// Vulkanのレイヤーとは？
//
//


// レイヤーの種類
//
//






















