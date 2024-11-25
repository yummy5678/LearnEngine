#include "VulkanInitializer.h"


VulkanInitializer::VulkanInitializer() :
	callback(),
	m_InstanceExtension(),
	m_DeviceExtension(),
	m_VmaAllocator()
{
}

VulkanInitializer::~VulkanInitializer()
{
}


int VulkanInitializer::init()
{	
	// �I�u�W�F�N�g
	InstanceGenerator	instanceCreator;
	DeviceGenerator		deviceCreator;


	try {
		//�C���X�^���X�̍쐬
		instanceCreator.Create(m_InstanceExtension);
		m_Instance = instanceCreator.GetInstanse();

		createDebugCallback();

		//�����E�_���f�o�C�X�̍쐬
		deviceCreator.Create(m_DeviceExtension, m_Instance);

		//�����f�o�C�X���擾
		m_PhysicalDevice = deviceCreator.GetPhysicalDevice();

		//�_���f�o�C�X���擾
		m_LogicalDevice	= deviceCreator.GetLogicalDevice();

		// �A���P�[�^�[�̍쐬
		CreateAllocator(m_Instance, m_LogicalDevice, m_PhysicalDevice);

		// Create our default "no texture" texture
		//createTexture("plain.png");
	}
	catch (const std::runtime_error& e) {
		//�G���[���b�Z�[�W�󂯎��
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

vk::Instance VulkanInitializer::GetInstance()
{
	return m_Instance;
}

vk::Device VulkanInitializer::GetLogicalDevice()
{
	return m_LogicalDevice;
}

vk::PhysicalDevice VulkanInitializer::GetPhysicalDevice()
{
	return m_PhysicalDevice;
}

VmaAllocator* VulkanInitializer::GetPVmaAllocator()
{
	return &m_VmaAllocator;
}

InstanceExtension* VulkanInitializer::GetPInstanceExtension()
{
	return &m_InstanceExtension;
}

DeviceExtension* VulkanInitializer::GetPDeviceExtension()
{
	return &m_DeviceExtension;
}

void VulkanInitializer::cleanup()
{
	// �f�o�C�X�̔j��
	vkDestroyDevice(m_LogicalDevice, nullptr);
	// �f�o�b�O�R�[���o�b�N�̔j��
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(m_Instance, callback, nullptr);
	}
	// �C���X�^���X�̔j��
	vkDestroyInstance(m_Instance, nullptr);
	// �A���P�[�^�[�̔j��
	vmaDestroyAllocator(m_VmaAllocator);
			
}

bool VulkanInitializer::CheckSupportSurface(VkSurfaceKHR surface)
{
	//�g�p�\�ȕ����f�o�C�X��T���Ă���
	PhysicalDeviceSelector selector(m_Instance);
	return selector.CheckSupportSurface(m_PhysicalDevice, surface);
}

void VulkanInitializer::createDebugCallback()
{
	// �f�o�b�O�R�[���o�b�N�̍쐬���
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    //createInfo.pfnCallback = VulkanValidation::debugCallback;

	// �f�o�b�O�R�[���o�b�N�̍쐬
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		func(m_Instance, &createInfo, nullptr, &callback);
	}
}


//void VulkanInitializer::allocateDynamicBufferTransferSpace()
//{
//	// Calculate alignment of model data
//	/*modelUniformAlignment = (sizeof(UboModel) + minUniformBufferOffset - 1)
//							& ~(minUniformBufferOffset - 1);
//
//	// Create space in memory to hold dynamic buffer that is aligned to our required alignment and holds MAX_OBJECTS
//	modelTransferSpace = (UboModel *)_aligned_malloc(modelUniformAlignment * MAX_OBJECTS, modelUniformAlignment);*/
//}




void VulkanInitializer::CreateAllocator(vk::Instance instance, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
	// �A���P�[�^�쐬���
	VmaAllocatorCreateInfo allocatorInfo;
	allocatorInfo.vulkanApiVersion = VulkanDefine.ApiVersion;
	allocatorInfo.instance			= instance;
	allocatorInfo.device			= logicalDevice;
	allocatorInfo.physicalDevice	= physicalDevice;

	// �A���P�[�^�̍쐬
	auto result = vmaCreateAllocator(&allocatorInfo, &m_VmaAllocator);

	// ���s��
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMA�A���P�[�^�̍쐬�Ɏ��s���܂���!");
	}
}




