#include "ViewProjectionBuffer.h"

VViewProjectionBuffer::VViewProjectionBuffer() : 
	VBufferBase(vk::BufferUsageFlagBits::eUniformBuffer, 
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT)
{
}

VViewProjectionBuffer::~VViewProjectionBuffer()
{
}

void VViewProjectionBuffer::SetData(VmaAllocator allocator, ViewProjection& projection)
{
	vk::DeviceSize dataSize = sizeof(ViewProjection);

	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, dataSize);

	// ステージングバッファを踏んでデータを入れてもらう
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);				//一度ステージングバッファにデータを入れてから
	StagingBuffer.TransferDataToBuffer(&projection, m_Buffer);	//indicesBuffer(VRAMに作られたバッファ)にコピーする
}

void VViewProjectionBuffer::Update(ViewProjection& projection)
{
}
