#include "VulkanInstance.h"



InstanceGenerator::InstanceGenerator()
{
	m_ClassName = "InstanceGenerator";
	std::cout << m_ClassName << "�̃R���X�g���N�^���Ă΂�܂���" << std::endl;
}

InstanceGenerator::~InstanceGenerator()
{
	std::cout << m_ClassName << "�̃f�X�g���N�^���Ă΂�܂���" << std::endl;
}

void InstanceGenerator::Create(InstanceExtensionManager extensionManager)
{
	std::cout << m_ClassName << "�쐬�֐����Ă΂�܂���" << std::endl;
	m_bCreated = true;	//�쐬�t���O���I���ɂ���(�f�o�b�O�p)

	/*/////////////////////
	* ���C���[���X�g�̍쐬
	*//////////////////////
	auto layers = GetLayers();

	/*/////////////////////
	* �g���@�\���X�g�̍쐬
	*//////////////////////
	auto extensions = extensionManager;

	m_ApplicationInfo = GetApplicationInfo();
	auto instanceInfo = GetInstanceInfo(&m_ApplicationInfo, layers, extensions);

	try
	{
		std::cout << m_ClassName << "�C���X�^���X�̍쐬�ɐ������܂����B" << std::endl;
		m_Instance = vk::createInstance(instanceInfo);
	}
	catch (const std::runtime_error& e)
	{
		// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ�̃G���[���b�Z�[�W
		throw std::runtime_error("�C���X�^���X�̍쐬�Ɏ��s���܂����I");
	}
}

void InstanceGenerator::Destroy()
{
	if (m_bCreated == false) return;
	throw std::runtime_error("�C���X�^���X��j�󂵂܂����I");
	vkDestroyInstance(m_Instance, nullptr);
	m_bCreated = false;
}

vk::Instance InstanceGenerator::GetInstanse()
{
	CheckCreated();
	return m_Instance;
}

std::vector<const char*> InstanceGenerator::GetRequiredInstanceExtensionsPointer()
{
	std::cout << m_ClassName << "GLFW�g���@�\�̃��X�g�̎󂯎��" << std::endl;

	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	std::vector<const char*> instanceExtensions;

	// �C���X�^���X���g�p����g���@�\��ݒ肷��
	uint32_t glfwExtensionCount = 0;	// GLFW�͕����̊g���@�\��v������ꍇ������
	const char** glfwExtensions;		// �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^(�z��)�̃|�C���^(C������)���K�v

	// GLFW�̊g���@�\���擾����
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFW�̊g���@�\�����X�g�ɒǉ�����
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
		std::cout<< "�g���@�\���F" << glfwExtensions[i] << std::endl;
	}

	return instanceExtensions;
}

InstanceLayerManager InstanceGenerator::GetLayers()
{
	InstanceLayerManager layers;
	//���C�����X�g�̎擾
	if (VulkanDefine.ValidationEnabled)//���؃��C���[�̃t���O�������Ă���Βǉ�
	{
		layers.Add(VULKAN_LAYER_VALIDATION);
	}
	//std::vector<const char*> layers = m_LayerManager.GetLayerList();
	//CheckLayersSupport(layers);	//���C���[���g�p�ł��邩�m�F
	return layers;
}

std::vector<const char*> InstanceGenerator::GetExtensions()
{
	// �g���@�\���X�g�̍쐬
	auto instanceExtensions = GetRequiredInstanceExtensionsPointer();	// GLFW�̊g���@�\���擾����

	// �o���f�[�V�������L���ȏꍇ�A���ؗp�̃f�o�b�O���g���@�\��ǉ�����
	if (VulkanDefine.ValidationEnabled)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		std::cout << "�f�o�b�O�g���@�\�̒ǉ�" << std::endl;
	}


	return instanceExtensions;
}

vk::ApplicationInfo InstanceGenerator::GetApplicationInfo()
{
	std::cout << "�A�v���P�[�V�������̍쐬" << std::endl;

	// �A�v���P�[�V�����̏���������
	// �����͍D���Ȓl����������
	// ApplicationInfo�̏���GraphicsDefine.h�Œ�`���Ă���
	vk::ApplicationInfo appInfo;
	appInfo.pNext = nullptr;										//�g���@�\���(����͐G��Ȃ��Ă���)
	appInfo.pApplicationName = VulkanDefine.ApplicationName;		// �A�v���P�[�V�����̖��O
	appInfo.applicationVersion = VulkanDefine.ApplicationVersion;	// �A�v���P�[�V�����̃o�[�W������ 
	appInfo.pEngineName = VulkanDefine.EngineName;					// �G���W���̖��O
	appInfo.engineVersion = VulkanDefine.EngineVersion;				// �G���W���̃o�[�W������
	appInfo.apiVersion = VulkanDefine.ApiVersion;					// Vulkan API�̃o�[�W����


	return appInfo;
}

const vk::InstanceCreateInfo InstanceGenerator::GetInstanceInfo(
	const vk::ApplicationInfo* appInfo,
	InstanceLayerManager layerManager,
	InstanceExtensionManager extensionManager)
{
	std::cout << "�C���X�^���X�쐬���̍쐬" << std::endl;
	auto layers = layerManager.GetList();
	auto extensions = extensionManager.GetList();

	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;												
	instanceInfo.flags;												
	instanceInfo.pApplicationInfo = appInfo;						// �A�v���P�[�V�������ւ̃|�C���^
	instanceInfo.enabledLayerCount = layers.size();					// �L���ɂ��郌�C���[�̐� 
	instanceInfo.ppEnabledLayerNames = &(*layers.begin());			// �L���ɂ��郌�C���[�̖��O�̔z�� 
	instanceInfo.enabledExtensionCount = extensions.size();			// �L���ɂ���g���@�\�̐� 
	instanceInfo.ppEnabledExtensionNames = &(*extensions.begin());	// �L���ɂ���g���@�\�̖��O�̔z�� 	

	return instanceInfo;
}


