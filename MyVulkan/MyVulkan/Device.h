#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "NonCopyable.h"
#include "PhysicalDeviceSelector.h"
#include "DeviceExtensionManager.h"
#include "QueueUtility.h"


class DeviceGenerator : NonCopyable
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(DeviceExtension extensionManager,vk::Instance instance);
	void Destroy();

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();	

private:
	//�����f�o�C�X
	vk::PhysicalDevice	m_PhysicalDevice;
	//�_���f�o�C�X
	vk::Device			m_LogicalDevice;
	//�L���[
	std::vector<vk::DeviceQueueCreateInfo> m_Queue;

};


//�ȉ��f�o�C�X�Ɋւ��郁��
//VkPhysicalDevice�I�u�W�F�N�g�͖����I�ɔj�����邱�Ƃ͂ł��܂���VkInstance�B
//����ɁA�擾���̃I�u�W�F�N�g���j�������ƈÖٓI�ɔj������܂��B

