#include "RenderImage.h"

RenderImage::RenderImage() :
	m_LogicalDevice(),
	m_PhysicalDevice(),
	m_ColorImage(vk::ImageAspectFlagBits::eColor),
	m_DepthImage(vk::ImageAspectFlagBits::eDepth),
	m_ImageExtent(),
	m_ImageFormat(vk::Format::eR8G8B8A8Unorm),
	m_ImageAspectFlag(),
	m_ColorImageUsage(vk::ImageUsageFlagBits::eColorAttachment),
	m_DepthImageUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment),
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

void RenderImage::Initialize(VmaAllocator* allocator, vk::Extent2D extent)
{
	// VMAに紐づけられているオブジェクトの情報を取得
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	m_LogicalDevice = allocatorInfo.device;
	m_PhysicalDevice = allocatorInfo.physicalDevice;

	m_ImageExtent = extent;


	m_ColorImage.Initialize(allocator, extent);
	m_DepthImage.Initialize(allocator, extent);

	//m_ImageSet = { m_ColorImage.GetImageSet(), m_DepthImage.GetImageSet() };
	
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
		{ m_Fence },						// 利用するフェンス達
		VK_TRUE,							// フェンスが全てシグナル状態になるまで待つ
		UINT64_MAX);						// 最大待機時間
	m_LogicalDevice.resetFences(m_Fence);	// フェンスを非シグナル状態にする

	

	// 描画コマンドの記録開始
	m_DrawCommand.BeginRendering(
		&m_ImageSet,
		VK_NULL_HANDLE,							// フェンス
		{ {0, 0}, m_ImageExtent });

	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommand.GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();

	// コマンドの記録の終了とキューへの送信
	m_DrawCommand.EndRendering(m_Fence, vk::ImageLayout::eGeneral);


}

vk::ImageCreateInfo RenderImage::GetImageCreateInfo(vk::Extent2D extent, vk::Format format, vk::ImageUsageFlags usage)
{
	vk::ImageCreateInfo imageinfo;
	imageinfo.imageType = vk::ImageType::e2D;          // 画像の種類（1D～3D）
	imageinfo.extent = vk::Extent3D{ extent, 1 };        // 画像の幅
	imageinfo.mipLevels = 1;                           // ミップマップレベルの数
	imageinfo.arrayLayers = 1;                         // 画像配列のレベル数
	imageinfo.format = format;     // 画像のフォーマットタイプ
	imageinfo.tiling = vk::ImageTiling::eLinear;       // 画像データのタイル配置方法（最適な読み取りのための配置）
	imageinfo.initialLayout = vk::ImageLayout::eUndefined; // 作成時の画像データのレイアウト
	imageinfo.usage = usage; // 画像の用途を定義するビットフラグ
	imageinfo.samples = vk::SampleCountFlagBits::e1;         // マルチサンプリング用のサンプル数
	imageinfo.sharingMode = vk::SharingMode::eExclusive;		// キュー間での画像共有が可能かどうか

	return imageinfo;
}
