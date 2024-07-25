#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"
#include "QueueUtility.h"
#include "SwapChainUtility.h"

//�g�p����g���@�\
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class DeviceGenerator : CGeneratorBase
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(vk::Instance instance, vk::SurfaceKHR surface);

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();
	

private:
	vk::PhysicalDevice	m_PhysicalDevice;
	vk::UniqueDevice	m_LogicalDevice;

	//�_���f�o�C�X�̍쐬���
	vk::DeviceCreateInfo m_DeviceInfo;

	//�����f�o�C�X�̎擾
	vk::PhysicalDevice BringPhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface);

	//�_���f�o�C�X�̍쐬�����쐬
	void CreateDeviceInfo(std::vector< vk::DeviceQueueCreateInfo >* queueCreateInfos);
	//�_���f�o�C�X�̍쐬
	void CreateLogicalDevice();



	//�g���@�\���g���邩�̊m�F
	bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);
	bool CheckDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface);


};