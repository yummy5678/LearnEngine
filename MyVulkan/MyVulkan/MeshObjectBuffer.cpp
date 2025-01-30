#include "MeshObjectBuffer.h"

VMeshObject::VMeshObject() :
    m_Name(""),
    m_Transform(0),
    m_Mesh(),
    m_Material(),
    m_MeshUpdateSubject(),
    m_MaterialUpdateSubject()
{
}

VMeshObject::~VMeshObject()
{
    // m_Mesh.Cleanup();

    // m_Material.Cleanup();

}

void VMeshObject::SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject)
{
    SetMesh(allocator, meshObject);
    SetMaterial(allocator, meshObject);
}

void VMeshObject::SetMeshUpdateObserver(Observer function)
{
    m_MeshUpdateSubject.addObserver(function);
}

void VMeshObject::DeleteMeshUpdateObserver(Observer function)
{
    m_MeshUpdateSubject.removeObserver(function);
}

void VMeshObject::SetMMaterialUpdateObserver(Observer function)
{
    m_MaterialUpdateSubject.addObserver(function);
}

void VMeshObject::DeleteMaterialUpdateObserver(Observer function)
{
    m_MaterialUpdateSubject.removeObserver(function);
}

VMesh* VMeshObject::GetPMesh()
{
    return &m_Mesh;
}

VMaterial* VMeshObject::GetPMaterial()
{
    return &m_Material;
}



void VMeshObject::SetMesh(VmaAllocator* allocator, MeshObject* meshObject)
{
    m_Mesh.SetMesh(allocator, &meshObject->mesh);

    // メッシュの内容を更新したら登録している各オブザーバーに通知
    m_MeshUpdateSubject.notifyObservers();
}

void VMeshObject::SetMaterial(VmaAllocator* allocator, MeshObject* meshObject)
{
    m_Material.SetMaterial(allocator, meshObject->material);

    // マテリアルの内容を更新したら登録している各オブザーバーに通知
    m_MaterialUpdateSubject.notifyObservers();

}


