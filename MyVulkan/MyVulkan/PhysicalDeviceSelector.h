#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <functional>
#include "QueueUtility.h"

struct PhysicalDeviceContainer
{
    vk::PhysicalDevice Handle = VK_NULL_HANDLE;
    std::vector<vk::DeviceQueueCreateInfo> QueueInfo;
};

class PhysicalDeviceSelector 
{
public:
    // �R���X�g���N�^
    PhysicalDeviceSelector(vk::Instance instance);

    // �`�揈���ɓK�����f�o�C�X��I��
    PhysicalDeviceContainer SelectGraphicsDevice();

    // �v�Z�����ɓK�����f�o�C�X��I��
    PhysicalDeviceContainer SelectComputeDevice();

    // �f�[�^�̓]�������ɓK�����f�o�C�X��I��
    PhysicalDeviceContainer SelectTransferDevice();

    // �X���b�v�`�F�C���A��ʕ\���ɓK�����f�o�C�X��I��
    PhysicalDeviceContainer SelectSwapchainDevice(vk::SurfaceKHR surface);


private:
    std::vector<vk::PhysicalDevice> m_PhysicalDevices;
    vk::Instance m_instance;

    std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos(std::set<int> queues);

    //�����f�o�C�X���w�肵���g���@�\�ɑΉ����Ă��邩�m�F
    bool CheckExtensionNames(vk::PhysicalDevice physicalDevice, std::vector<std::string> extensionNames);

};

