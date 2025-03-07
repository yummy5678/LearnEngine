#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>         //�E�B���h�E�̍쐬��GLFW���C�u�������g��
#include <string>
#include "Surface.h"
#include "VulkanInitializer.h" 
#include "SwapchainRenderer.h"
#include "DrawCommand.h"
#include "RenderFunction.h"

//struct RenderTask
//{
//    RenderConfig* config;
//    std::vector<RenderObject*>* objects;
//    SceneCamera* camera;
//};


//�E�B���h�E�쐬�p�̃N���X
class GraphicWindow
{
public:
    //�R���X�g���N�^�ƃf�X�g���N�^
    GraphicWindow(VulkanInitializer& initializer);
    ~GraphicWindow();

    //�E�B���h�E�̏������ƍ쐬
    void init(const std::string wName, const int width, const int height);
    //�E�B���h�E�̏I������
    void kill();

    void AddDrawTask(std::shared_ptr<RenderFunction> function);
	void ExecuteDrawTask();
    void Presentation();

    //�E�B���h�E�̃|�C���^�[��n��
    GLFWwindow*     GetPointer();
    RenderTarget*   GetRenderer();


    //�E�B���h�E�̏I���t���O�������Ă��邩�m�F
    int checkCloseWindow();

private:
    //GLFW�E�B���h�E�̃|�C���^�[
    GLFWwindow*                 m_pWindow;

    VulkanInitializer*          m_pInitializer;

    SurfaceGenerator            m_Surface;
    SwapchainRenderer           m_Swapchain;

    std::vector<vk::Fence>      m_Fences;
    std::vector<DrawCommand>    m_DrawCommands;

    // �摜�̑g������������Ƃ�(��ɃX���b�v�`�F�C��)�̕`��p�C���f�b�N�X
    std::vector<RenderingImageSet>  m_RenderingImage;
    // ������`�悷��C���[�W�C���f�b�N�X
    uint32_t					    m_CurrentIndex;	// �p�r : �t�F���X�A�Z�}�t�H
    // ���t���[���ŕ`�悷��C���[�W�C���f�b�N�X
    uint32_t					    m_NextIndex;	// �p�r : �R�}���h�A�摜
    //std::vector<RenderTask> m_DrawTasks;
    std::vector<std::shared_ptr<RenderFunction>> m_RenderFunctions;
   
};


