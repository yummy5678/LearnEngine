#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>


class DeviceExtension
{
public:
	DeviceExtension();	//�R���X�g���N�^
	~DeviceExtension();	//�f�X�g���N�^

	vk::PhysicalDeviceVulkan13Features* GetCreateDevicePNext();
	vk::PhysicalDeviceFeatures*			GetUseDeviceFeatures();

	//�g���@�\�̃��X�g���󂯎��
	std::vector<const char*> GetEnabledExtensions(vk::PhysicalDevice physicalDevice);

	// �g���@�\��L����
	void UseSwapchain();
	void UseMemoryBudget();
	void UseMemoryPriority();
	void UseDynamicRendering();
	void UseRenderingModeNonSolid();


private:
	std::vector<const char*> m_ExtensionList;
	vk::PhysicalDeviceFeatures m_DeviceFeatures;
	vk::PhysicalDeviceVulkan13Features m_DeviceExtensions;

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);
	// ���͂��������񂪂��邩���ׂ�֐�
	bool CheckHasString(const char* target);
};

