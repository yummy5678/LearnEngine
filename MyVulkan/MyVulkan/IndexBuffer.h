#pragma once
#include "VBufferBase.h"
#include "StagingBuffer.h"
#include "MeshObject.h"


class VIndexBuffer : public VBufferBase
{
public:
	VIndexBuffer();
	~VIndexBuffer();

	void SetData(VmaAllocator allocator, std::vector<uint32_t>&	indices);
	uint32_t GetSize();
private:
	
	uint32_t m_Size;



};

