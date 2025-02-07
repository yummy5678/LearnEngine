#pragma once
#include "MeshObject.h"
#include "VTextureBuffer.h"
#include "DescriptorHandleManeger.h"


class VMaterial
{
public:
	VMaterial();
	~VMaterial();

	void SetMaterial(VmaAllocator* allocator, Material material);

	vk::Image			GetTextureBuffer();
	vk::ImageView		GetTextureImageView();
	vk::Sampler			GetSampler();

	// �I�u�U�[�o�[�̊Ǘ��֐�
	//void SetMMaterialUpdateObserver(ObserveFunction function);
	//void DeleteMaterialUpdateObserver(ObserveFunction function);

	vk::DescriptorSet GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> wDescriptorSetLayout);

private:
	vk::Device			m_LogicalDevice;
	//glm::vec4 baseColor;	// ���쐬
	//float metallic;
	//float roughness;
	VTextureBuffer		m_Texture;

	vk::Sampler			m_Sampler;

	TextureDescriptorManager m_DescriptorManager;

	//Subject			m_BufferUpdateSubject;

	void SetTexture(VmaAllocator* allocator, Texture& texture);
	void CreateSampler(vk::Device logicalDevice);

};

