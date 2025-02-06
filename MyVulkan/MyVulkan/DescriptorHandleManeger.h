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

	void SetDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g��o�^
	
	VSingleTextureDescriptor GetVDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g���擾
	void DeleteDescriptorSet(std::weak_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F
	bool HasDescriptor(std::weak_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F

	void CleanupDeathOwner();// �����ɂȂ����I�[�i�[���폜

	void UpdateAll(vk::ImageView imageView, vk::Sampler sampler);

private:
	vk::Device* m_pLogicalDevice;
	std::unordered_map<std::weak_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor, std::owner_less<std::weak_ptr<void>>>	m_Descriptors;




};

