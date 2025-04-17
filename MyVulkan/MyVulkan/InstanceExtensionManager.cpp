#include "InstanceExtensionManager.h"


InstanceExtension::InstanceExtension()
{
}

InstanceExtension::~InstanceExtension()
{
}

std::vector<const char*>* InstanceExtension::GetExtensions()
{
	if (!CheckExtensionsSupport((m_ExtensionList)))
	{
		//�G���[
		//���X�g�̊g���@�\�̖��O���T�|�[�g���Ă��Ȃ�����
		return nullptr;
	}
	return &m_ExtensionList;
}

std::vector<const char*>* InstanceExtension::GetValidationLayers()
{
	return &m_ValidationLayers;
}

void InstanceExtension::UseGLFW()
{
	CreateGLFWExtension();
}

void InstanceExtension::UseValidation()
{
	CreateValidationExtension();
	m_ValidationLayers.push_back("VK_LAYER_KHRONOS_validation");
}

void InstanceExtension::CreateGLFWExtension()
{
	// glfwInit();
	std::cout << "GLFW�g���@�\�̃��X�g�̎󂯎��" << std::endl;

	// �C���X�^���X���g�p����g���@�\��ݒ肷��
	uint32_t glfwExtensionCount = 0;	// GLFW�͕����̊g���@�\��v������ꍇ������
	const char** glfwExtensions;		// �g���@�\��C������̔z��Ƃ��ēn����邽�߁A�|�C���^(�z��)�̃|�C���^(C������)���K�v

	// GLFW�̊g���@�\���擾����
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFW�̊g���@�\�����X�g�ɒǉ�����
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		m_ExtensionList.push_back(glfwExtensions[i]);
		std::cout << "�g���@�\���F" << glfwExtensions[i] << std::endl;
	}
}

void InstanceExtension::CreateValidationExtension()
{
	// �o���f�[�V�������L���ȏꍇ�A���ؗp�̃f�o�b�O���g���@�\��ǉ�����
	// m_ExtensionList.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	m_ExtensionList.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	std::cout << "�g���@�\���F" << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << std::endl;
}

// �z����̕����񂪊Ԉ���Ă�����A
// �Ή����Ă��Ȃ����̂��ƃG���[���o��
//bool InstanceExtensionManager::CheckExtensionSupport(const char* checkName)
//{
//    std::cout << "�C���X�^���X�̎w�肳�ꂽ�g���@�\�����p�o���邩�m�F" << std::endl;
//
//    // �g���@�\�̃��X�g���擾
//    auto extensions = vk::enumerateInstanceExtensionProperties();
//
//    // �w�肳�ꂽ�g���@�\�����X�g�ɑ��݂��邩�`�F�b�N
//    for (const auto& extension : extensions) {
//        if (std::strcmp(checkName, extension.extensionName) == 0) 
//        {
//            std::cout << "����" << std::endl;
//            return true;
//        }
//    }
//
//    std::cout << "���s" << std::endl;
//    return false;
//}

bool InstanceExtension::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames)
{
	std::wcout << L"�C���X�^���X�̎w�肳�ꂽ�g���@�\�����p�o���邩�m�F" << std::endl;

	// �g���@�\�̃��X�g���擾
	auto extensions = vk::enumerateInstanceExtensionProperties();

	// Check if given extensions are in list of available extensions
	for (const auto& checkExtension : checkExtensionNames)
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
