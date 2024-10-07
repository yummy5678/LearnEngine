#pragma once
#include "VBufferBase.h"
#include "MeshObject.h"


class VertexBuffer : public VBufferBase
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Create(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, std::vector<Vertex> vertices);





private:









};

