#pragma once
#include <vulkan/vulkan.hpp>

/*
1. ディスクリプタセットとは？
ディスクリプタセットは、シェーダーにデータを渡すための 「接続ポイント」 のようなものです。
シェーダーがテクスチャ、バッファ（データの配列）、イメージなどを扱うとき、
そのデータがどこにあるのかを教えるために使います。
Vulkanではデータが直接シェーダーに渡されるのではなく、
ディスクリプタセットを通してその位置を指示します。

2. なぜディスクリプタセットが必要なのか？
Vulkanは非常に低レベルなグラフィックスAPIで、
システムメモリとGPUメモリの管理が必要です。ディスクリプタセットを使うことで、
プログラムが どのデータをシェーダーに渡すか を簡単に管理・変更できるようになります。
*/

class VDescriptorBase
{
public:
	VDescriptorBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag);
	~VDescriptorBase();


	virtual vk::DescriptorSet		GetDescriptorSet();
	virtual vk::DescriptorSetLayout GetDescriptorSetLayout();

	virtual bool HasDescriptor();

protected:
	vk::Device*					m_pLogicalDevice;	// 作成＆破棄用のデバイス情報
	const vk::DescriptorType	m_DescriptorType;
	const vk::ShaderStageFlags	m_StageFlags;
	vk::DescriptorSetLayout		m_DescriptorSetLayout;
	vk::DescriptorPool			m_DescriptorPool;
	vk::DescriptorSet			m_DescriptorSet;


};

