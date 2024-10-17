#pragma once
#include "VBufferBase.h"
#include "StagingBuffer.h"
#include "MeshObject.h"


class VVertexBuffer : public VBufferBase
{
public:
	VVertexBuffer();
	~VVertexBuffer();

	void		SetData(VmaAllocator allocator, std::vector<Vertex>& vertices);
	uint32_t	GetSize();

	// 頂点入力デスクリプションを返す
	vk::PipelineVertexInputStateCreateInfo GetInputStateInfo();
private:
	uint32_t m_Size;	// データの個数
	

	static std::vector<vk::VertexInputBindingDescription>	m_BindingDescription;
	static std::vector<vk::VertexInputAttributeDescription> m_AttributeDescription;

};

