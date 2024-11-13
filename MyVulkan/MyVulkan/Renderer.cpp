#include "Renderer.h"


VulkanInitializer::VulkanInitializer() :
	m_pWindow(nullptr),
	m_InstanceExtension(),
	//m_DeviceExtension(),
	m_InstanceGenerator(),
	m_SurfaceGenerator(m_InstanceExtension),
	m_DeviceGenerator()

{
}

VulkanInitializer::~VulkanInitializer()
{
}


int VulkanInitializer::init(GameWindow renderWindow, DeviceExtensionCollector extension)
{
	m_pWindow = renderWindow.GetPointer(); //ウィンドウのポインタのセット
	
	try {
		//インスタンスの作成
		m_InstanceGenerator.Create(m_InstanceExtension);
		auto instance = m_InstanceGenerator.GetInstanse();

		createDebugCallback();

		//サーフェスの作成
		m_SurfaceGenerator.CreateWindowSurface(instance,m_pWindow);
		auto surface = m_SurfaceGenerator.GetSurface();


		//物理・論理デバイスの作成
		m_DeviceGenerator.Create(extension, instance, surface);
		//物理デバイスを取得
		auto physicalDevice			= m_DeviceGenerator.GetPhysicalDevice();
		auto surfaceCapabilities	= m_SurfaceGenerator.GetCapabilities(physicalDevice);
		auto windowExtent			= surfaceCapabilities.currentExtent;
		auto surfaceFomat			= m_SurfaceGenerator.GetFomats(physicalDevice)[0];

		//論理デバイスを取得
		auto logicalDevice	= m_DeviceGenerator.GetLogicalDevice();

		// アロケーターの作成
		CreateAllocator(instance, logicalDevice, physicalDevice);

		// Create our default "no texture" texture
		createTexture("plain.png");
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
	return m_InstanceGenerator.GetInstanse();
}

vk::Device VulkanInitializer::GetLogicalDevice()
{
	return m_LogicalDevice;
}

vk::PhysicalDevice VulkanInitializer::GetPhygicalDevice()
{
	return m_DeviceGenerator.GetPhysicalDevice();
}

VmaAllocator VulkanInitializer::GetVmaAllocator()
{
	return m_VmaAllocator;
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
	// アロケータ作成情報
	VmaAllocatorCreateInfo allocatorInfo;
	allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_0;
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




