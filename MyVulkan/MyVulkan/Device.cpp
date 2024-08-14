#include "Device.h"


DeviceGenerator::DeviceGenerator()
{
	m_ClassName = "DeviceGenerator";
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface)
{
	m_bCreated = true;

	//�g�p�\�ȕ����f�o�C�X��T���Ă���
	m_PhysicalDevice = BringPhysicalDevice(extensionManager,instance, surface);

	m_QueueFamilyGenerator.Create(m_PhysicalDevice, surface);

	//�_���f�o�C�X�̍쐬
	//�f�o�C�X�̍쐬���ɂǂ�ȃL���[���g�p���邩���߂�
	auto queueInfos = m_QueueFamilyGenerator.GetQueueInfos();
	m_DeviceInfo = CreateDeviceInfo(extensionManager, m_PhysicalDevice, queueInfos);
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
	return m_LogicalDevice.getQueue(m_QueueFamilyGenerator.GetGraphicIndex(), 0u);
}

vk::Queue DeviceGenerator::GetPresentationQueue()
{
	CheckCreated();
	return m_LogicalDevice.getQueue(m_QueueFamilyGenerator.GetPresentationIndex(), 0u);
}

int DeviceGenerator::GetQueueIndex()
{
	return m_QueueFamilyGenerator.GetGraphicIndex();//�����ł̓O���t�B�b�N�X�L���[��n���Ă���(��ŏC��)
}

vk::PhysicalDevice DeviceGenerator::BringPhysicalDevice(CDeviceExtensionManager extensionManager, vk::Instance instance, vk::SurfaceKHR surface)
{
	//�C���X�^���X���畨���f�o�C�X(GPU)��S�Ď擾
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : physicalDevices)
	{
		if (CheckDeviceSuitable(extensionManager,device, surface))
		{
			// �K�؂ȃf�o�C�X����������
			return device;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");
}

vk::DeviceCreateInfo DeviceGenerator::CreateDeviceInfo(CDeviceExtensionManager& extensionManager, vk::PhysicalDevice phygicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos)
{
	auto extension = extensionManager.GetExtensions(phygicalDevice);

	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();	// �L���[�쐬���̐�
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();					// �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)extension->size();			// �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceInfo.ppEnabledExtensionNames = extension->data();					// �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g
	deviceInfo.pEnabledFeatures = nullptr;

	return deviceInfo;
}

//bool DeviceGenerator::CheckDeviceExtensionSupport(vk::PhysicalDevice physicalDevice)
//{
//	// �f�o�C�X���T�|�[�g����g���@�\�̐����擾����
//	uint32_t extensionCount = 0;
//	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
//
//	// �����g���@�\��������Ȃ��ꍇ�A���s�Ƃ���false��Ԃ�
//	if (extensionCount == 0)
//	{
//		return false;
//	}
//
//	// �g���@�\�̏���ێ����邽�߂�VkExtensionProperties�̃��X�g���쐬����
//	std::vector<VkExtensionProperties> extensions(extensionCount);
//	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());
//
//	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�`�F�b�N����
//	for (const auto& deviceExtension : deviceExtensions)
//	{
//		bool hasExtension = false;
//		for (const auto& extension : extensions)
//		{
//			// �g���@�\������v���邩�ǂ�����strcmp�Ŕ�r����
//			if (strcmp(deviceExtension, extension.extensionName) == 0)
//			{
//				hasExtension = true;
//				break;
//			}
//		}
//
//		// �K�v�Ȋg���@�\��������Ȃ��ꍇ��false��Ԃ�
//		if (!hasExtension)
//		{
//			return false;
//		}
//	}
//
//	// ���ׂĂ̕K�v�Ȋg���@�\�����������ꍇ��true��Ԃ�
//	return true;
//}

bool DeviceGenerator::CheckDeviceSuitable(CDeviceExtensionManager extensionManager, vk::PhysicalDevice device, vk::SurfaceKHR surface)
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
	if(extensionManager.GetExtensions(device) == nullptr) return false;	//�g���@�\���T�|�[�g���Ă��Ȃ������I

	// ����̕����f�o�C�X�ɑ΂���X���b�v�`�F�C���̏ڍׂ��擾����
	SwapChainDetails swapChainDetails = GetSwapChainDetails(device, surface);


	// �v���[���e�[�V�������[�h����łȂ��A�t�H�[�}�b�g����łȂ��ꍇ�ɗL���Ƃ݂Ȃ��B
	// �g���@�\�ɑΉ����Ă��ăX���b�v�`�F�C�����L��
	return (!swapChainDetails.presentationModes.empty() && !swapChainDetails.formats.empty());
}

SwapChainDetails DeviceGenerator::GetSwapChainDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	SwapChainDetails swapChainDetails;

	// -- CAPABILITIES --
	// ����̕����f�o�C�X�ƃT�[�t�F�X�ɑ΂���\�ʂ̃L���p�r���e�B���擾����
	swapChainDetails.surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

	// -- FORMATS --
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// �t�H�[�}�b�g���Ԃ��ꂽ�ꍇ�A�t�H�[�}�b�g�̃��X�g���擾����
	if (formatCount != 0)
	{
		swapChainDetails.formats.resize(formatCount);
		swapChainDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
	}

	// -- PRESENTATION MODES --
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// �v���[���e�[�V�������[�h���Ԃ��ꂽ�ꍇ�A�v���[���e�[�V�������[�h�̃��X�g���擾����
	if (presentationCount != 0)
	{
		swapChainDetails.presentationModes.resize(presentationCount);
		swapChainDetails.presentationModes = physicalDevice.getSurfacePresentModesKHR(surface);
	}

	return swapChainDetails;
}
