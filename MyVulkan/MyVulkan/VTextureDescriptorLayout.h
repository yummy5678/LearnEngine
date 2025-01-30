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

    // ディスクリプタセットレイアウトバインディングの設定
    void CreateDescriptorSetLayout(uint32_t bindNumber);



};

