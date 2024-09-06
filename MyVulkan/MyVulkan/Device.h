#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"
#include "PhysicalDeviceSelector.h"
#include "VulkanExtensions.h"
#include "QueueUtility.h"
#include "SwapChainUtility.h"
#include "CommandUtility.h"


class DeviceGenerator : CGeneratorBase
{
public:
	DeviceGenerator();
	~DeviceGenerator();

	void Create(CDeviceExtensionManager extensionManager,vk::Instance instance, vk::SurfaceKHR surface);
	void Destroy();

	vk::PhysicalDevice	GetPhysicalDevice();
	vk::Device			GetLogicalDevice();	

private:
	//�����f�o�C�X
	vk::PhysicalDevice	m_PhysicalDevice;
	//�_���f�o�C�X
	vk::Device			m_LogicalDevice;
	//�L���[
	std::vector<vk::DeviceQueueCreateInfo> m_QueueFamilyGenerator;
	//�R�}���h�o�b�t�@�̍쐬
	CommandGenerator	m_CommandGenerator;

	//�_���f�o�C�X�̍쐬�����쐬
	vk::DeviceCreateInfo CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice physicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos);
};


//�ȉ��f�o�C�X�Ɋւ��郁��
//VkPhysicalDevice�I�u�W�F�N�g�͖����I�ɔj�����邱�Ƃ͂ł��܂���VkInstance�B
//����ɁA�擾���̃I�u�W�F�N�g���j�������ƈÖٓI�ɔj������܂��B

