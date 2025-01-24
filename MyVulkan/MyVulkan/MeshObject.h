#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>          // ���_��}�e���A���p�̃x�N�g����s��̃��C�u����
#include <glm/fwd.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// ���p�C�v���C���ɂ����̏���n�����߂̒�`��"PipelineInputUtility"�ɋL��

struct SamplerInfo	// �e���[�h�̏ڍׂɂ��Ă͖��쐬
{
	uint32_t	magFilter = 0;	// �e�N�X�`�����g�傷��ۂ̃t�B���^�����O���@
	uint32_t	minFilter = 0;	// �e�N�X�`�����k������ۂ̃t�B���^�����O���@
	uint32_t	wrapS = 0;		// �e�N�X�`����S�����iU���j�̋��E����
	uint32_t	wrapT = 0;		// �e�N�X�`����T�����iV���j�̋��E����
};			
					


struct Texture
{
	uint8_t* data = 0;			// RAM��̉摜�f�[�^�̃|�C���^
	uint32_t width = 0;			// �摜�̕�
	uint32_t height = 0;		// �摜�̍���
	uint32_t channel = 0;		// 1��f�Ɋ܂܂��`�����l����	
};

// �}�e���A��(�F��e�N�X�`��)�̏����i�[���邽�߂̍\����
struct Material
{
	glm::vec4	baseColor		= glm::vec4(1.0f);	// ��{�F (r, g, b, a)
	float		metallic		= 0.0f;				// ������
	float		roughness		= 0.0f;				// �\�ʂ̑e��

	Texture		texture			= Texture();		// �e�N�X�`���摜�̃f�[�^
	// SamplerInfo	samplerInfo;
};


// ���_�����i�[���邽�߂̍\����
struct Vertex
{
	glm::vec3 position = glm::vec3(0);		// ���_���W (x, y, z)
	glm::vec3 normal   = glm::vec3(0);		// �@���x�N�g��(�ʂ̌���)
	glm::vec2 textureCoord = glm::vec2(0); // �e�N�X�`�����W (u, v)
};

// ���b�V��(���_�ƃC���f�b�N�X)�̏����i�[���邽�߂̍\����
struct Mesh
{
	std::vector<Vertex>		vertices;	// ���_�̃��X�g
	std::vector<uint32_t>	indices;	// ���_�̃C���f�b�N�X���X�g(���_�̕`�揇��)
};

//struct Animation
//{
//	std::vector<Bone>;
//	std::vector<Skin>;
//	std::vector<Keyframe>;
//};

using Transform = glm::mat4;

// ���O�̏����l(���O�����ݒ�̎��p)
static const char* DafaultObjectName = "Undefined";

struct MeshObject
{
	std::string name		= DafaultObjectName;
	Transform	transform	= Transform(1.0f);

	Mesh		mesh		= Mesh();		// �I�u�W�F�N�g�̌`�̏��
	Material	material	= Material();	// �I�u�W�F�N�g�̕\�ʂ̌����ڂ̏��
	//Animation animation;	// �I�u�W�F�N�g�̃A�j���[�V�������
};



//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�ɂ���