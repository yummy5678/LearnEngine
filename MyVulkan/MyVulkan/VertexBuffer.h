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


private:
	VStagingBuffer m_Stage;
	uint32_t m_Size;	// ƒf[ƒ^‚ÌŒÂ”
	


};

