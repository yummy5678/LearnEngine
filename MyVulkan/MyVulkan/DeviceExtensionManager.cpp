#include "DeviceExtensionManager.h"

DeviceExtensionManager::DeviceExtensionManager()
{
}

DeviceExtensionManager::~DeviceExtensionManager()
{
}

std::set<const char*> DeviceExtensionManager::GetExtensions(vk::PhysicalDevice physicalDevice)
{
	//�쐬�������X�g��Ԃ�
	return m_ExtensionList;
}

//�X���b�v�`�F�[���̊g���@�\��L����(�t���O�ŊǗ����Ă���)
void DeviceExtensionManager::UseSwapchain()
{
	m_ExtensionList.insert(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void DeviceExtensionManager::UseDynamicRendering()
{
	m_ExtensionList.insert(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
}

bool DeviceExtensionManager::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice)
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
