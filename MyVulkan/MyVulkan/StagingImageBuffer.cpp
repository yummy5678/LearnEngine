#include "StagingImageBuffer.h"

VStagingImageBuffer::VStagingImageBuffer() :
	VBufferBase(
		vk::BufferUsageFlagBits::eTransferSrc | // このバッファはコピー元として利用する
		vk::BufferUsageFlagBits::eTransferDst , // このバッファはメモリ転送先として利用できる
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |	//CPUからアクセス可能
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_MEMORY_PROPERTY_HOST_CACHED_BIT,
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
		VMA_ALLOCATION_CREATE_MAPPED_BIT),
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_CommandBuffer(VK_NULL_HANDLE),
	m_CommandPool(VK_NULL_HANDLE),
	m_ImageWidth(0),
	m_ImageHeight(0),
	m_ImageChannel(0),
	m_Queue(VK_NULL_HANDLE)

{
	//m_BufferUsage = stagingImageUsage;
	//m_MemoryUsage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
	
}

VStagingImageBuffer::~VStagingImageBuffer()
{
}

void VStagingImageBuffer::Initialize(VmaAllocator* allocator, uint32_t imageWidth, uint32_t imageHeight, uint32_t imageChannel)
{
	m_Allocator = allocator;

	// アロケーターに登録されている情報を取得
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	m_LogicalDevice = vk::Device(allocatorInfo.device);
	m_PhysicalDevice = vk::PhysicalDevice(allocatorInfo.physicalDevice);
	m_ImageWidth = imageWidth;
	m_ImageHeight = imageHeight;
	m_ImageChannel = imageChannel;

	// 画像のデータサイズ
	m_DataSize = imageWidth * imageHeight * imageChannel;

	// 転送用キューの取得
	QueueFamilySelector queueFamily(&m_PhysicalDevice);
	m_CommandPool = CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer = CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);

	auto stagingBufferInfo = CreateBufferInfo(m_DataSize, m_BufferUsage, m_SharingMode);


	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.priority = 1.0f;
	stagingAllocateInfo.flags = m_AllocationFlag;
	stagingAllocateInfo.requiredFlags = m_RequiredFlag;
	stagingAllocateInfo.preferredFlags = m_PreferredFlag;
	stagingAllocateInfo.pool = VK_NULL_HANDLE;
	stagingAllocateInfo.memoryTypeBits = NULL;
	stagingAllocateInfo.pUserData = nullptr;
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;

	// ステージングバッファの作成
	vmaCreateBuffer(*allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_Buffer, &m_Allocation, nullptr);


}

void VStagingImageBuffer::TransferDataToImageBuffer(void* transfarData, vk::Image toBuffer)
{
	if (m_Allocator == nullptr) throw std::runtime_error("先にステージングバッファの初期化を行ってください!");

	VmaAllocationInfo allocInfo;
	vmaGetAllocationInfo(*m_Allocator, m_Allocation, &allocInfo);

	// データをステージングバッファにコピー
	VBufferBase::MapData(transfarData, m_DataSize);

	// 転送用バッファのデータを宛先のイメージバッファにコピー
	SetCopyToImageCommand(m_CommandBuffer, m_Buffer, toBuffer, m_ImageWidth, m_ImageHeight);	// 転送コマンドを作成

	// コマンドバッファを実行
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;				// 使うコマンドは1つだけで充分
	submitInfo.pCommandBuffers = &m_CommandBuffer;	// 作成したコマンドバッファをセット

	m_Queue.submit(1, &submitInfo, nullptr);		// コマンドをGPUのキューに送信
	m_Queue.waitIdle();								// 送ったコマンドキューの完了を待つ
}

vk::CommandPool VStagingImageBuffer::CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex)
{
	// コマンドプールの作成に必要な情報を設定する
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = queueFamilyIndex;
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient;	// コマンドバッファのリセットを許可する場合はフラグを追加する


	// グラフィックスキューファミリー用のコマンドプールを作成する
	try
	{
		return logicalDevice.createCommandPool(poolInfo);
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("転送用コマンドプールの作成に失敗しました！");
	}
}

vk::CommandBuffer VStagingImageBuffer::CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool)
{
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;	// プライマリコマンドバッファ
	allocInfo.commandBufferCount = 1;					// 1つのコマンドバッファを割り当て

	try
	{
		return logicalDevice.allocateCommandBuffers(allocInfo)[0];
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("転送用コマンドバッファの作成に失敗しました！");
	}

}

void VStagingImageBuffer::SetCopyToImageCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Image dstImage, uint32_t imageWidth, uint32_t imageHeight)
{
	// コマンドバッファの開始
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	// イメージのレイアウトを変更(Undefined → TransferDstOptimal)
	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = vk::ImageLayout::eUndefined;
	barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.image = dstImage;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.srcAccessMask = {};
	barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
		vk::PipelineStageFlagBits::eTransfer,
		{}, {}, {}, barrier);


	// ステージングバッファからイメージへコピー
	vk::BufferImageCopy copyRegion;
	copyRegion.bufferOffset = 0;
	copyRegion.bufferRowLength = 0;			// "0"を指定しておくと自動的にExtentの値が入る
	copyRegion.bufferImageHeight = 0;		// "0"を指定しておくと自動的にExtentの値が入る
	copyRegion.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	copyRegion.imageSubresource.mipLevel = 0;
	copyRegion.imageSubresource.baseArrayLayer = 0;
	copyRegion.imageSubresource.layerCount = 1;
	copyRegion.imageOffset = vk::Offset3D{ 0, 0, 0 };
	copyRegion.imageExtent = vk::Extent3D{ imageWidth, imageHeight, 1 };

	commandBuffer.copyBufferToImage(srcBuffer, dstImage, vk::ImageLayout::eTransferDstOptimal, { copyRegion });

	// イメージのレイアウトを変更（TransferDstOptimal → ShaderReadOnlyOptimal）
	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
		vk::PipelineStageFlagBits::eFragmentShader,
		{}, {}, {}, barrier);

	// コマンドバッファの終了
	commandBuffer.end();
}

//void VStagingImageBuffer::MapData(VmaAllocator* allocator, void* setData, vk::DeviceSize dataSize)
//{
//	// 確保したバッファの領域のポインタを取得
//	void* mapData = nullptr;
//
//	vmaMapMemory(*allocator, m_Allocation, &mapData);
//
//	// 頂点データの情報を取得したバッファにコピー
//	std::memcpy(mapData, setData, dataSize);
//
//	// メモリのアクセス制限を解除
//	vmaUnmapMemory(*allocator, m_Allocation);
//
//}
