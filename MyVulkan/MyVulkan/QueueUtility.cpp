#include "QueueUtility.h"

QueueFamilyIndices QueueUtility::getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	// �����f�o�C�X�ɑ΂��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	// �e�L���[�t�@�~���[�𒲂ׁA�K�v�ȃ^�C�v�̃L���[�����Ȃ��Ƃ�1�����Ă��邩�ǂ������m�F����
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// �܂��A�L���[�t�@�~���[�����Ȃ��Ƃ�1�̃L���[�������Ă��邩�m�F����i�L���[���Ȃ��\��������j
		// �L���[�̓r�b�g�t�B�[���h�ŕ����̃^�C�v���`���邱�Ƃ��ł���BVK_QUEUE_*_BIT�ƃr�b�g���Ƃ�AND���Z���s���A�K�v�ȃ^�C�v�������Ă��邩�m�F����
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
		}

		// �L���[�t�@�~���[���v���[���e�[�V�������T�|�[�g���Ă��邩�m�F����
		VkBool32 presentationSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentationSupport);
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
