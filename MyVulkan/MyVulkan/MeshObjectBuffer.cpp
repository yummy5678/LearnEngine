#include "MeshObjectBuffer.h"

VMeshObject::VMeshObject()
    : m_Mesh(),
      m_Texture()
{
}

VMeshObject::~VMeshObject()
{
    for (auto& mesh : m_Mesh)
    {
        mesh.Cleanup();
    }

    for (auto& texture : m_Texture)
    {
        texture.Cleanup();
    }
}

void VMeshObject::SetMeshObject(VmaAllocator* allocator, MeshObject& meshObject)
{
    SetMesh(allocator, meshObject);
    SetMaterial(allocator, meshObject);
}

std::vector<VMesh>& VMeshObject::GetMeshes()
{
    return m_Mesh;
}

std::vector<VTextureBuffer>& VMeshObject::GetTexture()
{
    return m_Texture;
}

void VMeshObject::SetMesh(VmaAllocator* allocator, MeshObject& meshObject)
{
    m_Mesh.resize(meshObject.meshes.size());
    for (size_t i = 0; i < m_Mesh.size(); i++)
    {
        m_Mesh[i].SetMesh(allocator, meshObject.meshes[i]);
    }
}

void VMeshObject::SetMaterial(VmaAllocator* allocator, MeshObject& meshObject)
{
    m_Texture.resize(meshObject.materials.size());
    for (size_t i = 0; i < m_Texture.size(); i++)
    {
        m_Texture[i].SetImage(allocator, meshObject.materials[i].texture);
    }

}

