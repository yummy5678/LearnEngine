#pragma once
#include "VDescriptorLayoutBase.h"


class VTextureDescriptorLayout :
    public VDescriptorLayoutBase
{
public:
    VTextureDescriptorLayout();
    ~VTextureDescriptorLayout();

    void Initialize(vk::Device* pLogicalDevice, uint32_t bindNumber);
    


private:

    // �f�B�X�N���v�^�Z�b�g���C�A�E�g�o�C���f�B���O�̐ݒ�
    void CreateDescriptorSetLayout(uint32_t bindNumber);



};

