#include "VertexBuffer.h"
#include "PipelineDescriptions.h"


VVertexBuffer::VVertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer,	// ���_�o�b�t�@�Ɏg��
		VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE)			// �f�o�C�X���[�J���D��Ń��������m��
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
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	StagingBuffer.TransferDataToBuffer(vertices.data(), m_Buffer);	//VertexBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����

}

uint32_t VVertexBuffer::GetSize()
{
	return m_Size;
}

vk::PipelineVertexInputStateCreateInfo VVertexBuffer::GetInputStateInfo()
{
	std::array<vk::VertexInputBindingDescription,1> BindingDescriptions =
	{
		vk::VertexInputBindingDescription
		{
		   0,                              // binding  
		   sizeof(Vertex),                 // stride   
		   vk::VertexInputRate::eVertex    // inputRate
		}
	};

	std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = 
	{
		// ���W
		vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },
		// �@��
		vk::VertexInputAttributeDescription{ 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) },
	   // �e�N�X�`�����W
		vk::VertexInputAttributeDescription{ 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) }
	};

	return vertexInputInfo;
}
