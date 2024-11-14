#pragma once
#include "MeshObject.h"
#include "MaterialBuffer.h"
#include "MeshBuffer.h"




class SceneObject
{
public:
	SceneObject() = default;
	~SceneObject();

	void					SetMesh(VmaAllocator allocator, MeshObject& mesh);
	std::vector<VMesh>		GetMeshes();
	std::vector<VMaterial>	GetMaterials();

	void		SetTransform(glm::mat4 transform);
	Transform	GetTransform();



private:
	Transform				m_Transform;	// �g�����X�t�H�[��(�I�u�W�F�N�g�̍��W�Ƃ��̏��)


	std::vector<VMaterial>	m_Material;		// �e�N�X�`�����̃o�b�t�@�������Ă���
	std::vector<VMesh>		m_Mesh;			// ���_��񓙂̃o�b�t�@�������Ă���




};

