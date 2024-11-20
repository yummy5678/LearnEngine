#pragma once
#include <vulkan/vulkan.hpp>
#include "MeshObject.h"



//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
// �p�C�v���C���ɓn�����̒�`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

// Vertex �\���̂̏����`
//glm::vec3 position;		// ���_���W (x, y, z)
//glm::vec3 normal;			// �@���x�N�g��(�ʂ̌���)
//glm::vec2 textureCoord;	// �e�N�X�`�����W (u, v)

namespace VertexInputBinding
{
     // �o�C���f�B���O�̒�`   //��Ńe���v���[�g������
     constexpr std::array<vk::VertexInputBindingDescription,1> bindingDescriptions = 
     {
         vk::VertexInputBindingDescription
         {
            0,                              // binding  
            sizeof(Vertex),                 // stride   
            vk::VertexInputRate::eVertex    // inputRate
         }    
     };

    // ���͑����̒�`
    constexpr std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {
        // ���W
        vk::VertexInputAttributeDescription{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) },

        // �@��
       vk::VertexInputAttributeDescription{ 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) },

        // �e�N�X�`�����W
       vk::VertexInputAttributeDescription{ 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) }

    };

    // �p�C�v���C�����_���͏�Ԃ̍쐬
    static vk::PipelineVertexInputStateCreateInfo& GetVertexInputInfo()
    {
        vk::PipelineVertexInputStateCreateInfo resultInfo;
        resultInfo.setVertexBindingDescriptions(bindingDescriptions);
        resultInfo.setVertexAttributeDescriptions(attributeDescriptions);
        return resultInfo;          
    };
};



// ���_��񂩂�v���~�e�B�u(��)�̑g�ݗ��ĕ��̒�`
//constexpr vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = [] {//�O���t�B�N�X�p�C�v���C���̍쐬�ɕK�v
//    vk::PipelineInputAssemblyStateCreateInfo info;
//    info.topology = vk::PrimitiveTopology::eTriangleList;   // �g�|���W�[(�O�p�`���X�g)
//    info.primitiveRestartEnable = VK_FALSE;                 // �v���~�e�B�u�ĊJ�𖳌��ɂ���
//    return info;
//    }();



///////////////////////////////////
// �v�b�V���萔
///////////////////////////////////
// Model
//constexpr std::array<vk::PushConstantRange, 1> pushConstantModelRange = {
//    // �v�b�V���萔�ɍ쐬���Ƃ��͕K�v�Ȃ�
//    vk::PushConstantRange{
//    vk::ShaderStageFlagBits::eVertex,	// �n�������V�F�[�_�[�X�e�[�W
//    0,								    // �n�����f�[�^����ǂ̈ʒu�̃f�[�^�����邩
//    sizeof(Transform)					// �n�������f�[�^�̃T�C�Y
//    }
//};


















