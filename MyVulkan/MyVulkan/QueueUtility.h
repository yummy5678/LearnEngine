#pragma once
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <set>

//�L���[���쐬����Ƃ��̏����l
constexpr uint32_t UndefinedQueueNumber = UINT32_MAX;   //�L���[�������������̔���Ɏg���l

class QueueFamilySelector
{
public:
    QueueFamilySelector();
    ~QueueFamilySelector();

    void Initialize(vk::PhysicalDevice physicalDevice);
    void Cleanup();

    uint32_t GetGraphicIndex();
    uint32_t GetComputeIndex();
    uint32_t GetTransferIndex();
    uint32_t GetPresentationIndex(vk::SurfaceKHR surface);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos(); //�f�o�C�X�̍쐬�p

private:   
    vk::PhysicalDevice  m_PhysicalDevice;
    uint32_t m_GraphicsFamilyIndex;  // �`��p�L���[�t�@�~���[�̏ꏊ
    uint32_t m_ComputeFamilyIndex;  // �v�Z�p�L���[�t�@�~���[�̏ꏊ
    uint32_t m_TransferFamilyIndex;  // �]���p�L���[�t�@�~���[�̏ꏊ
    uint32_t m_PresentationFamilyIndex;  // �v���[���e�[�V�����L���[�t�@�~���[(�摜�̕\��)�̏ꏊ


    uint32_t SearchGraphicsFamily();
    uint32_t SearchComputeFamily();
    uint32_t SearchTransferFamily();
    uint32_t SearchPresentationFamily(vk::SurfaceKHR surface);

    //�v�Z�v�����󂯕t����L���[���󂯎��
   // std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos();

};
