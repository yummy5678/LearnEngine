#include "VulkanInitializer.h"


VulkanInitializer::VulkanInitializer() :
	m_InstanceExtension(),
	//m_DeviceExtension(),
	m_InstanceGenerator(),
	m_DeviceGenerator()

{
}

VulkanInitializer::~VulkanInitializer()
{
}


int VulkanInitializer::init()
{	
	try {
		//�C���X�^���X�̍쐬
		m_InstanceGenerator.Create(m_InstanceExtension);
		m_Instance = m_InstanceGenerator.GetInstanse();

		createDebugCallback();

		//�����E�_���f�o�C�X�̍쐬
		m_DeviceGenerator.Create(m_DeviceExtension, m_Instance);

		//�����f�o�C�X���擾
		m_PhysicalDevice = m_DeviceGenerator.GetPhysicalDevice();

		//�_���f�o�C�X���擾
		m_LogicalDevice	= m_DeviceGenerator.GetLogicalDevice();

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
	return m_InstanceGenerator.GetInstanse();
}

vk::Device VulkanInitializer::GetLogicalDevice()
{
	return m_LogicalDevice;
}

vk::PhysicalDevice VulkanInitializer::GetPhysicalDevice()
{
	return m_DeviceGenerator.GetPhysicalDevice();
}

VmaAllocator VulkanInitializer::GetVmaAllocator()
{
	return m_VmaAllocator;
}

InstanceExtension* VulkanInitializer::GetPInstanceExtension()
{
	return &m_InstanceExtension;
}

DeviceExtension* VulkanInitializer::GetPDeviceExtension()
{
	return &m_DeviceExtension;
}

bool VulkanInitializer::CheckSupportSurface(VkSurfaceKHR surface)
{
	//�g�p�\�ȕ����f�o�C�X��T���Ă���
	PhysicalDeviceSelector selector(m_InstanceGenerator.GetInstanse());
	return selector.CheckSupportSurface(m_PhysicalDevice, surface);
}


void VulkanInitializer::allocateDynamicBufferTransferSpace()
{
	// Calculate alignment of model data
	/*modelUniformAlignment = (sizeof(UboModel) + minUniformBufferOffset - 1)
							& ~(minUniformBufferOffset - 1);

	// Create space in memory to hold dynamic buffer that is aligned to our required alignment and holds MAX_OBJECTS
	modelTransferSpace = (UboModel *)_aligned_malloc(modelUniformAlignment * MAX_OBJECTS, modelUniformAlignment);*/
}




void VulkanInitializer::CreateAllocator(vk::Instance instance, vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
	// �A���P�[�^�쐬���
	VmaAllocatorCreateInfo allocatorInfo;
	allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_0;
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




