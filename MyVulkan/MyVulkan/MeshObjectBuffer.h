#pragma once
#include "NonCopyable.h"
#include "MeshBuffer.h"
#include "MaterialBuffer.h"
#include "MeshObject.h" 
//#include "Observer.h"


class VMeshObject //: public NonCopyable
{
public:
	VMeshObject();
	~VMeshObject();

	void SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject);

	void Cleanup();

	//// �I�u�U�[�o�[�̊Ǘ��֐�
	//void SetMMaterialUpdateObserver(Observer function);
	//void DeleteMaterialUpdateObserver(Observer function);

	std::shared_ptr<VMesh>		GetSPMesh();
	std::shared_ptr<VMaterial>	GetSPMaterial();

	vk::DescriptorSet GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> sDescriptorSetLayout);
	
private:
	std::string					m_Name;
	vk::Device					m_LogicalDevice;
	vk::Fence					m_Fence;
	Transform					m_Transform;	// �g�����X�t�H�[��
	std::shared_ptr<VMesh>		m_Mesh;
	std::shared_ptr<VMaterial>	m_Material;

	std::unordered_map<std::shared_ptr<vk::DescriptorSetLayout>, VSingleTextureDescriptor*>	m_DescriptorSets;

	//Subject			m_MeshUpdateSubject;
	//Subject			m_BufferUpdateSubject;



	void SetMesh(VmaAllocator* allocator, Mesh* mesh, vk::Fence fence);
	void SetMaterial(VmaAllocator* allocator, Material* material, vk::Fence fence);

	// �f�B�X�N���v�^�Z�b�g�̃��X�g�쐬�p�֐��B
	void SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// �f�X�N���v�^�Z�b�g��o�^
	void DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout);// ���L�҂����݂��邩�m�F
	void CleanupDescriptorSets();// �����ɂȂ����I�[�i�[���폜
	void UpdateDescriptorSets(vk::ImageView imageView, vk::Sampler sampler);
};
