#include "MeshBuffer.h"

VMesh::VMesh()
{
}

VMesh::~VMesh()
{
}

void VMesh::SetMesh(VmaAllocator allocator, Mesh mesh)
{
	SetVertex(allocator, mesh.vertices);
	SetIndex(allocator, mesh.indices);
}

VVertexBuffer VMesh::GetVertex()
{
	return m_Vertex;
}

VIndexBuffer VMesh::GetIndex()
{
	return m_Index;
}

vk::PipelineVertexInputStateCreateInfo VMesh::GetInputStateInfo()
{
	return m_InputStateInfo;
}


void VMesh::SetVertex(VmaAllocator allocator, std::vector<Vertex> vertex)
{
	m_Vertex.SetData(allocator, vertex);
}

void VMesh::SetIndex(VmaAllocator allocator, std::vector<uint32_t> index)
{
	m_Index.SetData(allocator, index);

}

void VMesh::SetInputState()
{
	std::array < vk::VertexInputBindingDescription, 1> binding;	// 数を取得したいので形式だけ配列にしておく
	binding[0].binding = 0;  // バインディングインデックス
	binding[0].stride = sizeof(Vertex);  // 頂点構造体のサイズ
	binding[0].inputRate = vk::VertexInputRate::eVertex;  // 各頂点ごとにデータを読み取る
	
	std::array<vk::VertexInputAttributeDescription, 3> attribute;	
	attribute[0] = { 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) };	// 座標	
	attribute[1] = { 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) };	// 法線	
	attribute[2] = { 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) };	// テクスチャ座標

	vk::PipelineVertexInputStateCreateInfo info;
	info.vertexBindingDescriptionCount = binding.size();
	info.pVertexBindingDescriptions = binding.data();
	info.vertexAttributeDescriptionCount = attribute.size();
	info.pVertexAttributeDescriptions = attribute.data();
}
