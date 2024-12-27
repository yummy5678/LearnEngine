#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>


class DeviceExtension
{
public:
	DeviceExtension();	//�R���X�g���N�^
	~DeviceExtension();	//�f�X�g���N�^

	//�g���@�\�̃��X�g���󂯎��
	std::vector<const char*> GetExtensions(vk::PhysicalDevice physicalDevice);

	// �g���@�\��L����
	void UseSwapchain();
	void UseMemoryBudget();
	void UseMemoryPriority();
	void UseDynamicRendering();


private:
	std::vector<const char*> m_ExtensionList;

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);
	// ���͂��������񂪂��邩���ׂ�֐�
	bool CheckHasString(const char* target);
};

