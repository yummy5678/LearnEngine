#include "VertexBuffer.h"



VVertexBuffer::VVertexBuffer(): 
	m_VertexCount(0),
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer,	// 頂点バッファに使う
		NULL,											// 必須要件無し
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,			// デバイスローカル優先でメモリを確保
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT)
{

}

VVertexBuffer::~VVertexBuffer()
{
	Cleanup();
}

void VVertexBuffer::SetData(VmaAllocator* allocator, std::vector<Vertex>* vertices)
{
	m_VertexCount = vertices->size();
	vk::DeviceSize dataSize = sizeof(Vertex) * m_VertexCount;


	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, dataSize);

	// ステージングバッファを踏んでデータを入れてもらう
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//一度ステージングバッファにデータを入れてから
	StagingBuffer.TransferDataToBuffer(vertices->data(), m_Buffer);	//VertexBuffer(VRAMに作られたバッファ)にコピーする

}

size_t VVertexBuffer::GetVertexCount()
{
	return m_VertexCount;
}

void VVertexBuffer::Cleanup()
{
	printf("頂点バッファを解放します");
	VBufferBase::Cleanup();
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

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(BindingDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = BindingDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	return vertexInputInfo;
}
