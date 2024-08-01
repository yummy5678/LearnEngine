#pragma once
#include <vulkan/vulkan.hpp>
#include <set>
#include "GeneratorBase.h"

//�L���[���쐬����Ƃ��̏����l
constexpr int Number_NoneQueue = -1;

class QueueFamilyGenerator : CGeneratorBase
{
public:
    QueueFamilyGenerator();
    ~QueueFamilyGenerator();

    void Create(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    vk::Queue GetGraphics(vk::Device logicalDevice);
    vk::Queue GetPresentation(vk::Device logicalDevice);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos();

private:   
    int m_GraphicsFamilyIndex = Number_NoneQueue;      // �O���t�B�b�N�X�L���[�t�@�~���[�̏ꏊ
    int m_PresentationFamilyIndex = Number_NoneQueue;  // �v���[���e�[�V�����L���[�t�@�~���[(�摜�̕\��)�̏ꏊ

    // �L���[�쐬���p�̃x�N�^�[
    std::vector<vk::DeviceQueueCreateInfo> m_QueueCreateInfos;

    int SearchGraphicsFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
    int SearchPresentationFamily(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    //�v�Z�v�����󂯕t����L���[���󂯎��
    std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

};


//�ȉ�Queue�Ɋւ��郁��
//���� Vulkan �I�u�W�F�N�g�́A�L���[�����̃I�u�W�F�N�g���g�p����R�}���h�����s���Ă���Ԃ͔j�����Ă͂Ȃ�܂���B
//VkFence
//VkSemaphore
//VkCommandBuffer
//VkCommandPool