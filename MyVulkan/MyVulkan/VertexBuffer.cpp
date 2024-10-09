#include "VertexBuffer.h"


VertexBuffer::VertexBuffer():
	VBufferBase(vk::BufferUsageFlagBits::eVertexBuffer)
{

}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::SetData(VmaAllocator allocator, std::vector<Vertex> vertices)
{
	vk::DeviceSize dataSize = sizeof(Vertex) * vertices.size();

	// ���_�p�̃o�b�t�@�y�у������̍쐬
	CreateBuffer(allocator, dataSize);

	// �X�e�[�W���O�o�b�t�@�𓥂�Ńf�[�^�����Ă��炤
	m_Stage.Initialize(allocator, sizeof(Vertex) * vertices.size());		//��x�X�e�[�W���O�o�b�t�@�Ƀf�[�^�����Ă���
	m_Stage.TransferDataToBuffer(vertices.data(), dataSize, m_Buffer);	//VertexBuffer(VRAM�ɍ��ꂽ�o�b�t�@)�ɃR�s�[����

}

const vk::VertexInputBindingDescription VertexBuffer::GetBindingDescription()
{
	vk::VertexInputBindingDescription binding;
	binding.binding = 0;
	binding.stride = sizeof(Vertex);
	binding.inputRate = vk::VertexInputRate::eVertex;
	return binding;
}

const vk::PipelineVertexInputStateCreateInfo VertexBuffer::InputStateInfo()
{
	vk::PipelineVertexInputStateCreateInfo info;
	info.vertexBindingDescriptionCount = 1;
	info.pVertexBindingDescriptions = &GetBindingDescription();
	info.vertexAttributeDescriptionCount = GetAttributeDescriptions().size();
	info.pVertexAttributeDescriptions = GetAttributeDescriptions().data();
	return info;
}

const std::array<vk::VertexInputAttributeDescription, 3> VertexBuffer::GetAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 3> attribute;

	// ���W
	attribute[0] = { 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) };

	// �@��
	attribute[1] = { 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) };

	// �e�N�X�`�����W
	attribute[2] = { 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) };

	return attribute;
}
