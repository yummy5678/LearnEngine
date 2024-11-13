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
	m_pWindow = renderWindow.GetPointer(); //�E�B���h�E�̃|�C���^�̃Z�b�g
	
	try {
		//�C���X�^���X�̍쐬
		m_InstanceGenerator.Create(m_InstanceExtension);
		auto instance = m_InstanceGenerator.GetInstanse();

		createDebugCallback();

		//�T�[�t�F�X�̍쐬
		m_SurfaceGenerator.CreateWindowSurface(instance,m_pWindow);
		auto surface = m_SurfaceGenerator.GetSurface();


		//�����E�_���f�o�C�X�̍쐬
		m_DeviceGenerator.Create(extension, instance, surface);
		//�����f�o�C�X���擾
		auto physicalDevice			= m_DeviceGenerator.GetPhysicalDevice();
		auto surfaceCapabilities	= m_SurfaceGenerator.GetCapabilities(physicalDevice);
		auto windowExtent			= surfaceCapabilities.currentExtent;
		auto surfaceFomat			= m_SurfaceGenerator.GetFomats(physicalDevice)[0];

		//�_���f�o�C�X���擾
		auto logicalDevice	= m_DeviceGenerator.GetLogicalDevice();

		// �A���P�[�^�[�̍쐬
		CreateAllocator(instance, logicalDevice, physicalDevice);

		// Create our default "no texture" texture
		createTexture("plain.png");
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




