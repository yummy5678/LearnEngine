#include "DeviceExtensionManager.h"

DeviceExtension::DeviceExtension()
{
}

DeviceExtension::~DeviceExtension()
{
}

std::vector<const char*> DeviceExtension::GetExtensions(vk::PhysicalDevice physicalDevice)
{
	if (CheckExtensionsSupport(m_ExtensionList, physicalDevice) == false)
	{
		// �G���[���b�Z�[�W
		std::cout << "GPU���o�^���ꂽ�g���@�\�ɑΉ����Ă��܂���I" << std::endl;
	}

	//�쐬�������X�g��Ԃ�
	return m_ExtensionList;
}

//�X���b�v�`�F�[���̊g���@�\��L����(�t���O�ŊǗ����Ă���)
void DeviceExtension::UseSwapchain()
{
	const char* extension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	if (CheckHasString(extension) == false)	//�����G�N�X�e���V������������ΐV�������X�g�ɉ�����
	m_ExtensionList.push_back(extension);
}

void DeviceExtension::UseDynamicRendering()
{
	const char* extension = VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME;
	if (CheckHasString(extension) == false)	//�����G�N�X�e���V������������ΐV�������X�g�ɉ�����
	m_ExtensionList.push_back(extension);
}

bool DeviceExtension::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice)
{
	// �f�o�C�X���T�|�[�g����g���@�\�̐����擾����
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	// �����g���@�\��������Ȃ��ꍇ�A���s�Ƃ���false��Ԃ�
	if (extensionCount == 0)
	{
		return false;
	}

	// �g���@�\�̏���ێ����邽�߂�VkExtensionProperties�̃��X�g���쐬����
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

	// �f�o�C�X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�`�F�b�N����
	for (const auto& deviceExtension : checkExtensionNames)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// �g���@�\������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(deviceExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// �K�v�Ȋg���@�\��������Ȃ��ꍇ��false��Ԃ�
		if (!hasExtension)
		{
			return false;
		}
	}

	// ���ׂĂ̕K�v�Ȋg���@�\�����������ꍇ��true��Ԃ�
	return true;
}

bool DeviceExtension::CheckHasString(const char* target)
{
	for (const char* string : m_ExtensionList) 
	{
		// �����񂪈�v���邩�m�F
		if (std::strcmp(string, target) == 0)
		{ 

			return true;
		}
	}
	return false;
}
