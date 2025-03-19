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

	// オブザーバーの管理関数
	//void SetMMaterialUpdateObserver(ObserveFunction function);
	//void DeleteMaterialUpdateObserver(ObserveFunction function);

	/*vk::DescriptorSet GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> wDescriptorSetLayout);*/

private:
	vk::Device			m_LogicalDevice;
	//glm::vec4 baseColor;	// 未作成
	//float metallic;
	//float roughness;
	VMeshTextureBuffer		m_Texture;

	vk::Sampler			m_Sampler;

	//TextureDescriptorManager m_DescriptorManager;
	/*std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor>	m_DescriptorSets;*/
	//Subject			m_BufferUpdateSubject;

	void SetTexture(VmaAllocator* allocator, Texture& texture);
	void CreateSampler(vk::Device logicalDevice);

	//// ディスクリプタセットのリスト作成用関数達
	//void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// デスクリプタセットを登録
	//void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// 所有者が存在するか確認
	//void CleanupDescriptorSets();// 無効になったオーナーを削除
	//void UpdateDescriptorSets(vk::ImageView view, vk::Sampler sampler);
};

