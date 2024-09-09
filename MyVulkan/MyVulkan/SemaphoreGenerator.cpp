#include "SemaphoreGenerator.h"

SemaphoreGenerator::SemaphoreGenerator():
	m_LogicalDevice(VK_NULL_HANDLE),
	m_Semaphores()
{
	m_ClassName = "SemaphoreGenerator";
}

SemaphoreGenerator::~SemaphoreGenerator()
{
}

void SemaphoreGenerator::Create(vk::Device logicalDevice, uint32_t semaphoreCount)
{
	m_bCreated = true;

	m_LogicalDevice = logicalDevice;

	m_Semaphores = CreateSemaphore(logicalDevice, semaphoreCount);

}

void SemaphoreGenerator::Destroy()
{
	//���g���쐬����Ă��Ȃ��Ȃ�j�����s��Ȃ�
	if (m_bCreated == false) return;
	m_bCreated = false;

	//�Z�}�t�H�̔j��
	for (auto& semaphore : m_Semaphores)
	{
		m_LogicalDevice.destroySemaphore(semaphore);
	}
}

std::vector<vk::Semaphore> SemaphoreGenerator::GetSemaphore()
{
	CheckCreated();
	return m_Semaphores;
}

std::vector<vk::Semaphore> SemaphoreGenerator::CreateSemaphore(vk::Device logicalDevice ,uint32_t semaphoreCount)
{
	// �쐬���̈���m�ۂ���
	std::vector<vk::Semaphore> semaphores;
	semaphores.resize(semaphoreCount);

	// �Z�}�t�H�̍쐬����ݒ肷��
	vk::SemaphoreCreateInfo semaphoreCreateInfo;
	semaphoreCreateInfo.pNext;
	semaphoreCreateInfo.flags;

	try 
	{
		for (int i = 0; i < semaphoreCount; i++)
		{
			// �Z�}�t�H���쐬����
			semaphores[i] = logicalDevice.createSemaphore(semaphoreCreateInfo);
		}
	}
	catch (vk::SystemError& err) 
	{
		throw std::runtime_error("�Z�}�t�H�̍쐬�Ɏ��s���܂����I");
	}

}
