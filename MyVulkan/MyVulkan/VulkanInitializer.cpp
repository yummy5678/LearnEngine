#include "VulkanInitializer.h"



VulkanInitializer::VulkanInitializer() :
	m_Instance(VK_NULL_HANDLE),
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_VmaAllocator(VK_NULL_HANDLE),
	m_InstanceExtension(),
	m_DeviceExtension()
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

	if (VulkanDefine.ValidationEnabled == true)
	{
		// RAM�̏󋵂��Ď��ł���g���@�\��L��
		m_DeviceExtension.UseMemoryBudget();
		m_DeviceExtension.UseMemoryPriority();
	}

	try {
		//�C���X�^���X�̍쐬
		instanceCreator.Create(m_InstanceExtension);
		m_Instance = instanceCreator.GetInstanse();

		//�����E�_���f�o�C�X�̍쐬
		deviceCreator.Create(m_DeviceExtension, m_Instance);

		//�����f�o�C�X���擾
		m_PhysicalDevice = deviceCreator.GetPhysicalDevice();

		//�_���f�o�C�X���擾
		m_LogicalDevice	= deviceCreator.GetLogicalDevice();

		// �A���P�[�^�[�̍쐬
		//vmaCreator.CreateAllocator(m_Instance, m_LogicalDevice, m_PhysicalDevice);
		//m_VmaAllocator = vmaCreator.GetAllocator();
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

vk::Device* VulkanInitializer::GetPLogicalDevice()
{
	return &m_LogicalDevice;
}

vk::PhysicalDevice* VulkanInitializer::GetPPhysicalDevice()
{
	return &m_PhysicalDevice;
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

bool VulkanInitializer::IsInitialized()
{
	// �K�v�ȃI�u�W�F�N�g�����ׂĎ����Ă����TRUE
	if(	m_Instance			!= VK_NULL_HANDLE ||
		m_LogicalDevice		!= VK_NULL_HANDLE ||
		m_PhysicalDevice	!= VK_NULL_HANDLE ||
		m_VmaAllocator		!= VK_NULL_HANDLE )
	return true;

	// ����ȊO�̏ꍇ��FALSE
	return false;
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
	allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT | VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
	allocatorInfo.vulkanApiVersion = VulkanDefine.ApiVersion;
	allocatorInfo.instance = instance;
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = logicalDevice;
	allocatorInfo.preferredLargeHeapBlockSize = 0; // �u0�v�Ńf�t�H���g�l���ݒ肳���
	allocatorInfo.pTypeExternalMemoryHandleTypes = nullptr;
	allocatorInfo.pAllocationCallbacks = nullptr;
	allocatorInfo.pDeviceMemoryCallbacks = nullptr;
	allocatorInfo.pHeapSizeLimit = nullptr;
	allocatorInfo.pVulkanFunctions = nullptr;
	

	if (VulkanDefine.ValidationEnabled == true)
	{
		allocatorInfo.pAllocationCallbacks = &AllocationCallbacks;
		allocatorInfo.pDeviceMemoryCallbacks = &deviceMemoryCallbacks;
	}

	
	// �A���P�[�^�̍쐬
	VkResult result = vmaCreateAllocator(&allocatorInfo, &m_VmaAllocator);

	// ���s��
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMA�A���P�[�^�̍쐬�Ɏ��s���܂���!");
	}
}




