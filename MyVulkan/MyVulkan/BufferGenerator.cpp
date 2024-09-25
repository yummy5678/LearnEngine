#include "BufferGenerator.h"

BufferGenerator::BufferGenerator()
{
}

BufferGenerator::~BufferGenerator()
{
}

void BufferGenerator::CreateMeshObject(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, MeshObject object)
{
	m_bCreated = true;

	m_LogicalDevice = logicalDevice;
	m_PhysicalDevice = physicalDevice;

	m_BufferCount = object.meshes.size();

	// �������[�t���O(CPU���������� ���� ��ѐ��̂��郁����)
	vk::MemoryPropertyFlags memoryType = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
	for (int i = 0; i < m_BufferCount; i++)
	{
		// ���_�o�b�t�@�̍쐬
		auto bufferInfo = CreateVertexBufferInfo(object.meshes[i].vertices);
		try{ m_VertexBuffer[i] = logicalDevice.createBuffer(bufferInfo); }
		catch(const std::exception&) { std::runtime_error("���_�o�b�t�@�̍쐬�Ɏ��s���܂����I"); }
		// ���_�o�b�t�@�������̊��蓖��
		auto allocateInfo = AllocateBufferMemoryInfo(logicalDevice, physicalDevice, m_VertexBuffer[i], memoryType);
		m_VertexBufferMemory[i] = logicalDevice.allocateMemory(allocateInfo);

		// ���������o�b�t�@�Ƀo�C���h
		logicalDevice.bindBufferMemory(m_VertexBuffer[i], m_VertexBufferMemory[i], 0);		
	}

}

void BufferGenerator::CopyDataToBuffer(void* data, std::vector<Vertex> vertices)
{
	for (int i = 0; i < m_BufferCount; i++)
	{
		// �o�b�t�@�̃��������}�b�v�iCPU���ŃA�N�Z�X�\�ɂ���j
		void* dstData;
		m_LogicalDevice.mapMemory(m_VertexBufferMemory[0], (vk::DeviceSize)0, (vk::DeviceSize)vertices.size(), vk::MemoryMapFlags(), &dstData);

		// �f�[�^���R�s�[
		std::memcpy(dstData, data, (size_t)vertices.size());

		// �������̃}�b�v������
		m_LogicalDevice.unmapMemory(m_VertexBufferMemory[i]);
	}


}

void BufferGenerator::Destroy()
{
}

vk::BufferCreateInfo BufferGenerator::CreateVertexBufferInfo(std::vector<Vertex> vertices)
{
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;


	vk::BufferCreateInfo bufferCreateInfo;
	bufferCreateInfo.pNext;
	bufferCreateInfo.flags;
	bufferCreateInfo.size = sizeof(Vertex) * vertices.size();
	bufferCreateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;	// ���_�o�b�t�@�Ƃ��Ďg�p
	bufferCreateInfo.sharingMode = vk::SharingMode::eExclusive;			// �o�b�t�@�̎g�p��1�̃L���[�Ɍ���
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;
	return bufferCreateInfo;
}

vk::MemoryAllocateInfo BufferGenerator::AllocateBufferMemoryInfo(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags)
{
	// �C���[�W�̃������v�����擾
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);

	// �T���Ă��郁�����v���p�e�B�̃t���O
	vk::MemoryPropertyFlags properties = propertyFlags;

	// �������̊��蓖�ď���ݒ�
	vk::MemoryAllocateInfo allocateInfo;
	allocateInfo.pNext;
	allocateInfo.allocationSize = memoryRequirements.size;  // �摜�̃������T�C�Y
	allocateInfo.memoryTypeIndex = FindMemoryType(logicalDevice, physicalDevice, buffer, properties);   // �������^�C�v

	return allocateInfo;
}

uint32_t BufferGenerator::FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType)
{
	// �o�b�t�@�̃������v�����擾
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);
	auto typeFilter = memoryRequirements.memoryTypeBits;

	// �����f�o�C�X(GPU)���烁�����v���p�e�B���擾�B
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		// typeFilter�́A�r�b�g�}�X�N�Ń������^�C�v���t�B���^�����O���邽�߂̂��́B
		// �����ł́AtypeFilter�̃r�b�g��i�Ԗڂ̃������^�C�v�ɑΉ����邩�ǂ������m�F���Ă��܂��B
		// (1 << i)�́Ai�r�b�g�ڂ̃r�b�g��1�ɂ����r�b�g�}�X�N�ł��B
		// typeFilter & (1 << i) �́Ai�r�b�g�ڂ̃r�b�g�������Ă��邩���m�F���܂��B
		if ((typeFilter & (1 << i)) &&
			(memoryProperties.memoryTypes[i].propertyFlags & findType) == findType)
		{
			return i;
		}
	}

	// �G���[���b�Z�[�W
	throw std::runtime_error("�K�؂ȃ������^�C�v���������܂���ł���!");
}
