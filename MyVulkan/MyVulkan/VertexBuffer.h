#pragma once
#include "VBufferBase.h"
#include "StagingBuffer.h"
#include "MeshObject.h"
#include "PipelineDescriptions.h"

class VVertexBuffer : public VBufferBase
{
public:
	VVertexBuffer();
	~VVertexBuffer();

	void		SetData(VmaAllocator* allocator, std::vector<Vertex>* vertices);
	size_t	GetVertexCount();

	// 頂点入力デスクリプションを返す
	vk::PipelineVertexInputStateCreateInfo GetInputStateInfo();
private:
	size_t m_VertexCount;	// データの個数
	

	static std::vector<vk::VertexInputBindingDescription>	m_BindingDescription;
	static std::vector<vk::VertexInputAttributeDescription> m_AttributeDescription;

};

