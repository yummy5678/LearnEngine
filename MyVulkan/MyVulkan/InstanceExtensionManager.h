#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "NonCopyable.h"
#include "GraphicsDefine.h"

class InstanceExtension : public NonCopyable
{
public:
	InstanceExtension();
	~InstanceExtension();

	std::vector<const char*>* GetExtensions();
	std::vector<const char*>* GetValidationLayers();

	void UseGLFW();
	void UseValidation();

private:
	bool m_bGLFW = false;

	std::vector<const char*> m_ExtensionList;
	std::vector<const char*> m_ValidationLayers;
	void CreateGLFWExtension();
	void CreateValidationExtension();

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	//bool CheckExtensionSupport(const char* checkName);
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames);
};








