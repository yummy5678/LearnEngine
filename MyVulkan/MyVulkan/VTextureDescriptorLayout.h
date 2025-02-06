#pragma once
#include "VDescriptorLayoutBase.h"


class VTextureDescriptorSetLayout :
    public VDescriptorSetLayoutBase
{
public:
    VTextureDescriptorSetLayout();
    ~VTextureDescriptorSetLayout();

    void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
    


private:

    // �f�B�X�N���v�^�Z�b�g���C�A�E�g�o�C���f�B���O�̐ݒ�
    void CreateDescriptorSetLayout(uint32_t bindNumber);



};

