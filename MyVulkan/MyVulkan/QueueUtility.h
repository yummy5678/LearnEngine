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

namespace VulkanCreate
{
    //�v�Z�v�����󂯕t����L���[���󂯎��
    std::vector< vk::DeviceQueueCreateInfo > GetQueueInfos(vk::PhysicalDevice physicalDevice, VkSurfaceKHR surface);
}

namespace VulkanUtility
{
	QueueFamilyIndices GetQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface);
}


//�ȉ�Queue�Ɋւ��郁��
//���� Vulkan �I�u�W�F�N�g�́A�L���[�����̃I�u�W�F�N�g���g�p����R�}���h�����s���Ă���Ԃ͔j�����Ă͂Ȃ�܂���B
//VkFence
//VkSemaphore
//VkCommandBuffer
//VkCommandPool