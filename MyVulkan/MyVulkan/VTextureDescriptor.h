#pragma once
#include "DescriptorBase.h"


class VTextureDescriptor :
    public VDescriptorBase
{
public:
    VTextureDescriptor();
    ~VTextureDescriptor();

    // ディスクリプタセットの割り当て
    vk::DescriptorSet CreateSingleDescriptorSet(vk::ImageView imageView, vk::Sampler sampler);

    vk::DescriptorSetLayout GetDescriptorSetLayout() const;

private:
    // ディスクリプタセットレイアウトバインディングの設定
    void CreateDescriptorSetLayout();
    void CreateSingleDescriptorPool();


};

