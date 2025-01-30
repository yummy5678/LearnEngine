#pragma once
#include "MeshBuffer.h"
#include "MaterialBuffer.h"
#include "MeshObject.h" 
#include "Observer.h"


class VMeshObject
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject);

	//// オブザーバーの管理関数
	//void SetMMaterialUpdateObserver(Observer function);
	//void DeleteMaterialUpdateObserver(Observer function);

	VMesh*			GetPMesh();
	VMaterial*		GetPMaterial();
	
private:
	std::string		m_Name;
	Transform		m_Transform;	// トランスフォーム
	VMesh			m_Mesh;
	VMaterial		m_Material;	

	//Subject			m_MeshUpdateSubject;
	/*Subject			m_BufferUpdateSubject;*/



	void SetMesh(VmaAllocator* allocator, MeshObject* meshObject);
	void SetMaterial(VmaAllocator* allocator, MeshObject* meshObject);

	
};
