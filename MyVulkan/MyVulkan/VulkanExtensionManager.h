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

	std::unordered_set<const char*> m_ExtensionList;	//���e�̏d���������������set���g��

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	bool CheckExtensionSupport(const char* checkName);
};








