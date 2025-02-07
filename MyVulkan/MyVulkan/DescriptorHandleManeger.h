#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "DescriptorBase.h"
#include "SingleTextureDescriptor.h"

class TextureDescriptorManager
{
public:
	TextureDescriptorManager(vk::Device* pLogicalDevice);
	~TextureDescriptorManager() = default;

	void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを登録
	
	VSingleTextureDescriptor GetVDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを取得
	void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認
	bool HasDescriptor(std::shared_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認

	void CleanupDeathOwner();// 無効になったオーナーを削除

	void UpdateAll(vk::ImageView imageView, vk::Sampler sampler);

private:
	vk::Device* m_pLogicalDevice;
	std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor>	m_Descriptors;




};

