#pragma once
#include "MeshObject.h"
#include "MaterialBuffer.h"
#include "MeshBuffer.h"
#include "MeshObjectBuffer.h"
#include <memory> // std::shared_ptr���g�p


// �I�u�W�F�N�g�̕`�掞�ɕK�v�ȏ����܂Ƃ߂��N���X
class RenderObject
{
public:
	RenderObject();
	~RenderObject();

	void SetMesh(VmaAllocator* allocator, VMeshObject* mesh);
	std::vector<VMesh>&				GetMeshes();
	std::vector<VTextureBuffer>&	GetMaterials();

	void		SetTransform(glm::mat4 transform);
	Transform*  GetPTransform();

	std::vector<vk::DescriptorSet>  GetDescriptorSets();

private:
	Transform						m_Transform;	// �g�����X�t�H�[��(�I�u�W�F�N�g�̍��W�Ƃ��̏��)
	VMeshObject*	m_Mesh; 



};
