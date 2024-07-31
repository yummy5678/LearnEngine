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

	//�_���f�o�C�X�̍쐬
	auto queueCreateInfos = VulkanCreate::GetQueueInfos(m_PhysicalDevice, surface);
	CreateDeviceInfo(&queueCreateInfos);
	m_LogicalDevice = m_PhysicalDevice.createDevice(m_DeviceInfo);
	
	// �I�����������f�o�C�X�̃L���[�t�@�~���[�C���f�b�N�X���擾����
	QueueFamilyIndices queueIndex = getQueueFamilies(m_PhysicalDevice, surface);
	graphicsQueue = m_LogicalDevice.getQueue((uint32_t)queueIndex.graphicsFamily);
	//	 �L���[�̓f�o�C�X�Ɠ����ɍ쐬����邽��
	// �L���[�ւ̃n���h�����擾����
	//�w�肳�ꂽ���W�J���f�o�C�X����A�w�肳�ꂽ�L���[�t�@�~���[�̎w�肳�ꂽ�L���[�C���f�b�N�X�i0�͗B��̃L���[�Ȃ̂�0�j�A�w�肳�ꂽVkQueue�ւ̎Q�Ƃ�u��
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice.get(), queueIndex.presentationFamily, 0, &presentationQueue);
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

void DeviceGenerator::CreateLogicalDevice()
{


}

void DeviceGenerator::CreateDeviceInfo(std::vector<vk::DeviceQueueCreateInfo>* queueCreateInfos)
{
	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	m_DeviceInfo.pNext;
	m_DeviceInfo.flags;
	m_DeviceInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos->size();       // �L���[�쐬���̐�
	m_DeviceInfo.pQueueCreateInfos = queueCreateInfos->data();                    // �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	m_DeviceInfo.enabledLayerCount = 0;
	m_DeviceInfo.ppEnabledLayerNames = nullptr;
	m_DeviceInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();      // �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	m_DeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();              // �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g
	m_DeviceInfo.pEnabledFeatures = nullptr;
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

	// �L���[�t�@�~���[�̃C���f�b�N�X���擾����
	QueueFamilyIndices indices = VulkanUtility::GetQueueFamilies(device, surface);

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g���Ă��邩�m�F����
	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	// Swapchain���L�����ǂ������m�F����
	bool swapChainValid = false;
	if (extensionsSupported)
	{
		// ����̕����f�o�C�X�ɑ΂���Swap Chain�̏ڍׂ��擾����
		SwapChainDetails swapChainDetails = VulkanUtility::getSwapChainDetails(device, surface);

		// Swap Chain�̗L�������m�F����B�v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
		swapChainValid = !swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty();

	}

	// �f�o�C�X���K�����Ă��邩���m�F����
	return indices.isValid() && extensionsSupported && swapChainValid;
}

QueueFamilyIndices DeviceGenerator::getQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	QueueFamilyIndices indices;

	// �����f�o�C�X�ɑ΂��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyList.data());

	// �e�L���[�t�@�~���[�𒲂ׁA�K�v�ȃ^�C�v�̃L���[�����Ȃ��Ƃ�1�����Ă��邩�ǂ������m�F����
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// �܂��A�L���[�t�@�~���[�����Ȃ��Ƃ�1�̃L���[�������Ă��邩�m�F����i�L���[���Ȃ��\��������j
		// �L���[�̓r�b�g�t�B�[���h�ŕ����̃^�C�v���`���邱�Ƃ��ł���BVK_QUEUE_*_BIT�ƃr�b�g���Ƃ�AND���Z���s���A�K�v�ȃ^�C�v�������Ă��邩�m�F����
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
		}

		// �L���[�t�@�~���[���v���[���e�[�V�������T�|�[�g���Ă��邩�m�F����
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationSupport);
		// �L���[���v���[���e�[�V�����^�C�v�ł��邩�ǂ������m�F����i�O���t�B�b�N�X�ƃv���[���e�[�V�����̗����ɂȂ蓾��j
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		// �L���[�t�@�~���[�̃C���f�b�N�X���L���ȏ�Ԃɂ��邩�ǂ������m�F���A�����ł���Ό������~����
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}
