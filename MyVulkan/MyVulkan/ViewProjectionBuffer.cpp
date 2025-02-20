#include "ViewProjectionBuffer.h"

VViewProjectionBuffer::VViewProjectionBuffer() : 
	VBufferBase(vk::BufferUsageFlagBits::eUniformBuffer,	// ���j�t�H�[���o�b�t�@�Ɏg��
		NULL,												// �K�{�v������
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,				// �f�o�C�X���[�J���D��Ń��������m��
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT)
{
}

VViewProjectionBuffer::~VViewProjectionBuffer()
{
}

void VViewProjectionBuffer::Initialize(VmaAllocator* allocator)
{
	if (m_Buffer != VK_NULL_HANDLE)
	{
		// �G���[���b�Z�[�W
		return;
	}

	m_DataSize = sizeof(ViewProjection);

	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, m_DataSize);
}

void VViewProjectionBuffer::SetData(VmaAllocator* allocator, ViewProjection& projection)
{
	if (m_Buffer == VK_NULL_HANDLE || m_DataSize == 0)
	{
		// �G���[���b�Z�[�W
		return;
	}

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, m_DataSize);				//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	StagingBuffer.TransferDataToBuffer(&projection, m_Buffer);	//indicesBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����

	// CPU����GPU�֏��𑗂�̂ɓK�����������̈���쐬������
	//VmaAllocationCreateInfo stagingAllocateInfo;
	//stagingAllocateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;	// �z�X�g����̏������݂�����
	//stagingAllocateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;						// CPU����A�N�Z�X�\

}

//void VViewProjectionBuffer::UpdateDescriptorSets(VmaAllocator* allocator, ViewProjection& projection)
//{
//	MapData(&projection, sizeof(ViewProjection));
//}

