#include "DeviceExtensionManager.h"

CDeviceExtensionManager::CDeviceExtensionManager()
{
}

CDeviceExtensionManager::~CDeviceExtensionManager()
{
}

std::vector<const char*>* CDeviceExtensionManager::GetExtensions(vk::PhysicalDevice physicalDevice)
{
	//���X�g��������
	m_ExtensionList.clear();

	//�X���b�v�`�F�C�����L���ȏꍇ�A�g���@�\��ǉ�
	if (m_bSwapchain = true) CreateSwapChainExtension();

	if (CheckExtensionsSupport(m_ExtensionList, physicalDevice) == false)
	{
		//�G���[
		//���X�g�̊g���@�\�̖��O���T�|�[�g���Ă��Ȃ�����
		return nullptr;
	}

	//�쐬�������X�g��Ԃ�
	return &m_ExtensionList;
}

//�X���b�v�`�F�C���̊g���@�\��L����(�t���O�ŊǗ����Ă���)
void CDeviceExtensionManager::UseSwapchain()
{
	m_bSwapchain = true;
}

void CDeviceExtensionManager::CreateSwapChainExtension()
{
	m_ExtensionList.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

bool CDeviceExtensionManager::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames, vk::PhysicalDevice physicalDevice)
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
