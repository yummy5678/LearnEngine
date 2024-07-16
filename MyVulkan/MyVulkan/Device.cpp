#include "Device.h"

vk::PhysicalDevice DeviceUtility::getPhysicalDevice(vk::Instance& instance, vk::SurfaceKHR surface)
{

	vk::PhysicalDevice physicalDevice;	//�����f�o�C�X

	//�C���X�^���X���畨���f�o�C�X���擾
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : physicalDevices)
	{
		if (checkDeviceSuitable(device, surface))
		{
			// �K�؂ȃf�o�C�X����������
			return device;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");
	return vk::PhysicalDevice();
}


/// <summary>
/// �f�o�C�X���K�v�Ȋg���@�\�����邩�m�F����
/// </summary>
bool DeviceUtility::checkDeviceExtensionSupport(vk::PhysicalDevice device)
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




bool DeviceUtility::checkDeviceSuitable(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
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
	QueueFamilyIndices indices = QueueUtility::getQueueFamilies(physicalDevice,surface);

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g���Ă��邩�m�F����
	bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

	// Swapchain���L�����ǂ������m�F����
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// ����̕����f�o�C�X�ɑ΂���Swap Chain�̏ڍׂ��擾����
		SwapChainDetails swapChainDetails = SwapChainUtility::getSwapChainDetails(physicalDevice, surface);

		// Swap Chain�̗L�������m�F����B�v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// �f�o�C�X���K�����Ă��邩���m�F����
	return indices.isValid() && extensionsSupported && swapChainValid;
}


/// <summary>
/// �_���f�o�C�X�̍쐬
/// </summary>
vk::UniqueDevice DeviceUtility::createLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	//�_���f�o�C�X�̍쐬�ɕK�v�Ȃ���
	//1,�g�p����f�o�C�X�̊g��
	//2,�g�p����f�o�C�X�̃��C���[
	//3,�f�o�C�X�̂ǂ̃L���[���g�p���邩

	//�v�Z�v�����󂯕t����L���[��T��
	auto queueCreateInfos = QueueUtility::getQueueInfos(physicalDevice, surface);

	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	vk::DeviceCreateInfo deviceCreateInfo = {};
	//�L���[�̐ݒ�
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());       // �L���[�쐬���̐�
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();                                 // �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	//�g���@�\�̐ݒ�
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());      // �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();                           // �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g

	// �w�肳�ꂽ�����f�o�C�X�ɑ΂��ă��W�J���f�o�C�X���쐬����
	vk::UniqueDevice logicalDevice = physicalDevice.createDeviceUnique(deviceCreateInfo);
	if (!logicalDevice)
	{
		throw std::runtime_error("���W�J���f�o�C�X�̍쐬�Ɏ��s���܂����I");
	}

	return logicalDevice;
}

