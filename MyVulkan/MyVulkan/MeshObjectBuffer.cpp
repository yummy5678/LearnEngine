#include "MeshObjectBuffer.h"

VMeshObject::VMeshObject()
    : m_Mesh(),
      m_Material()
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
}

void VMeshObject::SetMaterial(VmaAllocator* allocator, MeshObject* meshObject)
{
    m_Material.SetMaterial(allocator, meshObject->material);

}


