#pragma once
#include "MeshObject.h"
#include "BufferGenerator.h"
#include "ImagesGenerator.h"
#include "ImageBuffer.h"

using Transform = glm::mat4;


class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	void		SetPMesh(MeshObject* pMesh);
	MeshObject	GetMesh();

	void		SetTransform(glm::mat4 transform);
	glm::mat4	GetTransform();


	vk::Buffer	GetVertexBuffer();


private:
	MeshObject		m_Mesh;			// ���b�V���̃|�C���^(���Ԃ̓��b�V���}�l�[�W���[���Ǘ�)
	Transform		m_Transform;	// �g�����X�t�H�[��(�I�u�W�F�N�g�̍��W�Ƃ��̏��)


	VVertexBuffer	m_VertexBuffer;	// ���_�o�b�t�@
	VImageBuffer	m_Texture;		// �e�N�X�`���[�o�b�t�@


};

