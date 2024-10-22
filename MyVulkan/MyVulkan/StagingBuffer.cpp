#include "StagingBuffer.h"

VStagingBuffer::VStagingBuffer() :
	VBufferBase(stagingUsage,
		VMA_MEMORY_USAGE_AUTO_PREFER_HOST)
{
}

VStagingBuffer::~VStagingBuffer()
{
}

void VStagingBuffer::Initialize(VmaAllocator allocator, vk::DeviceSize dataSize)
{
	m_Allocator = allocator;
	m_LogicalDevice		= vk::Device(allocator->m_hDevice);
	m_PhysicalDevice	= vk::PhysicalDevice(allocator->GetPhysicalDevice());

	m_BufferDataSize = dataSize;

	// Get indices of queue families from device
	QueueFamilySelector queueFamily(m_PhysicalDevice);
	m_CommandPool		= CreateCommandPool(m_LogicalDevice, queueFamily.GetTransferIndex());
	m_CommandBuffer		= CreateCommandBuffer(m_LogicalDevice, m_CommandPool);
	// グラフィックスキューの取得
	m_Queue = m_LogicalDevice.getQueue(queueFamily.GetTransferIndex(), 0);

	auto stagingBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);


	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;	// ホストからの書き込みを許可
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPUからアクセス可能


	// ステージングバッファの作成
	auto result = vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	// ステージングバッファとメモリの作成
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("ステージングバッファの作成に失敗しました!");
	}

}

void VStagingBuffer::TransferDataToBuffer(void* transfarData, vk::Buffer toBuffer)
{
	if(!m_Allocator) throw std::runtime_error("先にステージングバッファの初期化を行ってください!");

	// データをステージングバッファにコピー
	MapData(m_Allocator, transfarData, m_BufferDataSize);

	// トランスファーバッファのデータを宛先のバッファにコピー
	SetCopyBufferCommand(m_CommandBuffer, m_Buffer, toBuffer, m_BufferDataSize);	// 転送コマンドを作成

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


