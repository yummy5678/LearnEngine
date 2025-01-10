#pragma once
#include "MeshBuffer.h"
#include "MaterialBuffer.h"
#include "MeshObject.h" 


class VMeshObject
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject);

	VMesh*			GetPMesh();
	VMaterial*		GetPMaterial();
	
private:
	std::string		m_Name;
	Transform		m_Transform;	// �g�����X�t�H�[��
	VMesh			m_Mesh;
	VMaterial		m_Material;	
	

	void SetMesh(VmaAllocator* allocator, MeshObject* meshObject);
	void SetMaterial(VmaAllocator* allocator, MeshObject* meshObject);

	
};
