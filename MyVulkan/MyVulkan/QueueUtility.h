#pragma once
#include <vulkan/vulkan.hpp>
#include <set>

//�L���[���쐬����Ƃ��̏����l
constexpr int Number_NoneQueue = -1;

struct QueueContainer
{

};

class QueueFamilySelector
{
public:
    QueueFamilySelector(vk::PhysicalDevice physicalDevice);
    ~QueueFamilySelector();

    int GetGraphicIndex();
    int GetComputeIndex();
    int GetTransferIndex();
    int GetPresentationIndex(vk::SurfaceKHR surface);
    std::vector<vk::DeviceQueueCreateInfo> GetQueueInfos(); //�f�o�C�X�̍쐬�p

private:   
    vk::PhysicalDevice m_PhysicalDevice;
    int m_GraphicsFamilyIndex       = Number_NoneQueue;  // �`��p�L���[�t�@�~���[�̏ꏊ
    int m_ComputeFamilyIndex        = Number_NoneQueue;  // �v�Z�p�L���[�t�@�~���[�̏ꏊ
    int m_TransferFamilyIndex       = Number_NoneQueue;  // �]���p�L���[�t�@�~���[�̏ꏊ
    int m_PresentationFamilyIndex   = Number_NoneQueue;  // �v���[���e�[�V�����L���[�t�@�~���[(�摜�̕\��)�̏ꏊ

    // �L���[�쐬���p�̃x�N�^�[
    std::vector<vk::DeviceQueueCreateInfo> m_QueueCreateInfos;

    int SearchGraphicsFamily();
    int SearchComputeFamily();
    int SearchTransferFamily();
    int SearchPresentationFamily(vk::SurfaceKHR surface);

    //�v�Z�v�����󂯕t����L���[���󂯎��
   // std::vector<vk::DeviceQueueCreateInfo> CreateQueueInfos();

};


//�ȉ�Queue�Ɋւ��郁��
//���� Vulkan �I�u�W�F�N�g�́A�L���[�����̃I�u�W�F�N�g���g�p����R�}���h�����s���Ă���Ԃ͔j�����Ă͂Ȃ�܂���B
//VkFence
//VkSemaphore
//VkCommandBuffer
//VkCommandPool