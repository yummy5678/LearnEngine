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

	void SetData(VmaAllocator* allocator, ViewProjection& projection);
	void Update(VmaAllocator* allocator, ViewProjection& projection);

private:

};

