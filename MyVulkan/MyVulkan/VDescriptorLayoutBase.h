#pragma once
#include <vulkan/vulkan.hpp>
#include "NonCopyable.h"

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

class VDescriptorSetLayoutBase : public NonCopyable
{
public:
	VDescriptorSetLayoutBase(vk::DescriptorType type, vk::ShaderStageFlags stageFlag);
	~VDescriptorSetLayoutBase();

	virtual std::shared_ptr<vk::DescriptorSetLayout>	GetDescriptorSetLayout();
	virtual vk::DescriptorType							GetDescriptorType();
	const vk::ShaderStageFlags							GetStageFlag();

protected:
	vk::Device*									m_pLogicalDevice;	// 作成＆破棄用のデバイス情報
	const vk::DescriptorType					m_DescriptorType;
	const vk::ShaderStageFlags					m_StageFlags;
	std::shared_ptr<vk::DescriptorSetLayout>	m_DescriptorSetLayout;


};

