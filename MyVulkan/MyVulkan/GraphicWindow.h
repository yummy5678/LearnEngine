#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h> //�E�B���h�E�̍쐬��GLFW���C�u�������g��
#include <string>
#include "Surface.h"
#include "VulkanInitializer.h" 
#include "SwapchainRenderer.h"

struct RenderTask
{
    RenderConfig* config;
    std::vector<RenderObject>* objects;
    SceneCamera* camera;
};

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

    void AddDrawTask(RenderConfig* pRenderConfig, std::vector<RenderObject>* pObjects, SceneCamera* pCamera);
	void ExecuteDrawTask();

    //�E�B���h�E�̃|�C���^�[��n��
    GLFWwindow*     GetPointer();
    RendererBase*   GetRenderer();


    //�E�B���h�E�̏I���t���O�������Ă��邩�m�F
    int checkCloseWindow();

private:
    //GLFW�E�B���h�E�̃|�C���^�[
    GLFWwindow*             m_pWindow;

    VulkanInitializer*      m_pInitializer;

    SurfaceGenerator        m_Surface;
    SwapchainRenderer       m_GraphicController;

    DrawCommand             m_DrawCommand;
    std::vector<RenderTask> m_DrawTasks;

   
};


