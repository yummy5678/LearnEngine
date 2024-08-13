#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stdexcept>
#include <vulkan/vulkan.hpp>


enum InstanceLayerList
{
	LAYER_VALIDATION,
	//Synchronization,
	//API_Dump,
	//Device_Simulation,
	//Screenshot
	LAYER_LIST_MAX_NUMBER
};

//�C���X�^���X�̔ԍ��Ɩ��O�����т���
//constexpr const char VULKAN_LAYER_VALIDATION[] = "VK_LAYER_LUNARG_standard_validation";
constexpr const char VULKAN_LAYER_VALIDATION[] = "VK_LAYER_KHRONOS_validation";



class InstanceLayerManager
{
public:
	InstanceLayerManager();
	~InstanceLayerManager();

	void Add(const char*);
	bool hasLayer(const char*);
	bool DeleteLayer(const char*);

	std::unordered_set<const char*> GetList();


private:
	
	std::unordered_set<const char*> m_ExtensinList;	//���e�̏d���������������set���g��

	//���C���[�����g�p�ł��邩�m�F
	bool CheckLayerSupport(const char* validationLayers);
};

// Vulkan�̃��C���[�Ƃ́H
// ����̎w�肵��Vulkan�̏����ɒǉ��̓����������
// ��Ƀp�t�H�[�}���X�̊m�F��f�o�b�O�Ŏg��
// �o���f�[�V����(����)���C���[��Vulkan�I�u�W�F�N�g�̐���������
// �G���[�����o���Ă����B
//
// ���C���[����"VK_LAYER_ �񋟎Җ� _ �@�\��"�ƂȂ��Ă���
























