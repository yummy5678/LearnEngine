#include "VulkanInstance.h"
//VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE


InstanceGenerator::InstanceGenerator()
{
	m_ClassName = "InstanceGenerator";
	std::cout << m_ClassName << "�̃R���X�g���N�^���Ă΂�܂���" << std::endl;
}

InstanceGenerator::~InstanceGenerator()
{

}

void InstanceGenerator::Create(InstanceExtension extensionManager)
{
	std::cout << m_ClassName << "�쐬�֐����Ă΂�܂���" << std::endl;
	m_bCreated = true;	//�쐬�t���O���I���ɂ���(�f�o�b�O�p)

	/*/////////////////////
	* ���C���[���X�g�̍쐬
	*//////////////////////


	extensionManager.GetExtensions();

	m_ApplicationInfo = GetApplicationInfo();
	auto instanceInfo = GetInstanceInfo(&m_ApplicationInfo, extensionManager);

	try
	{

		m_Instance = vk::createInstance(instanceInfo);
		std::cout << m_ClassName << "�C���X�^���X�̍쐬�ɐ������܂����B" << std::endl;
		if (VulkanDefine.ValidationEnabled)
		{
			createDebugCallback();
			std::cout << m_ClassName << "�R�[���o�b�N�̍쐬�ɐ������܂����B" << std::endl;
		}
	}
	catch (const std::runtime_error& e)
	{
		// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ�̃G���[���b�Z�[�W
		throw std::runtime_error("�C���X�^���X�̍쐬�Ɏ��s���܂����I");
	}
}

void InstanceGenerator::Destroy()
{
	//�쐬�t���O�������Ă��Ȃ��ꍇ�͉���������s��Ȃ�
	if (m_bCreated == false) return;
	m_bCreated = false;

	// �C���X�^���X�̉��
	vkDestroyInstance(m_Instance, nullptr);
	throw std::runtime_error("�C���X�^���X��������܂����I");
}

vk::Instance InstanceGenerator::GetInstanse()
{
	CheckCreated();
	return m_Instance;
}

void InstanceGenerator::createDebugCallback()
{
	// DispatchLoaderDynamic�̃Z�b�g�A�b�v
	vk::DispatchLoaderDynamic dldi(m_Instance, vkGetInstanceProcAddr);

	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo{
	vk::DebugUtilsMessengerCreateFlagsEXT{}, // ���݂͋��OK
	vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning 
	| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError 
	//| vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
	//| vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
	,
	vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
	debugCallback, // �R�[���o�b�N�֐�
	nullptr        // pUserData
	};



	callback = m_Instance.createDebugUtilsMessengerEXTUnique(debugUtilsMessengerInfo, nullptr, dldi);
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
	InstanceExtension& extensionManager)
{
	std::cout << "�C���X�^���X�쐬���̍쐬" << std::endl;

	if (VulkanDefine.ValidationEnabled)//���؃��C���[�̃t���O�������Ă���Βǉ�
	{
		extensionManager.UseValidation();
	}
	auto extensions = extensionManager.GetExtensions();
	auto layers = extensionManager.GetValidationLayers();
	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;												
	instanceInfo.flags;												
	instanceInfo.pApplicationInfo = appInfo;					// �A�v���P�[�V�������ւ̃|�C���^
	instanceInfo.enabledLayerCount = layers->size();			// �L���ɂ��郌�C���[�̐� 
    instanceInfo.ppEnabledLayerNames = layers->data();			// �L���ɂ��郌�C���[�̖��O�̔z��
	instanceInfo.enabledExtensionCount = extensions->size();	// �L���ɂ���g���@�\�̐� 
	instanceInfo.ppEnabledExtensionNames = extensions->data();	// �L���ɂ���g���@�\�̖��O�̔z�� 	

	return instanceInfo;
}


