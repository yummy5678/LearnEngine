#include "VStagingBuffer.h"

VStagingBuffer::VStagingBuffer() :
	VBufferBase(stagingUsage)
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
	m_CommandPool		= CreateCommandPool(m_LogicalDevice, m_PhysicalDevice);
	m_CommandBuffer		= CreateCommandBuffer(m_LogicalDevice, m_CommandPool);

	auto stagingBufferInfo = CreateBufferInfo(dataSize, m_DataUsage);


	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;	// ホストからの書き込みを許可
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPUからアクセス可能


	// ステージングバッファの作成
	auto result = vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocateInfo, &m_DataBuffer, &m_DataAllocation, nullptr);
	// ステージングバッファとメモリの作成
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("ステージングバッファの作成に失敗しました!");
	}



}

void VStagingBuffer::TransferDataToBuffer(void* transfarData, vk::DeviceSize dataSize, vk::Buffer toBuffer)
{
	if(!m_Allocator) throw std::runtime_error("先にステージングバッファのイニシャライズを行ってください!");

	// データをステージングバッファにコピー
	MapData(m_Allocator, transfarData, dataSize);

	// トランスファーバッファのデータを宛先のバッファにコピー



}

vk::CommandPool VStagingBuffer::CreateCommandPool(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice)
{
	// Get indices of queue families from device
	QueueFamilySelector queue(physicalDevice);

	// コマンドプールの作成に必要な情報を設定する
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = queue.GetTransferIndex();
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
	allocInfo.level = vk::CommandBufferLevel::ePrimary;  // プライマリコマンドバッファ
	allocInfo.commandBufferCount = 1;  // 1つのコマンドバッファを割り当て

	try
	{
		return logicalDevice.allocateCommandBuffers(allocInfo)[0];
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("転送用コマンドバッファの作成に失敗しました！");
	}
	
}
