#include "ViewProjectionBuffer.h"

VViewProjectionBuffer::VViewProjectionBuffer() : 
	VBufferBase(vk::BufferUsageFlagBits::eUniformBuffer,	// ユニフォームバッファに使う
		NULL,												// 必須要件無し
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,				// デバイスローカル優先でメモリを確保
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT)
{
}

VViewProjectionBuffer::~VViewProjectionBuffer()
{
}

void VViewProjectionBuffer::Initialize(VmaAllocator* allocator)
{
	if (m_Buffer != VK_NULL_HANDLE)
	{
		// エラーメッセージ
		return;
	}

	m_DataSize = sizeof(ViewProjection);

	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, m_DataSize);
}

void VViewProjectionBuffer::SetData(VmaAllocator* allocator, ViewProjection& projection)
{
	if (m_Buffer == VK_NULL_HANDLE || m_DataSize == 0)
	{
		// エラーメッセージ
		return;
	}

	// ステージングバッファを踏んでデータを入れてもらう
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, m_DataSize);				//一度ステージングバッファにデータを入れてから
	StagingBuffer.TransferDataToBuffer(&projection, m_Buffer);	//indicesBuffer(VRAMに作られたバッファ)にコピーする

	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	//VmaAllocationCreateInfo stagingAllocateInfo;
	//stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;	// ホストからの書き込みを許可
	//stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPUからアクセス可能

}

//void VViewProjectionBuffer::UpdateDescriptorSets(VmaAllocator* allocator, ViewProjection& projection)
//{
//	MapData(&projection, sizeof(ViewProjection));
//}

