#pragma once
#include "DescriptorBase.h"


class VTextureDescriptor :
    public VDescriptorBase
{
public:
    VTextureDescriptor();
    ~VTextureDescriptor();

    void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
    void Update(vk::ImageView imageView, vk::Sampler sampler);


private:


    // �f�B�X�N���v�^�Z�b�g���C�A�E�g�o�C���f�B���O�̐ݒ�
    void CreateDescriptorSetLayout(uint32_t bindNumber);
    void CreateDescriptorPool();
    // �f�B�X�N���v�^�Z�b�g�̊��蓖��
    void CreateDescriptorSet();

    void UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);



};

