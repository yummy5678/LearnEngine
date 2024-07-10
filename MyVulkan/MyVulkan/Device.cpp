#include "Device.h"

LogicalDevice::LogicalDevice()
{
}

LogicalDevice::~LogicalDevice()
{
}

void LogicalDevice::create()
{
	// �I�����������f�o�C�X�̃L���[�t�@�~���[�C���f�b�N�X���擾����
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	// �L���[�쐬���p�̃x�N�^�[�ƃt�@�~���[�C���f�b�N�X�p�̃Z�b�g����������
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> queueFamilyIndices = { indices.graphicsFamily, indices.presentationFamily };

	// �_���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;                        // �L���[���쐬����t�@�~���[�̃C���f�b�N�X
		queueCreateInfo.queueCount = 1;                                             // �쐬����L���[�̐�
		float priority = 1.0f;														// �D��x
		queueCreateInfo.pQueuePriorities = &priority;                               // Vulkan�͕����̃L���[���ǂ̂悤�Ɉ�������m��K�v�����邽�߁A�D��x���w�肷�� (1���ō��D��x)

		queueCreateInfos.push_back(queueCreateInfo);
	}

	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // �L���[�쐬���̐�
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g

	// �_���f�o�C�X���g�p���镨���f�o�C�X�̋@�\
	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;            // ���W�J���f�o�C�X���g�p���镨���f�o�C�X�̋@�\

	// �w�肳�ꂽ�����f�o�C�X�ɑ΂��ă��W�J���f�o�C�X���쐬����
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("���W�J���f�o�C�X�̍쐬�Ɏ��s���܂����I");
	}

	// �L���[�̓f�o�C�X�Ɠ����ɍ쐬����邽��
	// �L���[�ւ̃n���h�����擾����
	// �w�肳�ꂽ���W�J���f�o�C�X����A�w�肳�ꂽ�L���[�t�@�~���[�̎w�肳�ꂽ�L���[�C���f�b�N�X�i0�͗B��̃L���[�Ȃ̂�0�j�A�w�肳�ꂽVkQueue�ւ̎Q�Ƃ�u��
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.presentationFamily, 0, &presentationQueue);

}

VkDevice LogicalDevice::get()
{
	return device.get();
}

void LogicalDevice::createLogicalDevice()
{
	//�C���X�^���X���畨���f�o�C�X���擾
	std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : physicalDevices)
	{
		if (checkDeviceSuitable(device))
		{
			// �K�؂ȃf�o�C�X����������
			physicalDevice = device;
			return;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");

}

void LogicalDevice::getPhysicalDevice()
{
	//�C���X�^���X���畨���f�o�C�X���擾
	std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : physicalDevices)
	{
		if (checkDeviceSuitable(device))
		{
			// �K�؂ȃf�o�C�X����������
			physicalDevice = device;
			return;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");
}

bool LogicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	// �f�o�C�X���T�|�[�g����g���@�\�̐����擾����
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	// �����g���@�\��������Ȃ��ꍇ�A���s�Ƃ���false��Ԃ�
	if (extensionCount == 0)
	{
		return false;
	}

	// �g���@�\�̏���ێ����邽�߂�VkExtensionProperties�̃��X�g���쐬����
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, extensions.data());

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�`�F�b�N����
	for (const auto& deviceExtension : deviceExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// �g���@�\������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// �K�v�Ȋg���@�\��������Ȃ��ꍇ��false��Ԃ�
		if (!hasExtension)
		{
			return false;
		}
	}

	// ���ׂĂ̕K�v�Ȋg���@�\�����������ꍇ��true��Ԃ�
	return true;
}

/// <summary>
/// �w�肳�ꂽ�����f�o�C�X�� Vulkan �����_���[�ɓK���Ă��邩�ǂ������m�F����
/// </summary>
bool LogicalDevice::checkDeviceSuitable(VkPhysicalDevice device)
{
	/*
	// �f�o�C�X���̂Ɋւ����� (ID�A���O�A�^�C�v�A�x���_�[�Ȃ�)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// �f�o�C�X�̋@�\�Ɋւ����� (�W�I���g���V�F�[�_�[�A�e�b�Z���[�V�����V�F�[�_�[�A���C�h���C���Ȃ�)
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	*/

	// �L���[�t�@�~���[�̃C���f�b�N�X���擾����
	QueueFamilyIndices indices = getQueueFamilies(device);

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g���Ă��邩�m�F����
	bool extensionsSupported = checkDeviceExtensionSupport(device);

	// Swapchain���L�����ǂ������m�F����
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// ����̕����f�o�C�X�ɑ΂���Swap Chain�̏ڍׂ��擾����
		SwapChainDetails swapChainDetails = getSwapChainDetails(device);

		// Swap Chain�̗L�������m�F����B�v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// �f�o�C�X���K�����Ă��邩���m�F����
	return indices.isValid() && extensionsSupported && swapChainValid;

}
