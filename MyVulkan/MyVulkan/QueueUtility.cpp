#include "QueueUtility.h"


QueueFamilySelector::QueueFamilySelector(vk::PhysicalDevice physicalDevice)
{
	m_PhysicalDevice = physicalDevice;
}

QueueFamilySelector::~QueueFamilySelector()
{
}

int QueueFamilySelector::GetGraphicIndex()
{
	if(m_GraphicsFamilyIndex == Number_NoneQueue) SearchGraphicsFamily();
	return m_GraphicsFamilyIndex;
}

int QueueFamilySelector::GetComputeIndex()
{
	if (m_ComputeFamilyIndex == Number_NoneQueue) SearchComputeFamily();
	return m_ComputeFamilyIndex;
}

int QueueFamilySelector::GetTransferIndex()
{
	if (m_TransferFamilyIndex == Number_NoneQueue) SearchTransferFamily();
	return m_TransferFamilyIndex;
}

int QueueFamilySelector::GetPresentationIndex(vk::SurfaceKHR surface)
{
	if (m_PresentationFamilyIndex == Number_NoneQueue) SearchPresentationFamily(surface);
	return m_PresentationFamilyIndex;
}



int QueueFamilySelector::SearchGraphicsFamily()
{
	// �����f�o�C�X�ɔ�����Ă��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
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

	printf("�`��p�L���[�̒T�����ʁF %d\n", index);
	return index;
}

int QueueFamilySelector::SearchComputeFamily()
{
	// �����f�o�C�X�ɔ�����Ă��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. �L���[�t�@�~���[��1�ł��L���[�������Ă��邩�m�F����
		// 2. �v�Z�Ɏg�p�ł���L���[���AAND���Z���s���m�F����
		if (queueFamilyList[i].queueCount != 0 &&
			queueFamilyList[i].queueFlags & vk::QueueFlagBits::eCompute)
		{
			index = i;	// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
			break;
		}
	}

	printf("�v�Z�p�L���[�̒T�����ʁF %d\n", index);
	return index;
}

int QueueFamilySelector::SearchTransferFamily()
{
	// �����f�o�C�X�ɔ�����Ă��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. �L���[�t�@�~���[��1�ł��L���[�������Ă��邩�m�F����
		// 2. �]���p�Ɏg�p�ł���L���[���AAND���Z���s���m�F����
		if (queueFamilyList[i].queueCount != 0 &&
			queueFamilyList[i].queueFlags & vk::QueueFlagBits::eTransfer)
		{
			index = i;	// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
			break;
		}
	}

	printf("�]���p�L���[�̒T�����ʁF %d\n", index);
	return index;
}

int QueueFamilySelector::SearchPresentationFamily(vk::SurfaceKHR surface)
{
	// �����f�o�C�X�ɔ�����Ă��邷�ׂẴL���[�t�@�~���[�v���p�e�B�����擾����
	const auto queueFamilyList = m_PhysicalDevice.getQueueFamilyProperties();
	int index = Number_NoneQueue;

	for (int i = 0; i < queueFamilyList.size(); i++)
	{
		// 1. �L���[�t�@�~���[��1�ł��L���[�������Ă��邩�m�F����
		// 2. �摜�̕\���@�\�����邩�m�F����
		if (queueFamilyList[i].queueCount > 0 && m_PhysicalDevice.getSurfaceSupportKHR(i, surface))
		{
			index = i;	// �L���[�t�@�~���[���L���ł���΁A���̃C���f�b�N�X���擾����
			break;
		}
	}

	printf("�v���[���e�[�V�����L���[�̒T�����ʁF %d\n", index);
	return index;
}

//std::vector<vk::DeviceQueueCreateInfo> QueueFamilySelector::CreateQueueInfos()
//{
//	//�t�@�~���[�C���f�b�N�X�p�̃Z�b�g
//	std::set<int> queueFamilyIndices = { m_GraphicsFamilyIndex, m_PresentationFamilyIndex };
//
//	// �L���[�쐬���p�̃x�N�^�[
//	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
//
//	// �_���f�o�C�X�ō쐬����K�v������L���[�Ƃ��̏���ݒ肷��
//	for (int queueFamilyIndex : queueFamilyIndices)
//	{
//		vk::DeviceQueueCreateInfo queueCreateInfo;
//		queueCreateInfo.pNext;
//		queueCreateInfo.flags;
//		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;	// �L���[���쐬����t�@�~���[�̃C���f�b�N�X
//		queueCreateInfo.queueCount = 1;							// �쐬����L���[�̐�
//		float priority = 1.0f;									// �D��x
//		queueCreateInfo.pQueuePriorities = &priority;			// Vulkan�͕����̃L���[���ǂ̂悤�Ɉ�������m��K�v�����邽�߁A�D��x���w�肷�� (1���ō��D��x)
//
//		queueCreateInfos.push_back(queueCreateInfo);
//	}
//
//	return queueCreateInfos;
//}

