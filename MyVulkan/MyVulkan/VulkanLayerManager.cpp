#include "VulkanLayerManager.h"


InstanceLayerManager::InstanceLayerManager()
{
}

InstanceLayerManager::~InstanceLayerManager()
{
}

void InstanceLayerManager::Add(const char* layerName)
{
	if (hasLayer(layerName) == false && CheckLayerSupport(layerName) == true)
	{
		std::cout << "���C���[:" << layerName << "���ǉ�����܂���" << std::endl;
		m_ExtensinList.push_back(layerName);
		return;
	}
	else
	{
		std::cout << "���C���[:" << layerName << "�͊m�F�ł��܂���ł���" << std::endl;
	}

}

bool InstanceLayerManager::hasLayer(const char* layerName)
{
	// m_ExtensinList��layerName�����݂��邩�m�F
	return std::find(m_ExtensinList.begin(), m_ExtensinList.end(), layerName) != m_ExtensinList.end();
}

std::vector<const char*>* InstanceLayerManager::GetList()
{
	return &m_ExtensinList;
}


bool InstanceLayerManager::CheckLayerSupport(const char* layerName)
{
	std::cout << "�w�肳�ꂽ���C���[�����p�o���邩�m�F" << std::endl;

	//���p�\�ȃ��C���[�̃��X�g�������Ă���
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	// �w�肳�ꂽ�o���f�[�V�������C���[�����p�\�ȃ��C���[���X�g�Ɋ܂܂�Ă��邩�`�F�b�N����

	for (const auto& availableLayer : availableLayers)
	{
		// ���C���[������v���邩�ǂ�����strcmp�Ŕ�r����
		if (strcmp(layerName, availableLayer.layerName) == 0)
		{
			// ���ׂĂ̎w�肳�ꂽ�o���f�[�V�������C���[�����������ꍇ��true��Ԃ�
			std::cout << "����" << std::endl;
			return true;
		}
	}

	// �w�肳�ꂽ���C���[��S�Č������Ȃ������ꍇ��false��Ԃ�
	std::cout << "�w�肳�ꂽ���C���[���������܂���ł���" << std::endl;
	return false;
}
