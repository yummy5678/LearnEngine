#pragma once
#include "MeshObject.h"
#include "MaterialBuffer.h"
#include "MeshBuffer.h"
#include "MeshObjectBuffer.h"
#include <memory> // std::shared_ptr���g�p

class SceneObject
{
public:
	SceneObject() = default;
	~SceneObject();

	void					SetMesh(VmaAllocator* allocator, VMeshObject* mesh);
	std::shared_ptr<std::vector<VMesh>>		GetMeshes();
	std::shared_ptr<std::vector<VMaterial>>	GetMaterials();

	void		SetTransform(glm::mat4 transform);
	Transform* GetPTransform();

private:
	Transform				m_Transform;	// �g�����X�t�H�[��(�I�u�W�F�N�g�̍��W�Ƃ��̏��)
	std::shared_ptr<std::vector<VMaterial>>	m_Material;		// �e�N�X�`�����̃o�b�t�@�������Ă���
	std::shared_ptr<std::vector<VMesh>>		m_Mesh;			// ���_��񓙂̃o�b�t�@�������Ă���
};
