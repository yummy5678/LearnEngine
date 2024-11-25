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
	// オブジェクト
	InstanceGenerator	instanceCreator;
	DeviceGenerator		deviceCreator;


	try {
		//インスタンスの作成
		instanceCreator.Create(m_InstanceExtension);
		m_Instance = instanceCreator.GetInstanse();

		createDebugCallback();

		//物理・論理デバイスの作成
		deviceCreator.Create(m_DeviceExtension, m_Instance);

		//物理デバイスを取得
		m_PhysicalDevice = deviceCreator.GetPhysicalDevice();

		//論理デバイスを取得
		m_LogicalDevice	= deviceCreator.GetLogicalDevice();

		// アロケーターの作成
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
	// デバイスの破棄
	vkDestroyDevice(m_LogicalDevice, nullptr);
	// デバッグコールバックの破棄
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(m_Instance, callback, nullptr);
	}
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

void VulkanInitializer::createDebugCallback()
{
	// デバッグコールバックの作成情報
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    //createInfo.pfnCallback = VulkanValidation::debugCallback;

	// デバッグコールバックの作成
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
	// アロケータ作成情報
	VmaAllocatorCreateInfo allocatorInfo;
	allocatorInfo.vulkanApiVersion = VulkanDefine.ApiVersion;
	allocatorInfo.instance			= instance;
	allocatorInfo.device			= logicalDevice;
	allocatorInfo.physicalDevice	= physicalDevice;

	// アロケータの作成
	auto result = vmaCreateAllocator(&allocatorInfo, &m_VmaAllocator);

	// 失敗時
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("VMAアロケータの作成に失敗しました!");
	}
}




