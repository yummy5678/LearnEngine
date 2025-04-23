#include "MeshObjectBuffer.h"

VMeshObject::VMeshObject() :
    m_Name(""),
	m_LogicalDevice(VK_NULL_HANDLE),
	m_Fence(VK_NULL_HANDLE),
    m_Transform(0),
    m_Mesh(),
    m_Material(),
	m_DescriptorSets()/*,
    m_MeshUpdateSubject(),
    m_MaterialUpdateSubject()*/
{
    m_Mesh = std::make_shared<VMesh>();
    m_Material = std::make_shared<VMaterial>();

}

VMeshObject::~VMeshObject()
{
	Cleanup();

}

void VMeshObject::SetMeshObject(VmaAllocator* allocator, MeshObject* meshObject)
{
	// �A���P�[�^�[�ɓo�^����Ă���_���f�o�C�X���擾
	VmaAllocatorInfo allocatorInfo;
	vmaGetAllocatorInfo(*allocator, &allocatorInfo);
	m_LogicalDevice = allocatorInfo.device;

	if (m_Fence == VK_NULL_HANDLE)
	{
		vk::FenceCreateInfo fenceInfo;
		fenceInfo.pNext;
		fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;
		m_Fence = m_LogicalDevice.createFence(fenceInfo);
	}


    SetMesh(allocator, &meshObject->mesh, m_Fence);
    SetMaterial(allocator, &meshObject->material, m_Fence);

	UpdateDescriptorSets(m_Material.get()->GetTextureImageView(), m_Material.get()->GetSampler());
}

void VMeshObject::Cleanup()
{
	if (m_LogicalDevice == VK_NULL_HANDLE) return;
	printf("���b�V����������܂�");

	CleanupDescriptorSets();

	// �}�e���A���̉������
	m_Material.get()->Cleanup();

	// ���b�V���̉������
	m_Mesh.get()->Cleanup();

	// �t�F���X�̉������
	if (m_Fence == VK_NULL_HANDLE)
	{
		m_LogicalDevice.destroyFence(m_Fence);
		m_Fence = VK_NULL_HANDLE;
	}

	// �����o�ϐ��̏�����
	//m_DescriptorSets.clear();
	m_Transform = {};
	m_Name.clear();
	m_LogicalDevice = VK_NULL_HANDLE;
}

//void VMeshObject::SetMeshUpdateObserver(Observer function)
//{
//    m_MeshUpdateSubject.addObserver(function);
//}
//
//void VMeshObject::DeleteMeshUpdateObserver(Observer function)
//{
//    m_MeshUpdateSubject.removeObserver(function);
//}
//
//void VMeshObject::SetMMaterialUpdateObserver(Observer function)
//{
//    m_MaterialUpdateSubject.addObserver(function);
//}
//
//void VMeshObject::DeleteMaterialUpdateObserver(Observer function)
//{
//    m_MaterialUpdateSubject.removeObserver(function);
//}

std::shared_ptr<VMesh> VMeshObject::GetSPMesh()
{
    return m_Mesh;
}

std::shared_ptr<VMaterial> VMeshObject::GetSPMaterial()
{
    return  m_Material;
}

vk::DescriptorSet VMeshObject::GetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> sDescriptorSetLayout)
{
	// ���ɓo�^�ς݂Ȃ牽�����Ȃ�
	if (m_DescriptorSets.find(sDescriptorSetLayout) == m_DescriptorSets.end())
		SetDescriptorSet(sDescriptorSetLayout);

	return m_DescriptorSets[sDescriptorSetLayout]->GetDescriptorSet();
}



void VMeshObject::SetMesh(VmaAllocator* allocator, Mesh* mesh, vk::Fence fence)
{
    m_Mesh->SetMesh(allocator, mesh, fence);

    // ���b�V���̓��e���X�V������o�^���Ă���e�I�u�U�[�o�[�ɒʒm
    //m_MeshUpdateSubject.notifyObservers();
}

void VMeshObject::SetMaterial(VmaAllocator* allocator, Material* material, vk::Fence fence)
{
    m_Material->SetMaterial(allocator, material, fence);

    // �}�e���A���̓��e���X�V������o�^���Ă���e�I�u�U�[�o�[�ɒʒm
    //m_MaterialUpdateSubject.notifyObservers();

}

void VMeshObject::SetDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
	if (*layout.get() == VK_NULL_HANDLE)
	{
		std::cerr << "SetDescriptorSet: layout is invalid." << std::endl;
		return;
	}

	// �K�v�ɉ����� VDescriptorBase �h���N���X�̃C���X�^���X�𐶐�
	VSingleTextureDescriptor* descriptor = new VSingleTextureDescriptor;
	descriptor->Initialize(m_LogicalDevice, *layout.get());
	descriptor->Update(m_Material.get()->GetTextureImageView(), m_Material.get()->GetSampler());
	m_DescriptorSets.insert({ layout, descriptor });
}

void VMeshObject::DeleteDescriptorSet(std::shared_ptr<vk::DescriptorSetLayout> layout)
{
	m_DescriptorSets.erase(layout);
}

void VMeshObject::CleanupDescriptorSets()
{
	for (auto& pair : m_DescriptorSets)
	{
		// shared_ptr�̒��g������������Ă�����폜
		if (*pair.first.get() == VK_NULL_HANDLE)
		{
			pair.second->Cleanup();
			free(pair.second);
			m_DescriptorSets.erase(pair.first);
		}
	}
}

void VMeshObject::UpdateDescriptorSets(vk::ImageView imageView, vk::Sampler sampler)
{
	// �o�^����Ă��邷�ׂẴf�B�X�N���v�^�Z�b�g���X�V
	for (auto& pair : m_DescriptorSets)
	{
		pair.second->Update(imageView, sampler);
	}
}



