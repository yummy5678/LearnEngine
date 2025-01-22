#include "Device.h"
#include "GraphicsDefine.h"


DeviceGenerator::DeviceGenerator()
{
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(DeviceExtension extensionManager, vk::Instance instance)
{
	m_bCreated = true;

	// �g�p�\�ȕ����f�o�C�X(GPU)��T���Ă���
	PhysicalDeviceSelector physicalSelector(instance);

	// �X���b�v�`�F�C���ɑΉ����������f�o�C�X��T��
	//�� �����ł̓T�[�t�F�X�Ƃ̑����ɂ��Ă͌��Ȃ�
	PhysicalDeviceContainer selectDevice = physicalSelector.SelectGraphicsDevice();
	m_PhysicalDevice = selectDevice.Handle;
	std::vector<vk::DeviceQueueCreateInfo> queueInfos = selectDevice.QueueInfo;

	auto pNext		= extensionManager.GetCreateDevicePNext();
	auto extension	= extensionManager.GetEnabledExtensions(m_PhysicalDevice);

	if (VulkanDefine.LogMessageEnabled == true)
	{
		std::printf("�ȉ��̃f�o�C�X�G�N�X�e���V�������g�p���܂�\n");
		for (size_t i = 0; i < extension.size(); i++)
		{
			std::cout << extension[i] << std::endl;
		}
	}

	// �_���f�o�C�X�ɐݒ肷������쐬
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext = pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)queueInfos.size();	// �L���[�쐬���̐�
	deviceInfo.pQueueCreateInfos = queueInfos.data();					// �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)extension.size();			// �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceInfo.ppEnabledExtensionNames = extension.data();					// �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g
	deviceInfo.pEnabledFeatures = nullptr;

	//�_���f�o�C�X�̍쐬
	m_LogicalDevice = m_PhysicalDevice.createDevice(deviceInfo);


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

