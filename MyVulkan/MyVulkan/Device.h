#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"
#include "VulkanExtensions.h"
#include "QueueUtility.h"
#include "SwapChainUtility.h"


class DeviceGenerator : CGeneratorBase
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(CDeviceExtensionManager extensionManager,vk::Instance instance, vk::SurfaceKHR surface);
	void Destroy();

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();

	vk::Queue			GetGraphicsQueue();
	vk::Queue			GetPresentationQueue();
	int					GetQueueIndex();
	

private:
	vk::PhysicalDevice	m_PhysicalDevice;
	vk::Device			m_LogicalDevice;

	//�_���f�o�C�X�̍쐬���
	vk::DeviceCreateInfo m_DeviceInfo;

	//�L���[
	QueueFamilyGenerator m_QueueFamilyGenerator;


	//�����f�o�C�X�̎擾
	vk::PhysicalDevice pickPhysicalDevice(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface);

	//�_���f�o�C�X�̍쐬�����쐬
	vk::DeviceCreateInfo CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice phygicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos);

	//�g���@�\���g���邩�̊m�F
	//bool CheckDeviceExtensionSupport(vk::PhysicalDevice physicalDevice);
	bool CheckDeviceSuitable(CDeviceExtensionManager extensionManager, vk::PhysicalDevice phygicalDevice, vk::SurfaceKHR surface);

	//QueueFamilyIndices getQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);


	SwapChainDetails GetSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
};


//�ȉ��f�o�C�X�Ɋւ��郁��
//VkPhysicalDevice�I�u�W�F�N�g�͖����I�ɔj�����邱�Ƃ͂ł��܂���VkInstance�B
//����ɁA�擾���̃I�u�W�F�N�g���j�������ƈÖٓI�ɔj������܂��B

