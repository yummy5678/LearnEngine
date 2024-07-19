#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "VulkanValidation.h"

// �C���X�^���X�g���@�\�̃��X�g���쐬����


namespace VulkanCreate
{
	vk::ApplicationInfo		GetApplicationInfo();
	vk::InstanceCreateInfo	GetInstanceInfo(vk::ApplicationInfo* appInfo = nullptr);
}

namespace VulkanUtility
{
	bool CheckValidationLayerSupport(const std::vector<const char*> validationLayers);
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);

	// GLFW �ŃT�[�t�F�X�����̂ɕK�v�ȃC���X�^���X�g�����擾
	std::vector<const char*>* GetRequiredInstanceExtensionsPointer();

};

