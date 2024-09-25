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

	// メモリーフラグ(CPUからも見れる かつ 一貫性のあるメモリ)
	vk::MemoryPropertyFlags memoryType = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
	for (int i = 0; i < m_BufferCount; i++)
	{
		// 頂点バッファの作成
		auto bufferInfo = CreateVertexBufferInfo(object.meshes[i].vertices);
		try{ m_VertexBuffer[i] = logicalDevice.createBuffer(bufferInfo); }
		catch(const std::exception&) { std::runtime_error("頂点バッファの作成に失敗しました！"); }
		// 頂点バッファメモリの割り当て
		auto allocateInfo = AllocateBufferMemoryInfo(logicalDevice, physicalDevice, m_VertexBuffer[i], memoryType);
		m_VertexBufferMemory[i] = logicalDevice.allocateMemory(allocateInfo);

		// メモリをバッファにバインド
		logicalDevice.bindBufferMemory(m_VertexBuffer[i], m_VertexBufferMemory[i], 0);		
	}

}

void BufferGenerator::CopyDataToBuffer(void* data, std::vector<Vertex> vertices)
{
	for (int i = 0; i < m_BufferCount; i++)
	{
		// バッファのメモリをマップ（CPU側でアクセス可能にする）
		void* dstData;
		m_LogicalDevice.mapMemory(m_VertexBufferMemory[0], (vk::DeviceSize)0, (vk::DeviceSize)vertices.size(), vk::MemoryMapFlags(), &dstData);

		// データをコピー
		std::memcpy(dstData, data, (size_t)vertices.size());

		// メモリのマップを解除
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
	bufferCreateInfo.usage = vk::BufferUsageFlagBits::eVertexBuffer;	// 頂点バッファとして使用
	bufferCreateInfo.sharingMode = vk::SharingMode::eExclusive;			// バッファの使用を1つのキューに限定
	bufferCreateInfo.queueFamilyIndexCount;
	bufferCreateInfo.pQueueFamilyIndices;
	return bufferCreateInfo;
}

vk::MemoryAllocateInfo BufferGenerator::AllocateBufferMemoryInfo(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags propertyFlags)
{
	// イメージのメモリ要件を取得
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);

	// 探しているメモリプロパティのフラグ
	vk::MemoryPropertyFlags properties = propertyFlags;

	// メモリの割り当て情報を設定
	vk::MemoryAllocateInfo allocateInfo;
	allocateInfo.pNext;
	allocateInfo.allocationSize = memoryRequirements.size;  // 画像のメモリサイズ
	allocateInfo.memoryTypeIndex = FindMemoryType(logicalDevice, physicalDevice, buffer, properties);   // メモリタイプ

	return allocateInfo;
}

uint32_t BufferGenerator::FindMemoryType(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::Buffer buffer, vk::MemoryPropertyFlags findType)
{
	// バッファのメモリ要件を取得
	vk::MemoryRequirements memoryRequirements = logicalDevice.getBufferMemoryRequirements(buffer);
	auto typeFilter = memoryRequirements.memoryTypeBits;

	// 物理デバイス(GPU)からメモリプロパティを取得。
	vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		// typeFilterは、ビットマスクでメモリタイプをフィルタリングするためのもの。
		// ここでは、typeFilterのビットがi番目のメモリタイプに対応するかどうかを確認しています。
		// (1 << i)は、iビット目のビットを1にしたビットマスクです。
		// typeFilter & (1 << i) は、iビット目のビットが立っているかを確認します。
		if ((typeFilter & (1 << i)) &&
			(memoryProperties.memoryTypes[i].propertyFlags & findType) == findType)
		{
			return i;
		}
	}

	// エラーメッセージ
	throw std::runtime_error("適切なメモリタイプを見つけられませんでした!");
}
