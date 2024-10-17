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

	// ���_���̓f�X�N���v�V������Ԃ�
	vk::PipelineVertexInputStateCreateInfo GetInputStateInfo();
private:
	uint32_t m_Size;	// �f�[�^�̌�
	

	static std::vector<vk::VertexInputBindingDescription>	m_BindingDescription;
	static std::vector<vk::VertexInputAttributeDescription> m_AttributeDescription;

};

