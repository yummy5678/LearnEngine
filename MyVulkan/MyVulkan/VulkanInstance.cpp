#include "VulkanInstance.h"




vk::ApplicationInfo VulkanCreate::GetApplicationInfo()
{
	// �A�v���P�[�V�����̏���������
	// �����͍D���Ȓl����������
	vk::ApplicationInfo appInfo = {};
	appInfo.pApplicationName = "Vulkan App";                     // �A�v���P�[�V�����̖��O
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);       // �A�v���P�[�V�����̃o�[�W������ 
	appInfo.pEngineName = "No Engine";                           // �G���W���̖��O
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);            // �G���W���̃o�[�W������
	appInfo.apiVersion = VK_API_VERSION_1_0;                     // Vulkan API�̃o�[�W����

	return appInfo;
}




VkInstanceCreateInfo VulkanCreate::GetInstanceInfo(vk::ApplicationInfo appInfo)
{
	// �G���[�`�F�b�N
	if (validationEnabled && !VulkanUtility::CheckValidationLayerSupport(validationLayers))
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}


	/*/////////////////////
	* �C���X�^���X�̍쐬
	*//////////////////////
	vk::InstanceCreateInfo createInfo = {};
	createInfo.pNext;											// �g���@�\�̏�� 
	createInfo.flags;											// �C���X�^���X�̍쐬�t���O 
	createInfo.pApplicationInfo = &appInfo;						// �A�v���P�[�V�������ւ̃|�C���^
	createInfo.enabledLayerCount = 0;							// �L���ɂ��郌�C���[�̐� 
	createInfo.ppEnabledLayerNames = nullptr;					// �L���ɂ��郌�C���[�̖��O�̔z�� 
	createInfo.enabledExtensionCount = 0;						// �L���ɂ���g���@�\�̐� 
	createInfo.ppEnabledExtensionNames = nullptr;				// �L���ɂ���g���@�\�̖��O�̔z�� 	

	// �C���X�^���X�g���@�\�̃��X�g���쐬����
	static std::vector<const char*> instanceExtensions = std::vector<const char*>();//�����̒l���X�R�[�v�O�Ɉړ���������

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

	// �o���f�[�V�������L���ȏꍇ�A���ؗp�̃f�o�b�O���g���@�\��ǉ�����
	if (validationEnabled)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// �C���X�^���X���K�v�Ƃ���g���@�\���T�|�[�g����Ă��邩�m�F����
	if (!VulkanUtility::CheckInstanceExtensionSupport(&instanceExtensions))
	{
		throw std::runtime_error("VkInstance �͕K�v�Ȋg���@�\���T�|�[�g���Ă��܂���I");
	}

	// �L���Ȋg���@�\�̐��Ɩ��O�̔z���ݒ肷��
	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// �o���f�[�V�������L���ȏꍇ�A�L���ȃ��C���[�̐��Ɩ��O�̔z���ݒ肷��
	if (validationEnabled)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}

	auto hoge = createInfo;
	
    return hoge;//�����𔲂����ppEnabledLayerNames��0xdddddddddd�ɂȂ�
}








bool VulkanUtility::CheckValidationLayerSupport(const std::vector<const char*> validationLayers)
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

bool VulkanUtility::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	// Vulkan�̃C���X�^���X���T�|�[�g���闘�p�\�Ȋg���@�\�̐����擾����
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// ���p�\�Ȋg���@�\�̏���ێ����邽�߂�VkExtensionProperties�̃��X�g���쐬����
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// �w�肳�ꂽ�g���@�\�����p�\�Ȋg���@�\���X�g�Ɋ܂܂�Ă��邩�`�F�b�N����
	for (const auto& checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// �g���@�\������v���邩�ǂ�����strcmp�Ŕ�r����
			if (strcmp(checkExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// �w�肳�ꂽ�g���@�\��������Ȃ��ꍇ��false��Ԃ�
		if (!hasExtension)
		{
			return false;
		}
	}

	// ���ׂĂ̎w�肳�ꂽ�g���@�\�����������ꍇ��true��Ԃ�
	return true;
}
