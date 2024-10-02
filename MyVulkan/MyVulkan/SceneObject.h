#pragma once
#include "MeshObject.h"
#include "BufferGenerator.h"
#include "ImagesGenerator.h"

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	void		SetMesh(MeshObject* pMesh);
	MeshObject* GetPMesh();

	void		SetTransform(glm::mat4 transform);
	glm::mat4	GetTransform();


	vk::Buffer	GetMeshBuffer();


private:
	MeshObject*		m_pMesh;			// ���b�V���̃|�C���^(���Ԃ̓��b�V���}�l�[�W���[���Ǘ�)
	glm::mat4		m_Transform;		// �g�����X�t�H�[��(�I�u�W�F�N�g�̍��W�Ƃ��̏��)

	BufferGenerator m_VertexBuffer;		// ���_�o�b�t�@
	//ImagesGenerator m_TextureBuffer;	// �e�N�X�`���[�o�b�t�@
};

