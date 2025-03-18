#include "VBufferBase.h"
#include "GraphicsDefine.h"
#include <iostream>

VBufferBase::VBufferBase(vk::BufferUsageFlags bufferusage,
	VkMemoryPropertyFlags requiredFlag,
	VkMemoryPropertyFlags preferredFlag,
	VmaAllocationCreateFlags allocationFlag) :
	m_pAllocator(VK_NULL_HANDLE),
	m_Buffer(VK_NULL_HANDLE),
	m_Allocation(VK_NULL_HANDLE),
	m_RequiredFlag(requiredFlag),
	m_PreferredFlag(preferredFlag),
	m_DataSize(0),
	m_AllocationFlag(allocationFlag)
{
	// m_DataUsageはステージングバッファからデータを設定するつもりなので転送先フラグを追加
	m_BufferUsage = bufferusage | vk::BufferUsageFlagBits::eTransferDst;
};

VBufferBase::~VBufferBase()
{
	//Cleanup();
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
	if (!m_pAllocator || !m_Buffer) return;

	vmaDestroyBuffer(*m_pAllocator, m_Buffer, m_Allocation);
	m_pAllocator = nullptr;
}

void VBufferBase::CreateBuffer(VmaAllocator* allocator, vk::DeviceSize dataSize)
{
	//m_pAllocator = allocator;
	m_DataSize = dataSize;

	auto dataBufferInfo = CreateBufferInfo(dataSize, m_BufferUsage, m_SharingMode);

	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.priority = 1.0f;					// 優先度
	dataAllocateInfo.flags = m_AllocationFlag;			// メモリの割り当て方式
	dataAllocateInfo.requiredFlags  = m_RequiredFlag;	// メモリの必須条件
	dataAllocateInfo.preferredFlags = m_PreferredFlag;	// メモリの優先条件
	dataAllocateInfo.pool = VK_NULL_HANDLE;				// VMAに決めさせるのでNULL
	dataAllocateInfo.memoryTypeBits = NULL;				// VMAに決めさせるのでNULL
	dataAllocateInfo.pUserData = nullptr;				// 追加で渡す情報は今のところ無いのでnull
	dataAllocateInfo.usage = VMA_MEMORY_USAGE_UNKNOWN;	// 使用しなくなった値


	// GPU内で使う頂点バッファの作成
	bool result = vmaCreateBuffer(*allocator, &dataBufferInfo, &dataAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
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

void VBufferBase::MapData(void* setData)
{
	VkResult mappingResult;

	// 確保したバッファの領域のポインタを取得
	// VMAで確保したメモリをCPUからアクセス可能にマップする
	void* mapData = nullptr;
	mappingResult = vmaMapMemory(*m_pAllocator, m_Allocation, &mapData);

	// マッピングに失敗した場合のエラーチェック
	if (mappingResult != VK_SUCCESS) 
	{
		std::cerr << "メモリーマッピングに失敗しました!" << std::endl;
		return;
	}

	// マップされたメモリ領域に、setDataからm_DataSizeバイト分のデータをコピーする
	std::memcpy(mapData, setData, m_DataSize);


	// メモリのアクセス制限を解除
	vmaUnmapMemory(*m_pAllocator, m_Allocation);
}




