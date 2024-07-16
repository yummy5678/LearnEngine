#pragma once
#include <vulkan/vulkan.hpp>
#include <set>


// �L���[�t�@�~���[�̃C���f�b�N�X�i�ꏊ�j�i���݂���ꍇ�j
//indices��index�̕����`
struct QueueFamilyIndices {
    int graphicsFamily      = -1;   // �O���t�B�b�N�X�L���[�t�@�~���[�̏ꏊ
    int presentationFamily  = -1;   // �v���[���e�[�V�����L���[�t�@�~���[�̏ꏊ

    // �L���[�t�@�~���[���L�����ǂ������`�F�b�N����
    bool isValid()
    {
        return graphicsFamily >= 0 && presentationFamily >= 0;
    }
};

namespace QueueUtility
{
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

    //�v�Z�v�����󂯕t����L���[���󂯎��
    std::vector< vk::DeviceQueueCreateInfo > getQueueInfos(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

