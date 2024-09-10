#include "FenceGenerator.h"

FenceGenerator::FenceGenerator():
	m_LogicalDevice(VK_NULL_HANDLE),
	m_Fences()
{
	m_ClassName = "FenceGenerator";
}

FenceGenerator::~FenceGenerator()
{
}

void FenceGenerator::Create(vk::Device logicalDevice, uint32_t fenceCount)
{
	m_bCreated = true;

	m_LogicalDevice = logicalDevice;

	m_Fences = CreateFences(logicalDevice, fenceCount);
}

void FenceGenerator::Destroy()
{
	//���g���쐬����Ă��Ȃ��Ȃ�j�����s��Ȃ�
	if (m_bCreated == false) return;
	m_bCreated = false;

	//�t�F���X�̔j��
	for (auto& fence : m_Fences)
	{
		m_LogicalDevice.destroyFence(fence);
	}
}

std::vector<vk::Fence> FenceGenerator::GetFence()
{
	CheckCreated();
	return m_Fences;
}

std::vector<vk::Fence> FenceGenerator::CreateFences(vk::Device logicalDevice, uint32_t fenceCount)
{
	// �쐬���̈���m�ۂ���
	std::vector<vk::Fence> fenes;
	fenes.resize(fenceCount);

	// �t�F���X�̍쐬����ݒ肷��
	vk::FenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.pNext;
	fenceCreateInfo.flags;

	try
	{
		for (int i = 0; i < fenceCount; i++)
		{
			// �t�F���X���쐬����
			fenes[i] = logicalDevice.createFence(fenceCreateInfo);
		}
		return fenes;
	}
	catch (vk::SystemError& err)
	{
		throw std::runtime_error("�t�F���X�̍쐬�Ɏ��s���܂����I");
	}
}
