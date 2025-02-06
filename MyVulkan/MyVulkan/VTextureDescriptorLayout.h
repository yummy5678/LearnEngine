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

    // ディスクリプタセットレイアウトバインディングの設定
    void CreateDescriptorSetLayout(uint32_t bindNumber);



};

