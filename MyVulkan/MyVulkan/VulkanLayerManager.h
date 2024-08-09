#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>


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
const std::unordered_map<InstanceLayerList, const char*> gLayerMap
{
	//{LAYER_VALIDATION,	"VK_LAYER_LUNARG_standard_validation"},
	{ LAYER_VALIDATION, "VK_LAYER_KHRONOS_validation" },

};


class InstanceLayerManager
{
public:
	InstanceLayerManager();
	~InstanceLayerManager();

	void AddLayer(InstanceLayerList layer);
	bool hasLayer(InstanceLayerList layer);
	bool DeleteLayer(InstanceLayerList layer);

	std::vector<const char*> GetLayerList();


private:

	std::vector<const char*> m_LayerList;





};

// Vulkan�̃��C���[�Ƃ́H
// ����̎w�肵��Vulkan�̏����ɒǉ��̓����������
// ��Ƀp�t�H�[�}���X�̊m�F��f�o�b�O�Ŏg��
// �o���f�[�V����(����)���C���[��Vulkan�I�u�W�F�N�g�̐���������
// �G���[�����o���Ă����B
//
// ���C���[����"VK_LAYER_ �񋟎Җ� _ �@�\��"�ƂȂ��Ă���
























