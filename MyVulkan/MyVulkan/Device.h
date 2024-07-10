#pragma once
#include <vulkan/vulkan.hpp>
#include "Utilities.h"


class LogicalDevice
{
public:
	LogicalDevice();
	~LogicalDevice();

	void create();
	//void draw();

	VkDevice get();

private:
	vk::UniqueInstance instance;

	//�f�o�C�X
	VkPhysicalDevice physicalDevice;	//�����f�o�C�X(�����̍\���̂Ȃ̂Ŕj���̕K�v�͂Ȃ�)
	vk::UniqueDevice device;		//�_���f�o�C�X

	//�L���[
	VkQueue graphicsQueue;
	VkQueue presentationQueue;

private:
	//�f�o�C�X�̍쐬
	//void createDebugCallback();
	void createLogicalDevice();

	// - Get Functions
	void getPhysicalDevice();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);

};

