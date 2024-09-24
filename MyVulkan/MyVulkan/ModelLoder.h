#pragma once
#include <assimp/Importer.hpp>  // Assimp�̃C���|�[�^�[���g�p
#include <assimp/scene.h>       // Assimp�̃V�[���\���̂��g�p
#include <assimp/postprocess.h> // ���f�������̃I�v�V�������w��
#include <glm/glm.hpp>          // ���_��}�e���A���p�̃x�N�g����s��̃��C�u����
#include <vector>
#include <string>
#include <iostream>

// ���_�����i�[���邽�߂̍\����
struct Vertex
{
	glm::vec3 position;		// ���_���W (x, y, z)
	glm::vec3 normal;		// �@���x�N�g��(�ʂ̌���)
	//glm::vec3 color;		// ���_�F (r, g, b)
	glm::vec2 textureCoord; // �e�N�X�`�����W (u, v)

};

// ���b�V��(���_�ƃC���f�b�N�X)�̏����i�[���邽�߂̍\����
struct Mesh 
{
	std::vector<Vertex>		vertices;	// ���_�̃��X�g
	std::vector<uint32_t>	indices;	// ���_�̃C���f�b�N�X���X�g(���_�̕`�揇��)
};

// �}�e���A��(�F��e�N�X�`��)�̏����i�[���邽�߂̍\����
struct Material
{
	glm::vec4 baseColor;		// ��{�F (r, g, b, a)
	float metallic;				// ������
	float roughness;			// �\�ʂ̑e��

	std::string texturePath;	// �e�N�X�`���摜�̃t�@�C���p�X
};

//struct Animation
//{
//	std::vector<Bone>;
//		std::vector<Skin>;
//		std::vector<Keyframe>;
//};

struct MeshObject
{
	std::vector<Mesh>		meshes;		// �I�u�W�F�N�g�̌`�̏��
	std::vector<Material>	materials;	// �I�u�W�F�N�g�̕\�ʂ̌����ڂ̏��
	//Animation animation;	// �I�u�W�F�N�g�̃A�j���[�V�������
};



class ModelLoder
{
public:
	ModelLoder();
	~ModelLoder();

	bool Load(std::string filePath);

	MeshObject GetModel();

	//void draw(VkCommandBuffer commandBuffer);
	void cleanup();



private:
	MeshObject m_Model;

	//std::vector<Mesh> meshes;
	//std::vector<Material> materials;


	// �m�[�h����������v���C�x�[�g�֐�
	void ProcessNode(aiNode* node, const aiScene* scene);
	// ���b�V������������v���C�x�[�g�֐�
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	Material ProcessMaterial(aiMaterial* material);
};

//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�ɂ���