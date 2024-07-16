#include "ShaderUtility.h"

vk::ShaderModule ShaderUtility::createShaderModule(const std::vector<char>& code, vk::Device logicalDevice)
{
    // �V�F�[�_�[���W���[���쐬���̐ݒ�
    vk::ShaderModuleCreateInfo shaderModuleCreateInfo(
        {},                                             // VkStructureType �͎����I�ɐݒ肳���
        code.size(),                                    // �R�[�h�̃T�C�Y
        reinterpret_cast<const uint32_t*>(code.data())  // �R�[�h�̃|�C���^��uint32_t*�ɃL���X�g
    );

    // �V�F�[�_�[���W���[���̍쐬
    vk::ShaderModule shaderModule;
    shaderModule = logicalDevice.createShaderModule(shaderModuleCreateInfo);
    if (!shaderModule)
    {
        throw std::runtime_error("�V�F�[�_�[���W���[���̍쐬�Ɏ��s���܂����I");
    }

    return shaderModule;
}