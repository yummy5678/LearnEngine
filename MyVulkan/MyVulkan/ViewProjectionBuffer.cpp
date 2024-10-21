#include "ViewProjectionBuffer.h"

VViewProjectionBuffer::VViewProjectionBuffer() : 
	VBufferBase(vk::BufferUsageFlagBits::eUniformBuffer, 
		VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT)
{
}

VViewProjectionBuffer::~VViewProjectionBuffer()
{
}

void VViewProjectionBuffer::SetData(VmaAllocator allocator, ViewProjection& projection)
{
	vk::DeviceSize dataSize = sizeof(ViewProjection);

	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, dataSize);

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);				//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	StagingBuffer.TransferDataToBuffer(&projection, m_Buffer);	//indicesBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����
}

void VViewProjectionBuffer::Update(ViewProjection& projection)
{
}
