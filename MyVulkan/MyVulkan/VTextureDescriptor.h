#pragma once
#include "DescriptorBase.h"


class VTextureDescriptor :
    public VDescriptorBase
{
public:
    VTextureDescriptor();
    ~VTextureDescriptor();

    void Initialize(vk::Device* pLogicalDevice);
    void Update(vk::ImageView imageView, vk::Sampler sampler);


private:


    // ディスクリプタセットレイアウトバインディングの設定
    void CreateDescriptorSetLayout();
    void CreateDescriptorPool();
    // ディスクリプタセットの割り当て
    void CreateDescriptorSet();

    void UpdateDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);



};

