#include "VertexBuffer.h"


VertexBuffer::VertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer)
{

}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::SetData(VmaAllocator allocator, std::vector<Vertex> vertices)
{
	vk::DeviceSize dataSize = sizeof(Vertex) * vertices.size();

	// 頂点用のバッファ及びメモリの作成
	CreateBuffer(allocator, dataSize);

	// ステージングバッファを踏んでデータを入れてもらう
	m_Stage.Initialize(allocator, sizeof(Vertex) * vertices.size());		//一度ステージングバッファにデータを入れてから
	m_Stage.TransferDataToBuffer(vertices.data(), dataSize, m_Buffer);	//VertexBuffer(VRAMに作られたバッファ)にコピーする

}

const vk::VertexInputBindingDescription VertexBuffer::GetBindingDescription()
{
	vk::VertexInputBindingDescription binding;
	binding.binding = 0;
	binding.stride = sizeof(Vertex);
	binding.inputRate = vk::VertexInputRate::eVertex;
	return binding;
}

const vk::PipelineVertexInputStateCreateInfo VertexBuffer::InputStateInfo()
{
	vk::PipelineVertexInputStateCreateInfo info;
	info.vertexBindingDescriptionCount = 1;
	info.pVertexBindingDescriptions = &GetBindingDescription();
	info.vertexAttributeDescriptionCount = GetAttributeDescriptions().size();
	info.pVertexAttributeDescriptions = GetAttributeDescriptions().data();
	return info;
}

const std::array<vk::VertexInputAttributeDescription, 3> VertexBuffer::GetAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 3> attribute;

	// 座標
	attribute[0] = { 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) };

	// 法線
	attribute[1] = { 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) };

	// テクスチャ座標
	attribute[2] = { 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) };

	return attribute;
}
