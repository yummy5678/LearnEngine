#include "FenceGenerator.h"

FenceGenerator::FenceGenerator():
	m_LogicalDevice(VK_NULL_HANDLE),
	m_Fences()
{
}

FenceGenerator::~FenceGenerator()
{
}

void FenceGenerator::Create(vk::Device logicalDevice, size_t fenceCount)
{
	m_LogicalDevice = logicalDevice;

	m_Fences = CreateFences(logicalDevice, fenceCount);
}

void FenceGenerator::Destroy()
{
	//�t�F���X�̔j��
	for (auto& fence : m_Fences)
	{
		m_LogicalDevice.destroyFence(fence);
	}
}

std::vector<vk::Fence> FenceGenerator::GetFence()
{
	return m_Fences;
}

std::vector<vk::Fence> FenceGenerator::CreateFences(vk::Device logicalDevice, size_t fenceCount)
{
	if(logicalDevice == VK_NULL_HANDLE) throw std::runtime_error("�t�F���X�̍쐬�Ɏ��s���܂����I");

	// �쐬���̈���m�ۂ���
	std::vector<vk::Fence> fenes;
	fenes.resize(fenceCount);

	// �t�F���X�̍쐬����ݒ肷��
	vk::FenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.pNext;
	fenceCreateInfo.flags;

	for (uint32_t i = 0; i < fenceCount; i++)
	{
		// �t�F���X���쐬����
		fenes[i] = logicalDevice.createFence(fenceCreateInfo);
	}
	return fenes;
}
