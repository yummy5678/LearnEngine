#include "VBufferBase.h"

VBufferBase::VBufferBase(vk::BufferUsageFlags bufferusage, VmaMemoryUsage memoryUsage, VmaAllocationCreateFlagBits allocationFlag) :
	m_Allocator(VK_NULL_HANDLE),
	m_Buffer(VK_NULL_HANDLE),
	m_Allocation(VK_NULL_HANDLE),
	m_MemoryUsage(VMA_MEMORY_USAGE_UNKNOWN),
	m_DataSize(0),
	m_AllocationFlag(VMA_ALLOCATION_CREATE_FLAG_BITS_MAX_ENUM)
{
	// m_DataUsageはステージングバッファからデータを設定するつもりなので転送先フラグを追加
	m_BufferUsage = bufferusage | vk::BufferUsageFlagBits::eTransferDst;
	m_MemoryUsage = memoryUsage;
	m_AllocationFlag = allocationFlag;
};

VBufferBase::~VBufferBase()
{
	//Cleanup();
}

void VBufferBase::SetData(void* pData, vk::DeviceSize dataSize)
{


}

vk::Buffer VBufferBase::GetBuffer()
{
	return vk::Buffer(m_Buffer);
}

vk::DeviceSize VBufferBase::GetDataSize()
{
	return m_DataSize;
}

void VBufferBase::Cleanup()
{
	if (!m_Allocator || !m_Buffer) return;

	vmaDestroyBuffer(*m_Allocator, m_Buffer, m_Allocation);
	m_Allocator = nullptr;
}

void VBufferBase::CreateBuffer(VmaAllocator* allocator, vk::DeviceSize dataSize)
{
	m_Allocator = allocator;
	m_DataSize = dataSize;

	auto dataBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);


	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.priority = 1.0f;
	dataAllocateInfo.flags = m_AllocationFlag;
	dataAllocateInfo.usage = m_MemoryUsage;	// 自動で最適なメモリを選択(通常はGPUローカルメモリ)
	dataAllocateInfo.pool = VK_NULL_HANDLE;
	dataAllocateInfo.memoryTypeBits = NULL;
	dataAllocateInfo.preferredFlags = NULL;
	dataAllocateInfo.requiredFlags  = NULL;
	dataAllocateInfo.pUserData = nullptr;

	// GPU内で使う頂点バッファの作成
	auto result = vmaCreateBuffer(*allocator, &dataBufferInfo, &dataAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	// ステージングバッファとメモリの作成
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("ローカルバッファの作成に失敗しました!");
	}

}

// 物理デバイス用のバッファの作成
// dataSize : 確保するデータのサイズ
VkBufferCreateInfo VBufferBase::CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags bufferUsage, vk::SharingMode mode)
{
	vk::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.pNext;
	bufferCreateInfo.flags;
	bufferCreateInfo.size					= dataSize;
	bufferCreateInfo.usage					= bufferUsage;		
	bufferCreateInfo.sharingMode			= mode;		// バッファの使用を1つのキューに限定
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;

	// 論理デバイスを通してバッファを作成
	return bufferCreateInfo;
}

uint32_t VBufferBase::FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType)
{
	// バッファのメモリ要件を取得
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);
	auto typeFilter = memoryRequirements.memoryTypeBits;

	// 物理デバイス(GPU)からメモリプロパティを取得。
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		// typeFilterは、ビットマスクでメモリタイプをフィルタリングするためのもの。
		// ここでは、typeFilterのビットがi番目のメモリタイプに対応するかどうかを確認しています。
		// (1 << i)は、iビット目のビットを1にしたビットマスクです。
		// typeFilter & (1 << i) は、iビット目のビットが立っているかを確認します。
		if ((typeFilter & (1 << i)) &&
			(memoryProperties.memoryTypes[i].propertyFlags & findType) == findType)
		{
			return i;
		}
	}

	// エラーメッセージ
	throw std::runtime_error("適切なメモリタイプを見つけられませんでした!");
}

void VBufferBase::MapData(void* setData, vk::DeviceSize dataSize)
{
	// 確保したバッファの領域のポインタを取得
	void* mapData;
	vmaMapMemory(*m_Allocator, m_Allocation, &mapData); // 修正: *allocator に変更

	// 頂点データの情報を取得したバッファにコピー
	memcpy(mapData, setData, dataSize);

	// メモリのアクセス制限を解除
	vmaUnmapMemory(*m_Allocator, m_Allocation);
}




