#include "IndexBuffer.h"

VIndexBuffer::VIndexBuffer() :
	VBufferBase(vk::BufferUsageFlagBits::eIndexBuffer,	// バッファの使用用途
		NULL,											// 使用するバッファの必須要件
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,			// 使用するバッファの優先要件
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT)	// メモリの割り当て方式
{ 
}

VIndexBuffer::~VIndexBuffer()
{
	Cleanup();
}

void VIndexBuffer::SetData(VmaAllocator* allocator, std::vector<uint32_t>* indices, vk::Fence fence)
{

	m_VertexCount = (uint32_t)indices->size();
	vk::DeviceSize dataSize = sizeof(uint32_t) * m_VertexCount;

	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, dataSize);

	// ステージングバッファを踏んでデータを入れてもらう
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//一度ステージングバッファにデータを入れてから
	StagingBuffer.TransferDataToBuffer(indices->data(), m_Buffer, fence);	//indicesBuffer(VRAMに作られたバッファ)にコピーする
}

uint32_t VIndexBuffer::GetVertexCount()
{
	return m_VertexCount;
}

void VIndexBuffer::Cleanup()
{
	printf("インデックスバッファを解放しました");
	VBufferBase::Cleanup();
}
