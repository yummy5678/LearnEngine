#pragma once
#include "VBufferBase.h"
#include "StagingBuffer.h"
#include "ViewProjection.h"

class VViewProjectionBuffer 
	: public VBufferBase
{
public:
	VViewProjectionBuffer();
	~VViewProjectionBuffer();

	void Initialize(VmaAllocator* allocator);

	void SetData(VmaAllocator* allocator, ViewProjection& projection);
	//void UpdateAll(VmaAllocator* allocator, ViewProjection& projection);

private:

};

