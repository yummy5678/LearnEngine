#include "RenderImage.h"

RenderImage::RenderImage() :
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_ImageExtent(0),
	m_ColorImage(
		vk::ImageUsageFlagBits::eColorAttachment | 
		vk::ImageUsageFlagBits::eTransferDst |
		vk::ImageUsageFlagBits::eTransferSrc,
		vk::ImageAspectFlagBits::eColor,
		(vk::Format)VulkanDefine.UseColorFormat,
		vk::MemoryPropertyFlagBits::eDeviceLocal
	),
	m_DepthImage(
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::ImageAspectFlagBits::eDepth,
		(vk::Format)VulkanDefine.UseDepthFormat,
		vk::MemoryPropertyFlagBits::eDeviceLocal
	),
	//m_ImageAspectFlag(),
	m_DrawCommand(),
	m_Fence(VK_NULL_HANDLE),
	m_ImageSet(),
	m_RenderFunctions()
{
}

RenderImage::~RenderImage()
{
	Cleanup();
}

vk::Device RenderImage::GetLogicalDevice()
{
	return m_LogicalDevice;
}

vk::Extent2D RenderImage::GetExtent()
{
	return { m_ColorImage.GetExtent().width, m_ColorImage.GetExtent().height };
}

RenderingImageSet RenderImage::GetImageSet()
{
	return m_ImageSet;
}

vk::Format RenderImage::GetColorFormat()
{
	return m_ColorImage.GetFormat();
}

vk::Format RenderImage::GetDepthFormat()
{
	return m_DepthImage.GetFormat();
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
	// フェンスのシグナル待ち
	m_LogicalDevice.waitForFences(
		{ m_Fence },						// 利用するフェンス達
		VK_TRUE,							// フェンスが全てシグナル状態になるまで待つ
		MAX_WAIT_TIME);						// 最大待機時間
	m_LogicalDevice.resetFences(m_Fence);	// フェンスを非シグナル状態にする

	

	// 描画コマンドの記録開始
	m_DrawCommand.BeginRendering(
		&m_ImageSet,
		{ {0, 0}, m_ImageExtent });

	// オブジェクトをパイプラインを通して描画
	for (auto& function : m_RenderFunctions)
	{
		(*function)(m_DrawCommand.GetBuffer(), &(*function));
	}
	m_RenderFunctions.clear();

	// コマンドの記録の終了とキューへの送信
	m_DrawCommand.EndRendering(m_Fence, VK_NULL_HANDLE, VK_NULL_HANDLE, vk::ImageLayout::eTransferSrcOptimal);


}

void RenderImage::WriteImage(std::string fileName)
{
	GraphicsUtility::WriteImage(&m_ColorImage, fileName, m_Fence);
}

void RenderImage::Cleanup()
{
	// NULLチェック
	if (m_LogicalDevice == VK_NULL_HANDLE) return; 
	if (m_PhysicalDevice == VK_NULL_HANDLE) return;

	printf("RendererBaseを解放します\n");

	// コマンドの解放
	m_DrawCommand.Destroy();

	// イメージバッファの解放
	m_DepthImage.Cleanup();
	m_ColorImage.Cleanup();

	// 同期オブジェクトの解放
	if (m_Fence != VK_NULL_HANDLE)
	{
		m_LogicalDevice.destroyFence(m_Fence);
		m_Fence = VK_NULL_HANDLE;
	}


	// メンバ変数の初期化
	m_RenderFunctions.clear();
	m_ImageSet = {};
	m_ImageExtent = 0;
	m_PhysicalDevice = VK_NULL_HANDLE;
	m_LogicalDevice = VK_NULL_HANDLE;
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
