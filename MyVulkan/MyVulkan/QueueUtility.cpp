#include "QueueUtility.h"


QueueFamilyGenerator::QueueFamilyGenerator()
{
	m_ClassName = "QueueFamilyGenerator";
}

QueueFamilyGenerator::~QueueFamilyGenerator()
{
}

void QueueFamilyGenerator::Create(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	m_bCreated = true;	//�쐬�֐���ʂ����t���O�𗧂Ă�
	m_GraphicsFamilyIndex = SearchGraphicsFamily(physicalDevice, surface);	// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����

	m_PresentationFamilyIndex = SearchPresentationFamily(physicalDevice, surface);




	m_QueueCreateInfos = CreateQueueInfos(physicalDevice, surface);
}

int QueueFamilyGenerator::GetGraphicIndex()
{
	CheckCreated();	//�쐬�֐���ʂ��O�Ɏg�p����ƃG���[
	return m_GraphicsFamilyIndex;
}

int QueueFamilyGenerator::GetPresentationIndex()
{
	CheckCreated();	//�쐬�֐���ʂ��O�Ɏg�p����ƃG���[
	return m_PresentationFamilyIndex;
}

std::vector<vk::DeviceQueueCreateInfo> QueueFamilyGenerator::GetQueueInfos()
{
	CheckCreated();	//�쐬�֐���ʂ��O�Ɏg�p����ƃG���[
	return m_QueueCreateInfos;
}

int QueueFamilyGenerator::SearchGraphicsFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	// �����f�o�C�X�ɔ�����Ă��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	const auto queueFamilyList = physicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. �L���[�t�@�~���[��1�ł��L���[�������Ă��邩�m�F����
		// 2. �����_�����O���ł���L���[���AAND���Z���s���m�F����
		if (queueFamilyList[i].queueCount != 0 && 
			queueFamilyList[i].queueFlags & vk::QueueFlagBits::eGraphics)
		{
			index = i;	// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
			break;
		}
	}

	printf("�O���t�B�b�N�X�L���[�̒T�����ʁF %d\n", index);
	return index;

}

int QueueFamilyGenerator::SearchPresentationFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	// �����f�o�C�X�ɔ�����Ă��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	const auto queueFamilyList = physicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. �L���[�t�@�~���[��1�ł��L���[�������Ă��邩�m�F����
		// 2. �摜�̕\���@�\�����邩�m�F����
		if (queueFamilyList[i].queueCount != 0 && physicalDevice.getSurfaceSupportKHR(i, surface))
		{
			index = i;	// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
			break;
		}
	}

	printf("�v���[���e�[�V�����L���[�̒T�����ʁF %d\n", index);
	return index;
}

std::vector<vk::DeviceQueueCreateInfo> QueueFamilyGenerator::CreateQueueInfos(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
{
	//�t�@�~���[�C���f�b�N�X�p�̃Z�b�g
	std::set<int> queueFamilyIndices = { m_GraphicsFamilyIndex, m_PresentationFamilyIndex };

	// �L���[�쐬���p�̃x�N�^�[
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	// �_���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
	for (int queueFamilyIndex : queueFamilyIndices)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo;
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

