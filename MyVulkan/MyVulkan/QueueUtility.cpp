#include "QueueUtility.h"


/// <summary>
/// �L���[�t�@�~���[�̃C���f�b�N�X���擾����
/// </summary>
QueueFamilyIndices VulkanUtility::GetQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	QueueFamilyIndices indices;

	// �����f�o�C�X�ɑ΂��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyList.data());

	// �e�L���[�t�@�~���[�𒲂ׁA�K�v�ȃ^�C�v�̃L���[�����Ȃ��Ƃ�1�����Ă��邩�ǂ������m�F����
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// �L���[�t�@�~���[�����Ȃ��Ƃ�1�̃L���[�������Ă��邩�m�F����i�L���[���Ȃ��\��������j
		// �L���[�̓r�b�g�t�B�[���h�ŕ����̃^�C�v���`���邱�Ƃ��ł���B
		// VK_QUEUE_*_BIT�ƃr�b�g���Ƃ�AND���Z���s���A�K�v�ȃ^�C�v�������Ă��邩�m�F����
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
		}

		// �L���[�t�@�~���[���v���[���e�[�V�������T�|�[�g���Ă��邩�m�F����
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationSupport);
		// �L���[���v���[���e�[�V�����^�C�v�ł��邩�ǂ������m�F����i�O���t�B�b�N�X�ƃv���[���e�[�V�����̗����ɂȂ蓾��j
		if (queueFamily.queueCount > 0 && presentationSupport)
		{
			indices.presentationFamily = i;
		}

		// �L���[�t�@�~���[�̃C���f�b�N�X���L���ȏ�Ԃɂ��邩�ǂ������m�F���A�����ł���Ό������~����
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}

std::vector<vk::DeviceQueueCreateInfo> VulkanCreate::GetQueueInfos(vk::PhysicalDevice physicalDevice,VkSurfaceKHR surface)
{
	// �I�����������f�o�C�X�̃L���[�t�@�~���[�C���f�b�N�X���擾����
	QueueFamilyIndices queueIndex = VulkanUtility::GetQueueFamilies(physicalDevice, surface);

	// �L���[�쐬���p�̃x�N�^�[
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	//�t�@�~���[�C���f�b�N�X�p�̃Z�b�g
	std::set<int> queueFamilyIndices = { queueIndex.graphicsFamily, queueIndex.presentationFamily };

	// �_���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.pNext;
		queueCreateInfo.flags;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;	// �L���[���쐬����t�@�~���[�̃C���f�b�N�X
		queueCreateInfo.queueCount = 1;							// �쐬����L���[�̐�
		float priority = 1.0f;									// �D��x
		queueCreateInfo.pQueuePriorities = &priority;			// Vulkan�͕����̃L���[���ǂ̂悤�Ɉ�������m��K�v�����邽�߁A�D��x���w�肷�� (1���ō��D��x)

		queueCreateInfos.push_back(queueCreateInfo);
	}

	return queueCreateInfos;
}

