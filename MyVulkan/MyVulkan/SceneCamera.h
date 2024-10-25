#pragma once
#include "ViewProjection.h"
#include "ViewProjectionBuffer.h"

class SceneCamera
{
public:
	SceneCamera() = default;
	~SceneCamera();

	void UpdateVBuffer(VmaAllocator allocator, ViewProjection viewProjection);

	vk::DescriptorSet GetDescriptorSet();



private:
	VViewProjectionBuffer	m_ProjectionBuffer;
	vk::DescriptorSet		m_DescriptorSet;


	void CreateDescriptorSet();

};

