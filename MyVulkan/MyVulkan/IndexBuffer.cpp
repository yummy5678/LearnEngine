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
}

void VIndexBuffer::SetData(VmaAllocator* allocator, std::vector<uint32_t>& indices)
{

	m_Size = (uint32_t)indices.size();
	vk::DeviceSize dataSize = sizeof(uint32_t) * m_Size;

	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, dataSize);

	// ステージングバッファを踏んでデータを入れてもらう
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//一度ステージングバッファにデータを入れてから
	StagingBuffer.TransferDataToBuffer(indices.data(), m_Buffer);	//indicesBuffer(VRAMに作られたバッファ)にコピーする
}

uint32_t VIndexBuffer::GetSize()
{
	return m_Size;
}
