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

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	//bool CheckExtensionSupport(const char* checkName);
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames);
};








