#pragma once
#include <iostream>
#include <vector>
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
//constexpr const char VULKAN_LAYER_VALIDATION[] = "VK_LAYER_LUNARG_standard_validation";	// ���o�[�W����
constexpr const char VULKAN_LAYER_VALIDATION[] = "VK_LAYER_KHRONOS_validation";



class InstanceLayerManager
{
public:
	InstanceLayerManager();
	~InstanceLayerManager();

	void Add(const char* layerName);
	bool hasLayer(const char* layerName);
	bool DeleteLayer(const char*);

	std::vector<const char*>* GetList();


private:
	
	std::vector<const char*> m_ExtensinList;	//���e�̏d���������������set���g��

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
























