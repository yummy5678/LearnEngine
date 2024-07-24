#include "VulkanInstance.h"


InstanceGenerator::InstanceGenerator()
{
	
}

InstanceGenerator::~InstanceGenerator()
{
}

void InstanceGenerator::CreateInstance()
{
	m_ApplicationInfo = CreateApplicationInfo();
	auto instanceInfo = CreateInstanceInfo(&m_ApplicationInfo);
	m_Instance = vk::createInstanceUnique(instanceInfo);
	// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ�̃G���[���b�Z�[�W
	if (!m_Instance) 
	{
		throw std::runtime_error("Vulkan�C���X�^���X�̍쐬�Ɏ��s���܂����I");
	}

}

void InstanceGenerator::Create()
{
	CreateInstance();
}

vk::Instance InstanceGenerator::GetInstanse()
{
	return m_Instance.get();
}

std::vector<const char*>* InstanceGenerator::GetRequiredInstanceExtensionsPointer()
{
	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	static std::vector<const char*> instanceExtensions;	//static�ϐ�

	// �C���X�^���X���g�p����g���@�\��ݒ肷��
	uint32_t glfwExtensionCount = 0;	// GLFW�͕����̊g���@�\��v������ꍇ������
	const char** glfwExtensions;		// �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^(�z��)�̃|�C���^(C������)���K�v

	// GLFW�̊g���@�\���擾����
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFW�̊g���@�\�����X�g�ɒǉ�����
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	return &instanceExtensions;
}

vk::ApplicationInfo InstanceGenerator::CreateApplicationInfo()
{
	// �A�v���P�[�V�����̏���������
	// �����͍D���Ȓl����������
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = "Vulkan App";                     // �A�v���P�[�V�����̖��O
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);       // �A�v���P�[�V�����̃o�[�W������ 
	appInfo.pEngineName = "No Engine";                           // �G���W���̖��O
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);            // �G���W���̃o�[�W������
	appInfo.apiVersion = VK_API_VERSION_1_0;                     // Vulkan API�̃o�[�W����

	return appInfo;
}

const vk::InstanceCreateInfo InstanceGenerator::CreateInstanceInfo(const vk::ApplicationInfo* appInfo)
{
	// �G���[�`�F�b�N
	if (validationEnabled && !CheckValidationLayerSupport(validationLayers))
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}


	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	m_InstanceInfo.pNext;											// �g���@�\�̏�� 
	m_InstanceInfo.flags;											// �C���X�^���X�̍쐬�t���O 
	m_InstanceInfo.pApplicationInfo = appInfo;						// �A�v���P�[�V�������ւ̃|�C���^
	m_InstanceInfo.enabledLayerCount = 0;							// �L���ɂ��郌�C���[�̐� 
	m_InstanceInfo.ppEnabledLayerNames = nullptr;					// �L���ɂ��郌�C���[�̖��O�̔z�� 
	m_InstanceInfo.enabledExtensionCount = 0;						// �L���ɂ���g���@�\�̐� 
	m_InstanceInfo.ppEnabledExtensionNames = nullptr;				// �L���ɂ���g���@�\�̖��O�̔z�� 	


	// GLFW�̊g���@�\���擾����
	auto instanceExtensions = GetRequiredInstanceExtensionsPointer();

	// �o���f�[�V�������L���ȏꍇ�A���ؗp�̃f�o�b�O���g���@�\��ǉ�����
	if (validationEnabled)
	{
		instanceExtensions->push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// �C���X�^���X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�m�F����
	if (!CheckInstanceExtensionSupport(instanceExtensions))
	{
		throw std::runtime_error("VkInstance �͕K�v�Ȋg���@�\���T�|�[�g���Ă��܂���I");
	}

	// �L���Ȋg���@�\�̐��Ɩ��O�̔z���ݒ肷��
	m_InstanceInfo.enabledExtensionCount = (uint32_t)instanceExtensions->size();
	m_InstanceInfo.ppEnabledExtensionNames = instanceExtensions->data();

	// �o���f�[�V�������L���ȏꍇ�A�L���ȃ��C���[�̐��Ɩ��O�̔z���ݒ肷��
	if (validationEnabled)
	{
		m_InstanceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
		m_InstanceInfo.ppEnabledLayerNames = validationLayers.data();
	}

	return m_InstanceInfo;
}

bool InstanceGenerator::CheckValidationLayerSupport(const std::vector<const char*> validationLayers)
{
	// �o���f�[�V�������C���[�̐����擾���A�K�؂ȃT�C�Y�̃x�N�^�[���쐬����
	uint32_t validationLayerCount;
	vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

	// �����o���f�[�V�������C���[��������Ȃ��ꍇ�����Ȃ��Ƃ�1�̃��C���[���K�v�ȏꍇ��false��Ԃ�
	if (validationLayerCount == 0 && validationLayers.size() > 0)
	{
		return false;
	}

	// ���p�\�ȃ��C���[�̏���ێ����邽�߂�VkLayerProperties�̃��X�g���쐬����
	std::vector<VkLayerProperties> availableLayers(validationLayerCount);
	vkEnumerateInstanceLayerProperties(&validationLayerCount, availableLayers.data());

	// �w�肳�ꂽ�o���f�[�V�������C���[�����p�\�ȃ��C���[���X�g�Ɋ܂܂�Ă��邩�`�F�b�N����
	for (const auto& validationLayer : validationLayers)
	{
		bool hasLayer = false;
		for (const auto& availableLayer : availableLayers)
		{
			// ���C���[������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(validationLayer, availableLayer.layerName) == 0)
			{
				hasLayer = true;
				break;
			}
		}

		// �w�肳�ꂽ�o���f�[�V�������C���[��������Ȃ��ꍇ��false��Ԃ�
		if (!hasLayer)
		{
			return false;
		}
	}

	// ���ׂĂ̎w�肳�ꂽ�o���f�[�V�������C���[�����������ꍇ��true��Ԃ�
	return true;
}

bool InstanceGenerator::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	// �C���X�^���X���g�p����g���@�\��ݒ肷��
	uint32_t glfwExtensionCount = 0;	// GLFW�͕����̊g���@�\��v������ꍇ������
	const char** glfwExtensions;		// �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^(�z��)�̃|�C���^(C������)���K�v

	// GLFW�̊g���@�\���擾����
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFW�̊g���@�\�����X�g�ɒǉ�����
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	return &instanceExtensions;
}
