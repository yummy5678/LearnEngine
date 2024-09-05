#include "SynchronizationGenerator.h"
#include "SwapChainUtility.h"

SynchronizationGenerator::SynchronizationGenerator()
{
	m_ClassName = "SynchronizationGenerator";
}

SynchronizationGenerator::~SynchronizationGenerator()
{

}

void SynchronizationGenerator::Create(vk::Device logicalDevice)
{
	m_bCreated = true;
	CreateSynchronisation(logicalDevice);
}

void SynchronizationGenerator::Destroy(vk::Device logicalDevice)
{
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		vkDestroySemaphore(logicalDevice, m_RenderFinished[i], nullptr);
		vkDestroySemaphore(logicalDevice, m_ImageAvailable[i], nullptr);
		vkDestroyFence	  (logicalDevice, m_DrawFences[i], nullptr);
	}
}

std::vector<vk::Semaphore> SynchronizationGenerator::GetImageAvailable()
{
	CheckCreated();
	return m_ImageAvailable;
}

std::vector<vk::Semaphore> SynchronizationGenerator::GetRenderFinished()
{
	CheckCreated();
	return m_RenderFinished;
}

std::vector<vk::Fence> SynchronizationGenerator::GetDrawFences()
{
	CheckCreated();
	return m_DrawFences;
}

void SynchronizationGenerator::CreateSynchronisation(vk::Device logicalDevice)
{
	// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X�����T�C�Y����
	m_ImageAvailable.resize(MAX_FRAME_DRAWS);
	m_RenderFinished.resize(MAX_FRAME_DRAWS);
	m_DrawFences.resize(MAX_FRAME_DRAWS);

	// �Z�}�t�H�̍쐬����ݒ肷��
	vk::SemaphoreCreateInfo semaphoreCreateInfo;

	// �t�F���X�̍쐬����ݒ肷��
	vk::FenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled; // ������ԂŃV�O�i�����ꂽ��Ԃ̃t�F���X���쐬����


	// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X���쐬���郋�[�v
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		// MAX_FRAME_DRAWS ���̃Z�}�t�H�ƃt�F���X���쐬���郋�[�v
		for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
		{
			try {
				// �Z�}�t�H�ƃt�F���X���쐬����
				m_ImageAvailable[i] = logicalDevice.createSemaphore(semaphoreCreateInfo);
				m_RenderFinished[i] = logicalDevice.createSemaphore(semaphoreCreateInfo);
				m_DrawFences[i] = logicalDevice.createFence(fenceCreateInfo);
			}
			catch (vk::SystemError& err) {
				throw std::runtime_error("Failed to create a Semaphore and/or Fence!");
			}
		}
	}
}
