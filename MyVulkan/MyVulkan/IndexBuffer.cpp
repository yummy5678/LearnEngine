#include "IndexBuffer.h"

VIndexBuffer::VIndexBuffer() :
	VBufferBase(vk::BufferUsageFlagBits::eIndexBuffer,	// �o�b�t�@�̎g�p�p�r
		NULL,											// �g�p����o�b�t�@�̕K�{�v��
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,			// �g�p����o�b�t�@�̗D��v��
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT)	// �������̊��蓖�ĕ���
{ 
}

VIndexBuffer::~VIndexBuffer()
{
	Cleanup();
}

void VIndexBuffer::SetData(VmaAllocator* allocator, std::vector<uint32_t>* indices, vk::Fence fence)
{

	m_VertexCount = (uint32_t)indices->size();
	vk::DeviceSize dataSize = sizeof(uint32_t) * m_VertexCount;

	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, dataSize);

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	StagingBuffer.TransferDataToBuffer(indices->data(), m_Buffer, fence);	//indicesBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����
}

uint32_t VIndexBuffer::GetVertexCount()
{
	return m_VertexCount;
}

void VIndexBuffer::Cleanup()
{
	printf("�C���f�b�N�X�o�b�t�@��������܂���");
	VBufferBase::Cleanup();
}
