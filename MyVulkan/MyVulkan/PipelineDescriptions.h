#pragma once
#include <vulkan/vulkan.hpp>
#include "MeshObject.h"
#include "Mesh.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
// �p�C�v���C���ɓn�����̒�`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

// Vertex �\���̂̏����`
//glm::vec3 position;		// ���_���W (x, y, z)
//glm::vec3 normal;			// �@���x�N�g��(�ʂ̌���)
//glm::vec2 textureCoord;	// �e�N�X�`�����W (u, v)


 // �o�C���f�B���O�̒�`   //��Ńe���v���[�g������
 constexpr std::array<vk::VertexInputBindingDescription,1> bindingDescription = [] {
     std::array<vk::VertexInputBindingDescription, 1> binding;
     binding[0].binding = 0;
     binding[0].stride = sizeof(Vertex);
     binding[0].inputRate = vk::VertexInputRate::eVertex;
     return binding;
 }();

// ���͑����̒�`
constexpr std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = [] {
    std::vector<vk::VertexInputAttributeDescription> attribute(3);

    // ���W
    attribute[0] = { 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) };

    // �@��
    attribute[1] = { 0, 1, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) };

    // �e�N�X�`�����W
    attribute[2] = { 0, 2, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoord) };

    return attribute;
}();

// �p�C�v���C�����_���͏�Ԃ̍쐬
constexpr vk::PipelineVertexInputStateCreateInfo vertexInputInfo = [] {
    vk::PipelineVertexInputStateCreateInfo info;
    info.vertexBindingDescriptionCount = bindingDescription.size();
    info.pVertexBindingDescriptions = bindingDescription.data();
    info.vertexAttributeDescriptionCount = attributeDescriptions.size();
    info.pVertexAttributeDescriptions = attributeDescriptions.data();
    return info;
}();




// ���_��񂩂�v���~�e�B�u(��)�̑g�ݗ��ĕ��̒�`
constexpr vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = [] {
    vk::PipelineInputAssemblyStateCreateInfo info;
    info.topology = vk::PrimitiveTopology::eTriangleList;   // �g�|���W�[(�O�p�`���X�g)
    info.primitiveRestartEnable = VK_FALSE;                 // �v���~�e�B�u�ĊJ�𖳌��ɂ���
    return info;
    }();









///////////////////////////////////
// �v�b�V���萔
///////////////////////////////////
// Model
constexpr std::vector<vk::PushConstantRange> pushConstantModelRange = [] {
    // �v�b�V���萔�ɍ쐬���Ƃ��͕K�v�Ȃ�
    vk::PushConstantRange	pushConstantRange;
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex;	// �n�������V�F�[�_�[�X�e�[�W
    pushConstantRange.offset = 0;								        // �n�����f�[�^����ǂ̈ʒu�̃f�[�^�����邩
    pushConstantRange.size = sizeof(Model);						        // �n�������f�[�^�̃T�C�Y

    return std::vector<vk::PushConstantRange> { pushConstantRange };
}();


























