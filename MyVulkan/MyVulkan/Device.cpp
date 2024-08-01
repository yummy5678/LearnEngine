#include "Device.h"


DeviceGenerator::DeviceGenerator()
{
	m_ClassName = "DeviceGenerator";
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(vk::Instance instance, vk::SurfaceKHR surface)
{
	m_bCreated = true;

	//�g�p�\�ȕ����f�o�C�X��T���Ă���
	m_PhysicalDevice = BringPhysicalDevice(instance, surface);

	queueFamilyGenerator.Create(m_PhysicalDevice, surface);

	//�_���f�o�C�X�̍쐬
	//�f�o�C�X�̍쐬���ɂǂ�ȃL���[���g�p���邩���߂�
	m_DeviceInfo = CreateDeviceInfo(&queueFamilyGenerator.GetQueueInfos());
	m_LogicalDevice = m_PhysicalDevice.createDevice(m_DeviceInfo);

}

void DeviceGenerator::Destroy()
{
	vkDestroyDevice(m_LogicalDevice, nullptr);
}

vk::PhysicalDevice DeviceGenerator::GetPhysicalDevice()
{
	CheckCreated();
	return m_PhysicalDevice;
}

vk::Device DeviceGenerator::GetLogicalDevice()
{
	CheckCreated();
	return m_LogicalDevice;
}

vk::Queue DeviceGenerator::GetGraphicsQueue()
{
	CheckCreated();
	return queueFamilyGenerator.GetGraphics(m_LogicalDevice);
}

vk::Queue DeviceGenerator::GetPresentationQueue()
{
	CheckCreated();
	return queueFamilyGenerator.GetPresentation(m_LogicalDevice);
}

vk::PhysicalDevice DeviceGenerator::BringPhysicalDevice(vk::Instance instance, vk::SurfaceKHR surface)
{
	//�C���X�^���X���畨���f�o�C�X(GPU)��S�Ď擾
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : physicalDevices)
	{
		if (CheckDeviceSuitable(device, surface))
		{
			// �K�؂ȃf�o�C�X����������
			return device;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");
}

vk::DeviceCreateInfo DeviceGenerator::CreateDeviceInfo(std::vector<vk::DeviceQueueCreateInfo>* m_QueueCreateInfos)
{
	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)m_QueueCreateInfos->size();       // �L���[�쐬���̐�
	deviceInfo.pQueueCreateInfos = m_QueueCreateInfos->data();                    // �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();      // �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();              // �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g
	deviceInfo.pEnabledFeatures = nullptr;

	return deviceInfo;
}

bool DeviceGenerator::CheckDeviceExtensionSupport(vk::PhysicalDevice device)
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

bool DeviceGenerator::CheckDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
	/*
	// �f�o�C�X���̂Ɋւ����� (ID�A���O�A�^�C�v�A�x���_�[�Ȃ�)
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// �f�o�C�X�̋@�\�Ɋւ����� (�W�I���g���V�F�[�_�[�A�e�b�Z���[�V�����V�F�[�_�[�A���C�h���C���Ȃ�)
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	*/


	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g���Ă��邩�m�F����
	if(CheckDeviceExtensionSupport(device) == false) return false;	//�g���@�\���T�|�[�g���Ă��Ȃ������I

	bool result = false;
	// ����̕����f�o�C�X�ɑ΂���X���b�v�`�F�C���̏ڍׂ��擾����
	SwapChainDetails swapChainDetails = VulkanUtility::getSwapChainDetails(device, surface);
	// �X���b�v�`�F�C���̗L�������m�F����B�v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
	result = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	// �g���@�\�ɑΉ����Ă��ăX���b�v�`�F�C�����L��
	return result;
}
