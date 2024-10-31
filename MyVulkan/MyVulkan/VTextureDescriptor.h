#pragma once
#include "DescriptorBase.h"


class VTextureDescriptor :
    public VDescriptorBase
{
public:
    VTextureDescriptor();
    ~VTextureDescriptor();

    // �f�B�X�N���v�^�Z�b�g�̊��蓖��
    vk::DescriptorSet CreateSingleDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);

    vk::DescriptorSetLayout GetDescriptorSetLayout() const;

private:
    // �f�B�X�N���v�^�Z�b�g���C�A�E�g�o�C���f�B���O�̐ݒ�
    void CreateDescriptorSetLayout();
    void CreateSingleDescriptorPool();


};

