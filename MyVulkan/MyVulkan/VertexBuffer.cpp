#include "VertexBuffer.h"


VVertexBuffer::VVertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer)
{

}

VVertexBuffer::~VVertexBuffer()
{
}

void VVertexBuffer::SetData(VmaAllocator allocator, std::vector<Vertex>& vertices)
{
	m_Size = vertices.size();
	vk::DeviceSize dataSize = sizeof(Vertex) * m_Size;


	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, dataSize);

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	m_Stage.Initialize(allocator, dataSize);					//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	m_Stage.TransferDataToBuffer(vertices.data(), m_Buffer);	//VertexBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����

}

uint32_t VVertexBuffer::GetSize()
{
	return m_Size;
}
