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
	// オブジェクト
	InstanceGenerator	instanceCreator;
	DeviceGenerator		deviceCreator;

	if (VulkanDefine.ValidationEnabled == true)
	{
		// RAMの状況を監視できる拡張機能を有効
		m_DeviceExtension.UseMemoryBudget();
		m_DeviceExtension.UseMemoryPriority();
	}

	try {
		//インスタンスの作成
		instanceCreator.Create(m_InstanceExtension);
		m_Instance = instanceCreator.GetInstanse();

		//物理・論理デバイスの作成
		deviceCreator.Create(m_DeviceExtension, m_Instance);

		//物理デバイスを取得
		m_PhysicalDevice = deviceCreator.GetPhysicalDevice();

		//論理デバイスを取得
		m_LogicalDevice	= deviceCreator.GetLogicalDevice();

		// アロケーターの作成
		//vmaCreator.CreateAllocator(m_Instance, m_LogicalDevice, m_PhysicalDevice);
		//m_VmaAllocator = vmaCreator.GetAllocator();
		CreateAllocator(m_Instance, m_LogicalDevice, m_PhysicalDevice);

		// Create our default "no texture" texture
		//createTexture("plain.png");

	}
	catch (const std::runtime_error& e) {
		//エラーメッセージ受け取り
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
	// デバイスの破棄
	vkDestroyDevice(m_LogicalDevice, nullptr);
	// インスタンスの破棄
	vkDestroyInstance(m_Instance, nullptr);
	// アロケーターの破棄
	vmaDestroyAllocator(m_VmaAllocator);
			
}

bool VulkanInitializer::CheckSupportSurface(VkSurfaceKHR surface)
{
	//使用可能な物理デバイスを探してくる
	PhysicalDeviceSelector selector(m_Instance);
	return selector.CheckSupportSurface(m_PhysicalDevice, surface);
}

bool VulkanInitializer::IsInitialized()
{
	// 必要なオブジェクトをすべて持っていればTRUE
	if(	m_Instance			!= VK_NULL_HANDLE ||
		m_LogicalDevice		!= VK_NULL_HANDLE ||
		m_PhysicalDevice	!= VK_NULL_HANDLE ||
		m_VmaAllocator		!= VK_NULL_HANDLE )
	return true;

	// それ以外の場合はFALSE
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
	// アロケータ作成情報
	VmaAllocatorCreateInfo allocatorInfo;
	allocatorInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT | VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
	allocatorInfo.vulkanApiVersion = VulkanDefine.ApiVersion;
	allocatorInfo.instance = instance;
	allocatorInfo.physicalDevice = physicalDevice;
	allocatorInfo.device = logicalDevice;
	allocatorInfo.preferredLargeHeapBlockSize = 0; // 「0」でデフォルト値が設定される
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

	
	// アロケータの作成
	VkResult result = vmaCreateAllocator(&allocatorInfo, &m_VmaAllocator);

	// 失敗時
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAアロケータの作成に失敗しました!");
	}
}




