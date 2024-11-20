#include "Device.h"

DeviceGenerator::DeviceGenerator()
{
}

DeviceGenerator::~DeviceGenerator()
{
}

void DeviceGenerator::Create(DeviceExtension extensionManager, vk::Instance instance)
{
	m_bCreated = true;

	//�g�p�\�ȕ����f�o�C�X��T���Ă���
	PhysicalDeviceSelector physicalSelector(instance);

	// �X���b�v�`�F�C���ɑΉ����������f�o�C�X��T��
	// �����ł̓T�[�t�F�X�Ƃ̑����ɂ��Ă͌��Ȃ�
	auto selectorResult = physicalSelector.SelectGraphicsDevice();
	m_PhysicalDevice = selectorResult.Handle;

	//�_���f�o�C�X�̍쐬
	//�f�o�C�X�̍쐬���ɂǂ�ȃL���[���g�p���邩���߂�
	auto queueInfo = selectorResult.QueueInfo;
	auto logicalDeviceInfo = CreateDeviceInfo(extensionManager, m_PhysicalDevice, queueInfo);
	m_LogicalDevice = m_PhysicalDevice.createDevice(logicalDeviceInfo);


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

vk::DeviceCreateInfo DeviceGenerator::CreateDeviceInfo(DeviceExtension& extensionManager, vk::PhysicalDevice physicalDevice, std::vector<vk::DeviceQueueCreateInfo>& queueCreateInfos)
{
	auto extension = extensionManager.GetExtensions(physicalDevice);

	// �_���f�o�C�X���쐬���邽�߂̏���ݒ肷��
	vk::DeviceCreateInfo deviceInfo;
	deviceInfo.pNext;
	deviceInfo.flags;
	deviceInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();	// �L���[�쐬���̐�
	deviceInfo.pQueueCreateInfos = queueCreateInfos.data();					// �f�o�C�X���K�v�Ƃ���L���[���쐬���邽�߂̃L���[�쐬���̃��X�g
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = nullptr;
	deviceInfo.enabledExtensionCount = (uint32_t)extension.size();			// �L���ȃ��W�J���f�o�C�X�g���@�\�̐�
	deviceInfo.ppEnabledExtensionNames = &*extension.begin();					// �L���ȃ��W�J���f�o�C�X�g���@�\�̃��X�g
	deviceInfo.pEnabledFeatures = nullptr;

	return deviceInfo;
}

