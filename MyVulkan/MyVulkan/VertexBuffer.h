#pragma once
#include "VBufferBase.h"
#include "StagingBuffer.h"
#include "MeshObject.h"


class VertexBuffer : public VBufferBase
{
public:
	VertexBuffer();
	~VertexBuffer();

	void SetData(VmaAllocator allocator, std::vector<Vertex> vertices);

	const vk::VertexInputBindingDescription GetBindingDescription();

	const vk::PipelineVertexInputStateCreateInfo InputStateInfo();

private:
	VStagingBuffer m_Stage;


	const std::array<vk::VertexInputAttributeDescription, 3> GetAttributeDescriptions();





};

