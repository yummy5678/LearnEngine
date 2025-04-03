#include "MaterialBuffer.h"

VMaterial::VMaterial() : 
	m_LogicalDevice(VK_NULL_HANDLE),
	m_Texture(),
	m_Sampler(VK_NULL_HANDLE)/*,
	m_DescriptorSets()*/
{
}

VMaterial::~VMaterial()
{
}

void VMaterial::SetMaterial(VmaAllocator* allocator, Material* material)
{
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);

	m_LogicalDevice = allocatorInfo.device;
	SetTexture(allocator, material->texture);

	if (m_Sampler == VK_NULL_HANDLE)
	{
		CreateSampler(allocatorInfo.device);
	}


	/*UpdateDescriptorSets(m_Texture.GetImageView(), m_Sampler);*/
}

vk::Image VMaterial::GetTextureBuffer()
{
	return m_Texture.GetImageBuffer();
}

vk::ImageView VMaterial::GetTextureImageView()
{
	return m_Texture.GetImageView();
}


vk::Sampler VMaterial::GetSampler()
{
	return m_Sampler;
}

//void VMaterial::SetMMaterialUpdateObserver(ObserveFunction function)
//{
//	m_BufferUpdateSubject.addObserver(function);
//}
//
//void VMaterial::DeleteMaterialUpdateObserver(ObserveFunction function)
//{
//	m_BufferUpdateSubject.removeObserver(function);
//}

//vk::DescriptorSet VMaterial::GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> sDescriptorSetLayout)
//{
//	// 既に登録済みなら何もしない
//	if (m_DescriptorSets.find(sDescriptorSetLayout) == m_DescriptorSets.end())
//		SetDescriptorSet(sDescriptorSetLayout);
//
//	return m_DescriptorSets[sDescriptorSetLayout].GetDescriptorSet();
//}

void VMaterial::SetTexture(VmaAllocator* allocator, Texture& texture)
{
	m_Texture.SetImage(allocator, texture, VK_NULL_HANDLE);
	//m_BufferUpdateSubject.notifyObservers();
}

void VMaterial::CreateSampler(vk::Device logicalDevice)
{
	vk::SamplerCreateInfo samplerCreateInfo;
	samplerCreateInfo.magFilter = vk::Filter::eLinear;
	samplerCreateInfo.minFilter = vk::Filter::eLinear;
	samplerCreateInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerCreateInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerCreateInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerCreateInfo.anisotropyEnable = false;
	samplerCreateInfo.maxAnisotropy = 16.0f;
	samplerCreateInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerCreateInfo.unnormalizedCoordinates = false;
	samplerCreateInfo.compareEnable = false;
	samplerCreateInfo.compareOp = vk::CompareOp::eAlways;
	samplerCreateInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 0.0f;

	m_Sampler = logicalDevice.createSampler(samplerCreateInfo);

}

//void VMaterial::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
//{
//	if (*layout.get() == VK_NULL_HANDLE)
//	{
//		std::cerr << "SetDescriptorSet: layout is invalid." << std::endl;
//		return;
//	}
//
//	// 必要に応じた VDescriptorBase 派生クラスのインスタンスを生成
//	VSingleTextureDescriptor descriptor;
//	descriptor.Initialize(m_LogicalDevice, *layout.get());
//	m_DescriptorSets.insert({ layout, descriptor });
//}
//
//void VMaterial::DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
//{
//	m_DescriptorSets.erase(layout);
//}
//
//void VMaterial::CleanupDescriptorSets()
//{
//	for (auto& pair : m_DescriptorSets)
//	{
//		// shared_ptrの中身が初期化されていたら削除
//		if (*pair.first.get() == VK_NULL_HANDLE)
//		{
//			m_DescriptorSets.erase(pair.first);
//		}
//	}
//}
//
//void VMaterial::UpdateDescriptorSets(vk::ImageView view, vk::Sampler sampler)
//{
//	// 登録されているすべてのディスクリプタセットを更新
//	for (auto& pair : m_DescriptorSets)
//	{
//		pair.second.Update(view, sampler);
//	}
//}
//
//

