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

	void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g��o�^
	
	VSingleTextureDescriptor GetVDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g���擾
	void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F
	bool HasDescriptor(std::shared_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F

	void CleanupDeathOwner();// �����ɂȂ����I�[�i�[���폜

	void UpdateAll(vk::ImageView imageView, vk::Sampler sampler);

private:
	vk::Device* m_pLogicalDevice;
	std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor>	m_Descriptors;




};

