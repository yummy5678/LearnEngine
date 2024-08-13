#include "VulkanExtensionManager.h"


InstanceExtensionManager::InstanceExtensionManager()
{
}

InstanceExtensionManager::~InstanceExtensionManager()
{
}

void InstanceExtensionManager::Add(const char* extensionName)
{
	if (CheckExtensionSupport(extensionName))
	{
		std::cout << "�C���X�^���X�g���@�\:" << extensionName << "���ǉ�����܂���" << std::endl;
		m_ExtensionList.insert(extensionName);
	}
	else
	{
		std::cout << "�C���X�^���X�g���@�\:" << extensionName << "���m�F�ł��܂���ł���" << std::endl;
	}

}

bool InstanceExtensionManager::Delete(const char* extensionName)
{
    return false;
}

std::unordered_set<const char*> InstanceExtensionManager::GetList()
{
    return m_ExtensionList;
}

// �z����̕����񂪊Ԉ���Ă�����A
// �Ή����Ă��Ȃ����̂��ƃG���[���o��
bool InstanceExtensionManager::CheckExtensionSupport(const char* checkName)
{
    std::cout << "�C���X�^���X�̎w�肳�ꂽ�g���@�\�����p�o���邩�m�F" << std::endl;

    // �g���@�\�̃��X�g���擾
    auto extensions = vk::enumerateInstanceExtensionProperties();

    // �w�肳�ꂽ�g���@�\�����X�g�ɑ��݂��邩�`�F�b�N
    for (const auto& extension : extensions) {
        if (std::strcmp(checkName, extension.extensionName) == 0) 
        {
            std::cout << "����" << std::endl;
            return true;
        }
    }

    std::cout << "���s" << std::endl;
    return false;
}
