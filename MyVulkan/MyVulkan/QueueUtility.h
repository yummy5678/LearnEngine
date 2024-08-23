#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>

//�L���[���쐬����Ƃ��̏����l
constexpr uint32_t Number_NoneQueue = -1;

class QueueFamilySelector
{
public:
    QueueFamilySelector(vk::PhysicalDevice physicalDevice);
    ~QueueFamilySelector();

    uint32_t GetGraphicIndex();
    uint32_t GetComputeIndex();
    uint32_t GetTransferIndex();
    uint32_t GetPresentationIndex(vk::SurfaceKHR surface);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos(); //�f�o�C�X�̍쐬�p

private:   
    vk::PhysicalDevice m_PhysicalDevice;
    uint32_t m_GraphicsFamilyIndex       = Number_NoneQueue;  // �`��p�L���[�t�@�~���[�̏ꏊ
    uint32_t m_ComputeFamilyIndex        = Number_NoneQueue;  // �v�Z�p�L���[�t�@�~���[�̏ꏊ
    uint32_t m_TransferFamilyIndex       = Number_NoneQueue;  // �]���p�L���[�t�@�~���[�̏ꏊ
    uint32_t m_PresentationFamilyIndex   = Number_NoneQueue;  // �v���[���e�[�V�����L���[�t�@�~���[(�摜�̕\��)�̏ꏊ

    // �L���[�쐬���p�̃x�N�^�[
    std::vector<vk::DeviceQueueCreateInfo> m_QueueCreateInfos;

    uint32_t SearchGraphicsFamily();
    uint32_t SearchComputeFamily();
    uint32_t SearchTransferFamily();
    uint32_t SearchPresentationFamily(vk::SurfaceKHR surface);

    //�v�Z�v�����󂯕t����L���[���󂯎��
   // std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos();

};
