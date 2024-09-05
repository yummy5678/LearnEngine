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
	// MAX_FRAME_DRAWS 分のセマフォとフェンスをリサイズする
	m_ImageAvailable.resize(MAX_FRAME_DRAWS);
	m_RenderFinished.resize(MAX_FRAME_DRAWS);
	m_DrawFences.resize(MAX_FRAME_DRAWS);

	// セマフォの作成情報を設定する
	vk::SemaphoreCreateInfo semaphoreCreateInfo;

	// フェンスの作成情報を設定する
	vk::FenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled; // 初期状態でシグナルされた状態のフェンスを作成する


	// MAX_FRAME_DRAWS 分のセマフォとフェンスを作成するループ
	for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
	{
		// MAX_FRAME_DRAWS 分のセマフォとフェンスを作成するループ
		for (size_t i = 0; i < MAX_FRAME_DRAWS; i++)
		{
			try {
				// セマフォとフェンスを作成する
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
