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
	//中身が作成されていないなら破棄も行わない
	if (m_bCreated == false) return;
	m_bCreated = false;

	//セマフォの破棄
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
	// 作成分領域を確保する
	std::vector<vk::Semaphore> semaphores;
	semaphores.resize(semaphoreCount);

	// セマフォの作成情報を設定する
	vk::SemaphoreCreateInfo semaphoreCreateInfo;
	semaphoreCreateInfo.pNext;
	semaphoreCreateInfo.flags;

	try 
	{
		for (int i = 0; i < semaphoreCount; i++)
		{
			// セマフォを作成する
			semaphores[i] = logicalDevice.createSemaphore(semaphoreCreateInfo);
		}
	}
	catch (vk::SystemError& err) 
	{
		throw std::runtime_error("セマフォの作成に失敗しました！");
	}

}
