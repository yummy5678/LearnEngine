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

void InstanceGenerator::Create()
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
	auto extensions = GetExtensions();

	m_ApplicationInfo = GetApplicationInfo();
	auto instanceInfo = GetInstanceInfo(&m_ApplicationInfo, &layers, &extensions);

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

std::vector<const char*> InstanceGenerator::GetLayers()
{
	//���C�����X�g�̎擾
	if (VulkanDefine.ValidationEnabled)//���؃��C���[�̃t���O�������Ă���Βǉ�
	{
		m_LayerManager.AddLayer(LAYER_VALIDATION);
	}
	std::vector<const char*> layers = m_LayerManager.GetLayerList();
	CheckLayersSupport(layers);	//���C���[���g�p�ł��邩�m�F
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

	// �C���X�^���X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�m�F����
	if (!CheckExtensionsSupport(instanceExtensions))
	{
		throw std::runtime_error("�C���X�^���X�͕K�v�Ȋg���@�\���T�|�[�g���Ă��܂���I");
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
	const std::vector<const char*>* layers,
	const std::vector<const char*>* extensions)
{
	std::cout << "�C���X�^���X�쐬���̍쐬" << std::endl;

	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;									// �g���@�\�̏�� 
	instanceInfo.flags;									// �C���X�^���X�̍쐬�t���O 
	instanceInfo.pApplicationInfo = appInfo;			// �A�v���P�[�V�������ւ̃|�C���^
	instanceInfo.enabledLayerCount = layers->size();		// �L���ɂ��郌�C���[�̐� 
	instanceInfo.ppEnabledLayerNames = layers->data();	// �L���ɂ��郌�C���[�̖��O�̔z�� 
	instanceInfo.enabledExtensionCount = (uint32_t)extensions->size();	// �L���ɂ���g���@�\�̐� 
	instanceInfo.ppEnabledExtensionNames = extensions->data();			// �L���ɂ���g���@�\�̖��O�̔z�� 	

	return instanceInfo;
}

// �z����̕����񂪊Ԉ���Ă�����A
// �Ή����Ă��Ȃ����̂��ƃG���[���o��
bool InstanceGenerator::CheckLayersSupport(const std::vector<const char*> validationLayers)
{
	std::cout << "�w�肳�ꂽ���C���[�����p�o���邩�m�F" << std::endl;

	//���p�\�ȃ��C���[�̃��X�g�������Ă���
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	// ���p�\�ȃ��C���[�������̃��C���[����0�̏ꍇ��false��Ԃ�
	if (availableLayers.empty() || validationLayers.empty())
	{
		std::cout << "���s" << std::endl;
		return false;
	}

	int hasCount = validationLayers.size();
	// �w�肳�ꂽ�o���f�[�V�������C���[�����p�\�ȃ��C���[���X�g�Ɋ܂܂�Ă��邩�`�F�b�N����
	for (const auto& validationLayer : validationLayers)
	{
		
		for (const auto& availableLayer : availableLayers)
		{
			// ���C���[������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(validationLayer, availableLayer.layerName) == 0)
			{
				if (--hasCount == 0)
				{
					// ���ׂĂ̎w�肳�ꂽ�o���f�[�V�������C���[�����������ꍇ��true��Ԃ�
					std::cout << "����" << std::endl;
					return true;
				}
			}
		}
	}

	// �w�肳�ꂽ���C���[��S�Č������Ȃ������ꍇ��false��Ԃ�
	std::cout << "�w�肳�ꂽ���C���[���������܂���ł���" << std::endl;
	return false;
}

// �z����̕����񂪊Ԉ���Ă�����A
// �Ή����Ă��Ȃ����̂��ƃG���[���o��
bool InstanceGenerator::CheckExtensionsSupport(std::vector<const char*> checkExtensions)
{
	std::cout << "�C���X�^���X�̎w�肳�ꂽ�g���@�\�����p�o���邩�m�F" << std::endl;

	// Need to get number of extensions to create array of correct size to hold extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Create a list of VkExtensionProperties using count
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Check if given extensions are in list of available extensions
	for (const auto& checkExtension : checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			if (strcmp(checkExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		if (!hasExtension)
		{
			std::cout << "���s" << std::endl;
			return false;
		}
	}

	std::cout << "����" << std::endl;
	return true;
}
