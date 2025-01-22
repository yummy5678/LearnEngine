#pragma once
#include "ViewProjection.h"
#include "ViewProjectionBuffer.h"

class SceneCamera
{
public:
	SceneCamera(VmaAllocator* allocator);
	~SceneCamera();


	void UpdateBuffer(ViewProjection viewProjection);
	vk::Buffer GetProjectionBuffer();




private:
	VmaAllocator* m_pAllocator;
	VViewProjectionBuffer	m_ProjectionBuffer;

	void Initialize();

};

