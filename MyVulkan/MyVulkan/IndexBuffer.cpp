#include "IndexBuffer.h"

VIndexBuffer::VIndexBuffer() :
	VBufferBase(vk::BufferUsageFlagBits::eIndexBuffer)
{
}

VIndexBuffer::~VIndexBuffer()
{
}

void VIndexBuffer::SetData(VmaAllocator allocator, std::vector<uint32_t>& indices)
{

	m_Size = indices.size();
	vk::DeviceSize dataSize = sizeof(uint32_t) * indices.size();

	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, dataSize);

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	StagingBuffer.TransferDataToBuffer(indices.data(), m_Buffer);	//indicesBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����
}

uint32_t VIndexBuffer::GetSize()
{
	return m_Size;
}
