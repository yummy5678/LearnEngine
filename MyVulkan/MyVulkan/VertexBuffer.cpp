#include "VertexBuffer.h"
#include "PipelineDescriptions.h"


VVertexBuffer::VVertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer,	// 頂点バッファに使う
		VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE)			// デバイスローカル優先でメモリを確保
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
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//一度ステージングバッファにデータを入れてから
	StagingBuffer.TransferDataToBuffer(vertices.data(), m_Buffer);	//VertexBuffer(VRAMに作られたバッファ)にコピーする

}

uint32_t VVertexBuffer::GetSize()
{
	return m_Size;
}

vk::PipelineVertexInputStateCreateInfo VVertexBuffer::GetInputStateInfo()
{
	std::array<vk::VertexInputBindingDescription,1> BindingDescriptions =
	{
		vk::VertexInputBindingDescription
		{
		   0,                              // binding  
		   sizeof(Vertex),                 // stride   
		   vk::VertexInputRate::eVertex    // inputRate
		}
	};

	std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = 
	{
		// 座標
		vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },
		// 法線
		vk::VertexInputAttributeDescription{ 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) },
	   // テクスチャ座標
		vk::VertexInputAttributeDescription{ 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) }
	};

	return vertexInputInfo;
}
