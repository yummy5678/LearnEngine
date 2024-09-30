#pragma once
#include <vulkan/vulkan.hpp>
#include "ModelLoder.h"

//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E
// �p�C�v���C���ɓn�����̒�`
//�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E�E

// Vertex �\���̂̏����`
//glm::vec3 position;		// ���_���W (x, y, z)
//glm::vec3 normal;			// �@���x�N�g��(�ʂ̌���)
//glm::vec2 textureCoord;	// �e�N�X�`�����W (u, v)

// �o�C���f�B���O�̒�`   //��Ńe���v���[�g������
constexpr vk::VertexInputBindingDescription bindingDescription = [] {
    vk::VertexInputBindingDescription binding;
    binding.binding = 0;
    binding.stride = sizeof(Vertex);
    binding.inputRate = vk::VertexInputRate::eVertex;
    return binding;
}();

// ���͑����̒�`
constexpr std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = [] {
    std::array<vk::VertexInputAttributeDescription, 3> attribute;

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
    info.vertexBindingDescriptionCount = 1;
    info.pVertexBindingDescriptions = &bindingDescription;
    info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
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
