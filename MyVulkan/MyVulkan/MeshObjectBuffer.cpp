#include "MeshObjectBuffer.h"

VMeshObject::VMeshObject() :
    m_Name(""),
	m_LogicalDevice(VK_NULL_HANDLE),
    m_Transform(0),
    m_Mesh(),
    m_Material(),
	m_DescriptorSets()/*,
    m_MeshUpdateSubject(),
    m_MaterialUpdateSubject()*/
{
    m_Mesh = std::make_shared<VMesh>();
    m_Material = std::make_shared<VMaterial>();

}

VMeshObject::~VMeshObject()
{
    // m_Mesh.Cleanup();

    // m_Material.Cleanup();

}

void VMeshObject::SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject)
{
	// アロケーターに登録されている論理デバイスを取得
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);
	m_LogicalDevice = allocatorInfo.device;


    SetMesh(allocator, &meshObject->mesh);
    SetMaterial(allocator, &meshObject->material);

	UpdateDescriptorSets(m_Material.get()->GetTextureImageView(), m_Material.get()->GetSampler());
}

//void VMeshObject::SetMeshUpdateObserver(Observer function)
//{
//    m_MeshUpdateSubject.addObserver(function);
//}
//
//void VMeshObject::DeleteMeshUpdateObserver(Observer function)
//{
//    m_MeshUpdateSubject.removeObserver(function);
//}
//
//void VMeshObject::SetMMaterialUpdateObserver(Observer function)
//{
//    m_MaterialUpdateSubject.addObserver(function);
//}
//
//void VMeshObject::DeleteMaterialUpdateObserver(Observer function)
//{
//    m_MaterialUpdateSubject.removeObserver(function);
//}

std::shared_ptr<VMesh> VMeshObject::GetSPMesh()
{
    return m_Mesh;
}

std::shared_ptr<VMaterial> VMeshObject::GetSPMaterial()
{
    return  m_Material;
}

vk::DescriptorSet VMeshObject::GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> sDescriptorSetLayout)
{
	// 既に登録済みなら何もしない
	if (m_DescriptorSets.find(sDescriptorSetLayout) == m_DescriptorSets.end())
		SetDescriptorSet(sDescriptorSetLayout);

	return m_DescriptorSets[sDescriptorSetLayout]->GetDescriptorSet();
}



void VMeshObject::SetMesh(VmaAllocator* allocator, Mesh* mesh)
{
    m_Mesh->SetMesh(allocator, mesh);

    // メッシュの内容を更新したら登録している各オブザーバーに通知
    //m_MeshUpdateSubject.notifyObservers();
}

void VMeshObject::SetMaterial(VmaAllocator* allocator, Material* material)
{
    m_Material->SetMaterial(allocator, material);

    // マテリアルの内容を更新したら登録している各オブザーバーに通知
    //m_MaterialUpdateSubject.notifyObservers();

}

void VMeshObject::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
	if (*layout.get() == VK_NULL_HANDLE)
	{
		std::cerr << "SetDescriptorSet: layout is invalid." << std::endl;
		return;
	}

	// 必要に応じた VDescriptorBase 派生クラスのインスタンスを生成
	VSingleTextureDescriptor* descriptor = new VSingleTextureDescriptor;
	descriptor->Initialize(m_LogicalDevice, *layout.get());
	descriptor->Update(m_Material.get()->GetTextureImageView(), m_Material.get()->GetSampler());
	m_DescriptorSets.insert({ layout, descriptor });
}

void VMeshObject::DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
	m_DescriptorSets.erase(layout);
}

void VMeshObject::CleanupDescriptorSets()
{
	for (auto& pair : m_DescriptorSets)
	{
		// shared_ptrの中身が初期化されていたら削除
		if (*pair.first.get() == VK_NULL_HANDLE)
		{
			free(pair.second);
			m_DescriptorSets.erase(pair.first);
		}
	}
}

void VMeshObject::UpdateDescriptorSets(vk::ImageView imageView, vk::Sampler sampler)
{
	// 登録されているすべてのディスクリプタセットを更新
	for (auto& pair : m_DescriptorSets)
	{
		pair.second->Update(imageView, sampler);
	}
}



