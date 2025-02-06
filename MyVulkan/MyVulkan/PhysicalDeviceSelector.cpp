#include "PhysicalDeviceSelector.h"


PhysicalDeviceSelector::PhysicalDeviceSelector(vk::Instance instance)
{
    //�C���X�^���X����ڑ�����Ă��镨���f�o�C�X��S�Ď擾
    m_PhysicalDevices = instance.enumeratePhysicalDevices();



    //�����f�o�C�X���̂��������Ȃ������ꍇ
    if (m_PhysicalDevices.empty()) {
        throw std::runtime_error("Vulkan���T�|�[�g���Ă���GPU��������܂���!");
    }
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectGraphicsDevice()
{
	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (auto& device : m_PhysicalDevices)
	{
        QueueFamilySelector queueFamily(&device);
		//�f�o�C�X���g�p����g���@�\�A
		if (queueFamily.GetGraphicIndex() != NoneQueueNumber)
		{
			// �K�؂ȃf�o�C�X����������
            return { device, CreateQueueInfos({queueFamily.GetGraphicIndex()}) };
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("�`��p�Ɏg�p�ł���GPU��������܂���I");
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectComputeDevice()
{
    // �K�؂ȃf�o�C�X��������܂Ń��[�v����
    for (auto& device : m_PhysicalDevices)
    {
        QueueFamilySelector queueFamily(&device);
        if (queueFamily.GetComputeIndex() != NoneQueueNumber)
        {
            // �K�؂ȃf�o�C�X����������
            return { device, CreateQueueInfos({queueFamily.GetComputeIndex()}) };
        }
    }

    // ���p�\�ȃf�o�C�X���Ȃ��ꍇ
    throw std::runtime_error("�v�Z�p�Ɏg�p�ł���GPU��������܂���I");
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectTransferDevice()
{
    // �K�؂ȃf�o�C�X��������܂Ń��[�v����
    for (auto& device : m_PhysicalDevices)
    {
        QueueFamilySelector queueFamily(&device);
        if (queueFamily.GetTransferIndex() != NoneQueueNumber)
        {
            // �K�؂ȃf�o�C�X����������
            return { device, CreateQueueInfos({queueFamily.GetComputeIndex()}) };
        }
    }

    // ���p�\�ȃf�o�C�X���Ȃ��ꍇ
    throw std::runtime_error("�f�[�^�]���p�Ɏg�p�ł���GPU��������܂���I");
}

PhysicalDeviceContainer PhysicalDeviceSelector::SelectSwapchainDevice()
{
    // �K�؂ȃf�o�C�X��������܂Ń��[�v����
    for (auto& device : m_PhysicalDevices)
    {
        // �`��p�L���[�ƕ\���p�L���[�����݂��A
        // �g���@�\�ɂ��Ή����Ă���
        QueueFamilySelector queueFamily(&device);
        if (queueFamily.GetGraphicIndex()               != NoneQueueNumber &&
            //queueFamily.GetPresentationIndex(surface)   != NoneQueueNumber &&
            //CheckSupportSurface(device,surface) == true                     &&
            CheckExtensionNames(device, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }) == true)
        {
            // �K�؂ȃf�o�C�X����������
            //return { device, CreateQueueInfos({queueFamily.GetGraphicIndex(), queueFamily.GetPresentationIndex(surface)}) };
            return { device, CreateQueueInfos({queueFamily.GetGraphicIndex()}) };
        }
    }

    // ���p�\�ȃf�o�C�X���Ȃ��ꍇ
    throw std::runtime_error("�X���b�v�`�F�[���p�Ɏg�p�ł���GPU��������܂���I");
}

std::vector<vk::DeviceQueueCreateInfo> PhysicalDeviceSelector::CreateQueueInfos(std::set<uint32_t> queueFamilyIndices)
{
       	// �L���[�쐬���p�̃x�N�^�[
    	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    
    	// �_���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
    	for (int queueFamilyIndex : queueFamilyIndices)
    	{
    		vk::DeviceQueueCreateInfo queueCreateInfo;
    		queueCreateInfo.pNext;
    		queueCreateInfo.flags;
    		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;	// �L���[���쐬����t�@�~���[�̃C���f�b�N�X
    		queueCreateInfo.queueCount = 1;							// �쐬����L���[�̐�(1�����ł���)
    		float priority = 1.0f;									// �D��x
    		queueCreateInfo.pQueuePriorities = &priority;			// Vulkan�͕����̃L���[���ǂ̂悤�Ɉ�������m��K�v�����邽�߁A�D��x���w�肷�� (1���ō��D��x)
    
    		queueCreateInfos.push_back(queueCreateInfo);
    	}
    
    	return queueCreateInfos;
}

bool PhysicalDeviceSelector::CheckSupportSurface(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
    return !physicalDevice.getSurfaceFormatsKHR(surface).empty() ||
        !physicalDevice.getSurfacePresentModesKHR(surface).empty();
}

bool PhysicalDeviceSelector::CheckExtensionNames(vk::PhysicalDevice physicalDevice, std::vector<std::string> extensionNames)
{
    // �����f�o�C�X���T�|�[�g���Ă���g���@�\���擾
    auto availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();

    // �g���@�\�̖��O���Z�b�g�ɂ��āA���݊m�F��������
    std::set<std::string> availableExtensionsSet;
    for (const auto& extension : availableExtensions) {
        availableExtensionsSet.insert(extension.extensionName);
    }

    // �K�v�Ȋg���@�\���T�|�[�g����Ă��邩�m�F
    for (const auto& extensionName : extensionNames) {
        if (availableExtensionsSet.find(extensionName) == availableExtensionsSet.end()) {
            return false; // �K�v�Ȋg���@�\��������Ȃ�
        }
    }

    return true; // �S�Ă̕K�v�Ȋg���@�\���T�|�[�g����Ă���
}

