#include "VBufferBase.h"

VBufferBase::VBufferBase(vk::BufferUsageFlags usage)
{
	//m_DataUsageはステージングバッファからデータを設定するつもりなので転送先フラグを追加
	m_Usage = usage | vk::BufferUsageFlagBits::eTransferDst;  
}

VBufferBase::~VBufferBase()
{
	Cleanup();
}

void VBufferBase::SetData(void* pData, vk::DeviceSize dataSize)
{


}

vk::Buffer VBufferBase::GetBuffer()
{
	return vk::Buffer(m_Buffer);
}

void VBufferBase::Cleanup()
{
	if (!m_Allocator) return;
	vmaDestroyBuffer(m_Allocator, m_Buffer, m_Allocation);

}

void VBufferBase::CreateBuffer(VmaAllocator allocator, vk::DeviceSize dataSize)
{
	m_Allocator = allocator;

	auto dataBufferInfo = CreateBufferInfo(dataSize, m_Usage);


	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo dataAllocateInfo;
	dataAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO;	// 自動で最適なメモリを選択(通常はGPUローカルメモリ)



	// GPU内で使う頂点バッファの作成
	auto result = vmaCreateBuffer(allocator, &dataBufferInfo, &dataAllocateInfo, &m_Buffer, &m_Allocation, nullptr);
	// ステージングバッファとメモリの作成
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("ローカルバッファの作成に失敗しました!");
	}

}

// 物理デバイス用のバッファの作成
// dataSize : 確保するデータのサイズ
VkBufferCreateInfo VBufferBase::CreateBufferInfo(vk::DeviceSize dataSize, vk::BufferUsageFlags usage)
{
	vk::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.pNext;
	bufferCreateInfo.flags;
	bufferCreateInfo.size					= dataSize;
	bufferCreateInfo.usage					= usage;			// 頂点バッファとして使用
	bufferCreateInfo.sharingMode			= m_SharingMode;	// バッファの使用を1つのキューに限定
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;

	// 論理デバイスを通してバッファを作成
	return bufferCreateInfo;
}

//void VBufferBase::AllocateBufferMemory(VmaAllocator allocator, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags)
//{
//
//
//	// イメージのメモリ要件を取得
//	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);
//
//	// 探しているメモリプロパティのフラグ
//	vk::MemoryPropertyFlags properties = propertyFlags;
//
//	// メモリの割り当て情報を設定
//	//vk::MemoryAllocateInfo allocateInfo;
//	//allocateInfo.pNext;
//	//allocateInfo.allocationSize = memoryRequirements.size;  // 画像のメモリサイズ
//	//allocateInfo.memoryTypeIndex = FindMemoryType(logicalDevice, physicalDevice, buffer, properties);   // メモリタイプ
//
//	VmaAllocationCreateInfo allocateInfo;
//	allocateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU; // CPUからGPUへの転送用
//
//
//	auto result = vmaCreateBuffer(allocator, &bufferInfo, &allocateInfo, &m_TransfarBuffer, &vertexBufferAllocation, nullptr);
//
//	// バッファとメモリの割り当て
//	if (result != VK_SUCCESS) {
//		throw std::runtime_error("バッファの作成に失敗しました!");
//	}
//
//	return allocateInfo;
//}

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




