#include "VulkanInstance.h"



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
	auto layers = GetLayers();

	m_ApplicationInfo = GetApplicationInfo();
	auto instanceInfo = GetInstanceInfo(&m_ApplicationInfo, layers, extensionManager);

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

InstanceLayerManager InstanceGenerator::GetLayers()
{
	InstanceLayerManager layers;
	//���C�����X�g�̎擾
	if (VulkanDefine.ValidationEnabled)//���؃��C���[�̃t���O�������Ă���Βǉ�
	{
		layers.Add(VULKAN_LAYER_VALIDATION);
	}

	return layers;
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
	InstanceLayerManager& layerManager,
	InstanceExtension& extensionManager)
{
	std::cout << "�C���X�^���X�쐬���̍쐬" << std::endl;
	auto layers = layerManager.GetList();
	auto extensions = extensionManager.GetExtensions();

	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;												
	instanceInfo.flags;												
	instanceInfo.pApplicationInfo = appInfo;						// �A�v���P�[�V�������ւ̃|�C���^
	instanceInfo.enabledLayerCount = layers->size();					// �L���ɂ��郌�C���[�̐� 
	instanceInfo.ppEnabledLayerNames = &(*layers->begin());			// �L���ɂ��郌�C���[�̖��O�̔z�� 
	instanceInfo.enabledExtensionCount = extensions->size();		// �L���ɂ���g���@�\�̐� 
	instanceInfo.ppEnabledExtensionNames = extensions->data();	// �L���ɂ���g���@�\�̖��O�̔z�� 	

	return instanceInfo;
}


