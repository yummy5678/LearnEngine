#pragma once
#include "NonCopyable.h"
#include "MeshBuffer.h"
#include "MaterialBuffer.h"
#include "MeshObject.h" 
//#include "Observer.h"


class VMeshObject //: public NonCopyable
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject);

	void Cleanup();

	//// オブザーバーの管理関数
	//void SetMMaterialUpdateObserver(Observer function);
	//void DeleteMaterialUpdateObserver(Observer function);

	std::shared_ptr<VMesh>		GetSPMesh();
	std::shared_ptr<VMaterial>	GetSPMaterial();

	vk::DescriptorSet GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> sDescriptorSetLayout);
	
private:
	std::string					m_Name;
	vk::Device					m_LogicalDevice;
	vk::Fence					m_Fence;
	Transform					m_Transform;	// トランスフォーム
	std::shared_ptr<VMesh>		m_Mesh;
	std::shared_ptr<VMaterial>	m_Material;

	std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor*>	m_DescriptorSets;

	//Subject			m_MeshUpdateSubject;
	//Subject			m_BufferUpdateSubject;



	void SetMesh(VmaAllocator* allocator, Mesh* mesh, vk::Fence fence);
	void SetMaterial(VmaAllocator* allocator, Material* material, vk::Fence fence);

	// ディスクリプタセットのリスト作成用関数達
	void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを登録
	void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認
	void CleanupDescriptorSets();// 無効になったオーナーを削除
	void UpdateDescriptorSets(vk::ImageView imageView, vk::Sampler sampler);
};
