#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>


class DeviceExtensionCollector
{
public:
	DeviceExtensionCollector();	//�R���X�g���N�^
	~DeviceExtensionCollector();	//�f�X�g���N�^

	//�g���@�\�̃��X�g���󂯎��
	std::set<const char*> GetExtensions(vk::PhysicalDevice physicalDevice);

	//�X���b�v�`�F�[���̊g���@�\��L����
	void UseSwapchain();
	void UseDynamicRendering();


private:
	std::set<const char*> m_ExtensionList;

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);

};

