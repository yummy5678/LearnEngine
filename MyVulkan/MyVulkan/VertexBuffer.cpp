#include "VertexBuffer.h"


VVertexBuffer::VVertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer)
{

}

VVertexBuffer::~VVertexBuffer()
{
}

void VVertexBuffer::SetData(VmaAllocator allocator, std::vector<Vertex>& vertices)
{
	m_Size = vertices.size();
	vk::DeviceSize dataSize = sizeof(Vertex) * m_Size;


	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, dataSize);

	// ステージングバッファを踏んでデータを入れてもらう
	m_Stage.Initialize(allocator, dataSize);					//一度ステージングバッファにデータを入れてから
	m_Stage.TransferDataToBuffer(vertices.data(), m_Buffer);	//VertexBuffer(VRAMに作られたバッファ)にコピーする

}

uint32_t VVertexBuffer::GetSize()
{
	return m_Size;
}
