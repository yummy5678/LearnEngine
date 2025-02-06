#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <unordered_map>
#include "DescriptorBase.h"
#include "SingleTextureDescriptor.h"

class TextureDescriptorManeger
{
public:
	TextureDescriptorManeger(vk::Device* pLogicalDevice);
	~TextureDescriptorManeger() = default;

	void SetDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを登録
	
	VSingleTextureDescriptor GetVDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを取得
	void DeleteDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認
	bool HasDescriptor(std::weak_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認

	void CleanupDeathOwner();// 無効になったオーナーを削除

	void UpdateAll(vk::ImageView imageView, vk::Sampler sampler);

private:
	vk::Device* m_pLogicalDevice;
	std::unordered_map<std::weak_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor, std::owner_less<std::weak_ptr<void>>>	m_Descriptors;




};

