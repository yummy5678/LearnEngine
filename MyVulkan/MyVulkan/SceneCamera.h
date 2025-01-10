#pragma once
#include "ViewProjection.h"
#include "ViewProjectionBuffer.h"

class SceneCamera
{
public:
	SceneCamera();
	~SceneCamera();


	void UpdateBuffer(VmaAllocator* allocator, ViewProjection viewProjection);
	vk::Buffer GetProjectionBuffer();




private:
	VViewProjectionBuffer	m_ProjectionBuffer;


};

