#pragma once
#include "MeshObject.h"
#include "VTextureBuffer.h"
#include "SingleTextureDescriptor.h"
//#include "DescriptorHandleManeger.h"


class VMaterial
{
public:
	VMaterial();
	~VMaterial();

	void SetMaterial(VmaAllocator* allocator, Material* material);

	vk::Image			GetTextureBuffer();
	vk::ImageView		GetTextureImageView();
	vk::Sampler			GetSampler();

	// �I�u�U�[�o�[�̊Ǘ��֐�
	//void SetMMaterialUpdateObserver(ObserveFunction function);
	//void DeleteMaterialUpdateObserver(ObserveFunction function);

	/*vk::DescriptorSet GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> wDescriptorSetLayout);*/

private:
	vk::Device			m_LogicalDevice;
	//glm::vec4 baseColor;	// ���쐬
	//float metallic;
	//float roughness;
	VMeshTextureBuffer		m_Texture;

	vk::Sampler			m_Sampler;

	//TextureDescriptorManager m_DescriptorManager;
	/*std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor>	m_DescriptorSets;*/
	//Subject			m_BufferUpdateSubject;

	void SetTexture(VmaAllocator* allocator, Texture& texture);
	void CreateSampler(vk::Device logicalDevice);

	//// �f�B�X�N���v�^�Z�b�g�̃��X�g�쐬�p�֐��B
	//void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g��o�^
	//void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F
	//void CleanupDescriptorSets();// �����ɂȂ����I�[�i�[���폜
	//void UpdateDescriptorSets(vk::ImageView view, vk::Sampler sampler);
};

