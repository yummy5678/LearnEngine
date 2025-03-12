#include "RenderImage.h"

RenderImage::RenderImage() :
	m_LogicalDevice(),
	m_PhysicalDevice(),
	m_ColorImage(),
	m_DepthImage(),
	m_DrawCommand(),
	m_Fence(),
	m_ImageSet(),
	m_RenderFunctions()
{
}

RenderImage::~RenderImage()
{
}

vk::Extent2D RenderImage::GetExtent()
{
	return vk::Extent2D();
}

RenderingImageSet RenderImage::GetImageSet()
{
	return m_ImageSet;
}

vk::Format RenderImage::GetColorFormat()
{
	return vk::Format();
}

vk::Format RenderImage::GetDepthFormat()
{
	return vk::Format();
}

void RenderImage::Initialize(VmaAllocator* allocator, vk::ImageCreateInfo createInfo, vk::ImageAspectFlags aspectFlag)
{
	// VMAに紐づけられているオブジェクトの情報を取得
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	m_LogicalDevice = allocatorInfo.device;
	m_PhysicalDevice = allocatorInfo.physicalDevice;

	m_ColorImage.Create(allocator, createInfo, aspectFlag);
	m_DepthImage.Create(allocator, createInfo, aspectFlag);

	m_ImageSet = { m_ColorImage.GetImageSet(), m_DepthImage.GetImageSet() };
	
	m_DrawCommand.Create(allocatorInfo.device, allocatorInfo.physicalDevice);

	// フェンスを画像の数だけ作成
	vk::FenceCreateInfo fenceInfo;
	fenceInfo.pNext;
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
	m_Fence = m_LogicalDevice.createFence(fenceInfo);

}

void RenderImage::AddDrawTask(std::shared_ptr<RenderFunction> function)
{
	// 描画する関数を保持しておく
	m_RenderFunctions.push_back(function);
}

void RenderImage::ExecuteDrawTask()
{
	// 次のインデックス
	m_LogicalDevice.waitForFences(
		{ m_Fence },	// 利用するフェンス達
		VK_TRUE,						// フェンスが全てシグナル状態になるまで待つ
		UINT64_MAX);					// 最大待機時間
	m_LogicalDevice.resetFences(m_Fence);	// フェンスを非シグナル状態にする

	

	// 描画コマンドの記録開始
	m_DrawCommand.BeginRendering(
		&m_ImageSet,
		m_Swapchain.GetImageAvailableSemaphore(),
		{ {0, 0}, m_Swapchain.GetExtent() });

	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommand.GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();

	// コマンドの記録の終了とキューへの送信
	m_DrawCommand.EndRendering(m_Fence, vk::ImageLayout::eGeneral);


}
