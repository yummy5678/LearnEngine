#pragma once
#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>


class DeviceExtensionManager
{
public:
	DeviceExtensionManager();	//�R���X�g���N�^
	~DeviceExtensionManager();	//�f�X�g���N�^

	//�g���@�\�̃��X�g���󂯎��
	std::vector<const char*>* GetExtensions(vk::PhysicalDevice physicalDevice);

	//�X���b�v�`�F�[���̊g���@�\��L����
	void UseSwapchain();


private:
	bool m_bSwapchain = false;	//�X���b�v�`�F�[���̊Ǘ��t���O

	std::vector<const char*> m_ExtensionList;

	//�X���b�v�`�F�[���̊g���@�\�����X�g�ɒǉ�
	void CreateSwapChainExtension();

	// �����̊g���@�\�̖��O�̂��̂����p�ł��邩�m�F
	bool CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice);

};

