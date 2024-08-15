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

vk::PhysicalDevice PhysicalDeviceSelector::selectGraphicsDevice()
{
	// �K�؂ȃf�o�C�X��������܂Ń��[�v����
	for (const auto& device : m_PhysicalDevices)
	{
		//�f�o�C�X���g�p����g���@�\�A
		if (CheckDeviceSuitable(extensionManager, device, surface))
		{
			// �K�؂ȃf�o�C�X����������
			return device;
		}
	}

	// ���p�\�ȃf�o�C�X���Ȃ��ꍇ
	throw std::runtime_error("Vulkan���T�|�[�g����GPU��������܂���I");
}

vk::PhysicalDevice PhysicalDeviceSelector::selectSwapchainDevice(std::function<bool(const vk::PhysicalDevice&, vk::SurfaceKHR)> criteria)
{

    return vk::PhysicalDevice();
}

bool PhysicalDeviceSelector::CheckGraphics(vk::PhysicalDevice physicalDevice)
{
    // �f�o�C�X�̃v���p�e�B�Ƌ@�\���擾
    vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();
    vk::PhysicalDeviceFeatures deviceFeatures = physicalDevice.getFeatures();

    std::optional<uint32_t> graphicsFamily = findQueueFamilies(physicalDevice);

    // �A�v���P�[�V�����̗v���ɉ����ăf�o�C�X���K�؂��ǂ����𔻒f
    return graphicsFamily.has_value() && deviceFeatures.geometryShader;
}

std::optional<uint32_t> PhysicalDeviceSelector::findQueueFamilies(vk::PhysicalDevice device)
{
    // �f�o�C�X�̃L���[�t�@�~���[���擾
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    std::optional<uint32_t> graphicsFamily;

    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            graphicsFamily = i;
            break;
        }
    }

    return graphicsFamily;
}
