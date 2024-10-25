#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// glTF�̃t�@�C������`
// �g���@�\�̏��ɂ��Ă͔�Ή�

namespace glTF_Data
{
    struct Asset {
        std::string copyright;          // ���쌠���
        std::string generator;          // glTF�𐶐������c�[����
        std::string version;            // glTF�̃o�[�W����   ���K�{
        std::string minVersion;         // �T�|�[�g����Ă���ŏ��o�[�W����
    };

    struct Scene {
        std::string name;               // �V�[���̖��O 
        std::vector<int> nodes;         // �V�[���Ɋ܂܂��m�[�h�̃C���f�b�N�X���X�g
    };

    struct Node {
        std::string name;               // �m�[�h�̖��O
        std::vector<int> children;      // ���̃m�[�h�̎q�m�[�h�̃C���f�b�N�X���X�g
        glm::mat4 matrix;               // �m�[�h�̕ϊ��s��(4x4)
        glm::vec3 translation;          // �m�[�h�̈ʒu�x�N�g��(3D)
        glm::quat rotation;             // �m�[�h�̉�]��\���N�H�[�^�j�I��
        glm::vec3 scale;                // �m�[�h�̃X�P�[���x�N�g��(3D)
        int mesh;                       // ���̃m�[�h���Q�Ƃ��郁�b�V���̃C���f�b�N�X
        int camera;                     // ���̃m�[�h���Q�Ƃ���J�����̃C���f�b�N�X
    };

    struct Mesh {
        std::string name;               // ���b�V���̖��O
        std::vector<int> primitives;    // ���b�V���̃v���~�e�B�u�̃C���f�b�N�X���X�g
        std::vector<float> weights;     // �E�F�C�g�̃��X�g(�X�L���A�j���[�V�����p)
    };

    struct Primitive {
        int mode;                       // �v���~�e�B�u�̕`�惂�[�h(��: TRIANGLES)
        std::vector<int> attributes;    // ���_�����̃C���f�b�N�X���X�g
        std::vector<int> indices;       // ���_�C���f�b�N�X�̃o�b�t�@
        int material;                   // �g�p����}�e���A���̃C���f�b�N�X
        std::vector<int> modeTargets;   // ���[�t�^�[�Q�b�g�̃C���f�b�N�X(�I�v�V����)
    };

    struct Image {
        std::string name;               // �摜�̖��O
        std::string uri;                // �摜�f�[�^��URI
        std::string mimeType;           // �摜�f�[�^��MIME�^�C�v

    };
        int bufferView;                 // �摜�f�[�^���i�[����Ă���o�b�t�@�r���[�̃C���f�b�N�X
    struct Sampler {
        std::string name;               // �T���v���[�̖��O
        int magFilter;                  // �g��t�B���^�����O�̕��@
        int minFilter;                  // �k���t�B���^�����O�̕��@
        int wrapS;                      // S���̃��b�s���O���[�h
        int wrapT;                      // T���̃��b�s���O���[�h
    };

    struct Texture {
        std::string name;               // �e�N�X�`���̖��O
        int sampler;                    // �g�p����T���v���[�̃C���f�b�N�X
        int source;                     // �e�N�X�`���摜�̃C���f�b�N�X
    };

    // �}�e���A��(���f���\�ʂ̍ގ�)
    struct Material {
        std::string name;               // �}�e���A���̖��O
        MetallicRoughness metallicRoughness; // �����I����ёe���̐ݒ�
        int normalTexture;              // �@���}�b�v�̃C���f�b�N�X
        int occlusionTexture;           // �A�e�}�b�v�̃C���f�b�N�X
        int emissiveTexture;            // �������}�b�v�̃C���f�b�N�X
        glm::vec3 emissiveFactor;       // �������̋��x(RGB)
        std::string alphaMode;          // �A���t�@(�����x)���[�h
        float alphaCutoff;              // �A���t�@�J�b�g�I�t�l
        bool doubleSided;               // ���ʕ`��̃t���O
    };

