#include "ViewProjectionBuffer.h"

VViewProjectionBuffer::VViewProjectionBuffer() : 
	VBufferBase(vk::BufferUsageFlagBits::eUniformBuffer,	// ユニフォームバッファに使う
		VMA_MEMORY_USAGE_AUTO_PREFER_HOST)					// ホスト側からも見れるメモリ
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

	// CPUからGPUへ情報を送るのに適したメモリ領域を作成したい
	VmaAllocationCreateInfo stagingAllocateInfo;
	stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;	// ホストからの書き込みを許可
	stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPUからアクセス可能

}

void VViewProjectionBuffer::Update(VmaAllocator allocator, ViewProjection& projection)
{
	MapData(allocator, &projection, sizeof(ViewProjection));
}

