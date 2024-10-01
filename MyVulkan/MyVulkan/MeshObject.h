#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>          // ���_��}�e���A���p�̃x�N�g����s��̃��C�u����


// ���p�C�v���C���ɂ����̏���n�����߂̒�`��"PipelineInputUtility"�ɋL��
// ���_�����i�[���邽�߂̍\����
struct Vertex
{
	glm::vec3 position;		// ���_���W (x, y, z)
	glm::vec3 normal;		// �@���x�N�g��(�ʂ̌���)
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



//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�ɂ���