    struct MetallicRoughness {
        int baseColorTexture;           // �x�[�X�J���[�̃e�N�X�`��
        glm::vec4 baseColorFactor;      // �x�[�X�J���[�̈��q(RGBA)
        int metallicRoughnessTexture;   // �����I����ёe���̃e�N�X�`��
        float metallicFactor;           // �����I���q
        float roughnessFactor;          // �e�����q
    };

    struct TextureInfo {
        int index;                      // �e�N�X�`���̃C���f�b�N�X
        int texCoord;                   // �e�N�X�`�����W�̃Z�b�g
        float scale;                    // �e�N�X�`���̃X�P�[��
        float strength;                 // �e�N�X�`���̋��x
    };

    struct SceneCamera {
        std::string name;               // �J�����̖��O
        std::string type;               // �J�����̃^�C�v(orthographic�܂���perspective)
        OrthoGraphic orthographic;      // ���s���e�Ɋւ�����
        Perspective perspective;        // �������e�Ɋւ�����
    };

    struct OrthoGraphic {
        float xMag;                    // X�����̃X�P�[��
        float yMag;                    // Y�����̃X�P�[��
        float zFar;                    // �J�����̉��߃N���b�s���O��
        float zNear;                   // �J�����̋ߐڃN���b�s���O��
    };

    struct Perspective {
        float aspectRatio;             // �A�X�y�N�g��
        float yFov;                    // ��������p
        float zFar;                    // �J�����̉��߃N���b�s���O��
        float zNear;                   // �J�����̋ߐڃN���b�s���O��
    };

    struct Skin {
        std::string name;              // �X�L���̖��O
        std::vector<float> inverseBindMatrices; // �t�o�C���h�s��̃��X�g
        std::vector<int> joints;       // �{�[���̃C���f�b�N�X���X�g
        int skeleton;                  // �X�P���g���̃m�[�h�̃C���f�b�N�X
    };

    struct Animation {
        std::string name;              // �A�j���[�V�����̖��O
        std::vector<int> channels;     // �A�j���[�V�����`�����l���̃��X�g
        std::vector<int> samplers;     // �T���v���[�̃��X�g
    };


    //�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
    // �ȉ��A�t�@�C���ǂݍ��ݎ��Ɏg�p����
    // �f�[�^�̔z�u��m��ׂ̏��
    // (�ǂݍ��݃��C�u�������g���ׂ̏��Ȃ̂ō쐬�s�v)
    //�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
    struct Buffer {
        std::string name;           // �o�b�t�@�̖��O
        size_t byteLength;          // �o�b�t�@�̃o�C�g��
        std::string uri;            // �o�b�t�@�f�[�^��URI
    };

    struct BufferView {
        std::string name;           // �o�b�t�@�r���[�̖��O
        int buffer;                 // �Ή�����o�b�t�@�̃C���f�b�N�X
        size_t byteOffset;          // �o�b�t�@���ł̃I�t�Z�b�g
        size_t byteLength;          // �r���[�̃o�C�g��
        int target;                 // �o�b�t�@�r���[�̃^�[�Q�b�g(��: ARRAY_BUFFER)
    };

    struct Accessor {
        std::string name;           // �A�N�Z�T�̖��O
        int bufferView;             // �f�[�^���i�[����Ă���o�b�t�@�r���[�̃C���f�b�N�X
        size_t byteOffset;          // �o�b�t�@���ł̃I�t�Z�b�g
        int componentType;          // �R���|�[�l���g�̃f�[�^�^
        bool normalized;            // ���K���t���O
        int count;                  // �v�f�̐�
        std::vector<float> max;     // �ő�l�̃��X�g
        std::vector<float> min;     // �ŏ��l�̃��X�g
        Sparse sparse;              // �X�p�[�X�f�[�^�̐ݒ�
    };

    struct Sparse {
        int count;                  // �X�p�[�X�f�[�^�̗v�f��
        int indices;                // �C���f�b�N�X�f�[�^�̃A�N�Z�T
        int values;                 // �l�f�[�^�̃A�N�Z�T
    };
}
