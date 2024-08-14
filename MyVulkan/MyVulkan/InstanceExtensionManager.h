#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GraphicsDefine.h"

class InstanceExtensionManager
{
public:
	InstanceExtensionManager();
	~InstanceExtensionManager();

	std::vector<const char*>* GetExtensions();

	void UseGLFW();


private:
	bool m_bGLFW = false;

	std::vector<const char*> m_ExtensionList;

	void CreateGLFWExtension();
	void CreateValidationExtension();

	// 引数の拡張機能の名前のものが利用できるか確認
	//bool CheckExtensionSupport(const char* checkName);
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames);
};








