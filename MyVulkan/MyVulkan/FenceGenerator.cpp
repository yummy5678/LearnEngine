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
	//中身が作成されていないなら破棄も行わない
	if (m_bCreated == false) return;
	m_bCreated = false;

	//フェンスの破棄
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
	// 作成分領域を確保する
	std::vector<vk::Fence> fenes;
	fenes.resize(fenceCount);

	// フェンスの作成情報を設定する
	vk::FenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.pNext;
	fenceCreateInfo.flags;

	try
	{
		for (int i = 0; i < fenceCount; i++)
		{
			// フェンスを作成する
			fenes[i] = logicalDevice.createFence(fenceCreateInfo);
		}
		return fenes;
	}
	catch (vk::SystemError& err)
	{
		throw std::runtime_error("フェンスの作成に失敗しました！");
	}
}
