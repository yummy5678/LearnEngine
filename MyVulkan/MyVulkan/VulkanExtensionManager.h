#pragma once
#include <vulkan/vulkan.hpp>
#include <unordered_set>
#include <iostream>

class VulkanExtensionManager
{
};



class InstanceExtensionManager
{
public:
	InstanceExtensionManager();
	~InstanceExtensionManager();

	void Add(const char* extensionName);
	bool Delete(const char* extensionName);

	std::unordered_set<const char*> GetList();


private:

	std::unordered_set<const char*> m_ExtensionList;	//内容の重複を避けたいためsetを使う

	// 引数の拡張機能の名前のものが利用できるか確認
	bool CheckExtensionSupport(const char* checkName);
};








