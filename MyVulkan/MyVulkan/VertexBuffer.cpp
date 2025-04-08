#include "VertexBuffer.h"



VVertexBuffer::VVertexBuffer(): 
	m_VertexCount(0),
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer,	// ���_�o�b�t�@�Ɏg��
		NULL,											// �K�{�v������
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,			// �f�o�C�X���[�J���D��Ń��������m��
		VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT)
{

}

VVertexBuffer::~VVertexBuffer()
{
	Cleanup();
}

void VVertexBuffer::SetData(VmaAllocator* allocator, std::vector<Vertex>* vertices)
{
	m_VertexCount = vertices->size();
	vk::DeviceSize dataSize = sizeof(Vertex) * m_VertexCount;


	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, dataSize);

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	VStagingBuffer StagingBuffer;
	StagingBuffer.Initialize(allocator, dataSize);					//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	StagingBuffer.TransferDataToBuffer(vertices->data(), m_Buffer);	//VertexBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����

}

size_t VVertexBuffer::GetVertexCount()
{
	return m_VertexCount;
}

void VVertexBuffer::Cleanup()
{
	printf("���_�o�b�t�@��������܂�");
	VBufferBase::Cleanup();
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

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(BindingDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = BindingDescriptions.data();
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	return vertexInputInfo;
}
