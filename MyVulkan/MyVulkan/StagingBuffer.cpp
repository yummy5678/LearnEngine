#include "StagingBuffer.h"

VStagingBuffer::VStagingBuffer() :
	VBufferBase(
		vk::BufferUsageFlagBits::eTransferSrc | 				// バッファの使用用途
		vk::BufferUsageFlagBits::eTransferDst,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |					// 使用するバッファの必須要件		
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_MEMORY_PROPERTY_HOST_CACHED_BIT,						// 使用するバッファの優先要件
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |// メモリの割り当て方式
		VMA_ALLOCATION_CREATE_MAPPED_BIT),
	m_LogicalDevice(VK_NULL_HANDLE),
	m_PhysicalDevice(VK_NULL_HANDLE),
	m_CommandBuffer(VK_NULL_HANDLE),
	m_CommandPool(VK_NULL_HANDLE),
	m_Queue(VK_NULL_HANDLE)
{
}

VStagingBuffer::~VStagingBuffer()
{
}

void VStagingBuffer::Initialize(VmaAllocator* allocator, vk::DeviceSize dataSize)
{
	m_Allocator = allocator;

	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);
	m_LogicalDevice		= vk::Device(allocatorInfo.device);
	m_PhysicalDevice	= vk::PhysicalDevice(allocatorInfo.physicalDevice);

	m_DataSize = dataSize;

	
	QueueFamilySelector queueFamily;
	queueFamily.Initialize(m_PhysicalDevice);
	m_CommandPool		= CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer		= CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	// グラフィックスキューの取得
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);
	



	CreateBuffer(allocator, dataSize);

	//auto stagingBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);

	//// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	//VmaAllocationCreateInfo stagingAllocateInfo;
	//stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;	// ホストからの書き込みを許可
	//stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPUからアクセス可能

	//// ステージングバッファの作成
	//auto result = vmaCreateBuffer(*allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	//// ステージングバッファとメモリの作成
	//if (result != VK_SUCCESS)
	//{
	//	throw std::runtime_error("ステージングバッファの作成に失敗しました!");
	//}
}

void VStagingBuffer::TransferDataToBuffer(void* transfarData, vk::Buffer toBuffer)
{
	if(!m_Allocator) throw std::runtime_error("先にステージングバッファの初期化を行ってください!");

	// データをステージングバッファにコピー
	MapData(transfarData, m_DataSize);

	// トランスファーバッファのデータを宛先のバッファにコピー
	SetCopyBufferCommand(m_CommandBuffer, m_Buffer, toBuffer, m_DataSize);	// 転送コマンドを作成

	// コマンドバッファを実行
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer;

	m_Queue.submit(1, &submitInfo, nullptr);
	m_Queue.waitIdle(); // 完了を待つ
}

vk::CommandPool VStagingBuffer::CreateCommandPool(vk::Device logicalDevice, uint32_t queueFamilyIndex)
{
	// コマンドプールの作成に必要な情報を設定する
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.pNext = nullptr;
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

vk::CommandBuffer VStagingBuffer::CreateCommandBuffer(vk::Device logicalDevice, vk::CommandPool commandPool)
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

void VStagingBuffer::SetCopyBufferCommand(vk::CommandBuffer commandBuffer, vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
{
	// コマンドバッファの開始
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	// コピーコマンドの作成
	vk::BufferCopy copyRegion;
	copyRegion.srcOffset = 0; // 送信元オフセット
	copyRegion.dstOffset = 0; // 受信先オフセット
	copyRegion.size = size;    // 転送サイズ
	commandBuffer.copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

	// コマンドバッファの終了
	commandBuffer.end();
}


