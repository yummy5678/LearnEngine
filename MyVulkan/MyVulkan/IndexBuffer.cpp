#include "IndexBuffer.h"

VIndexBuffer::VIndexBuffer() :
	VBufferBase(vk::BufferUsageFlagBits::eIndexBuffer)
{
}

VIndexBuffer::~VIndexBuffer()
{
}

void VIndexBuffer::SetData(VmaAllocator allocator, std::vector<uint32_t>& indices)
{

	m_Size = indices.size();
	vk::DeviceSize dataSize = sizeof(uint32_t) * indices.size();

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
