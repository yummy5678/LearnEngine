#include "VMeshManager.h"

VMeshManager::~VMeshManager()
{
}

bool VMeshManager::Load(VmaAllocator* allocator, MeshObject* pMeshObject)
{
    VMeshObject object;
    object.SetMeshObject(allocator, *pMeshObject);
    m_ObjectList[pMeshObject] = object;

    return true;
}

VMeshObject* VMeshManager::GetObject(MeshObject* pMeshObject)
{
    return &m_ObjectList[pMeshObject];
}
