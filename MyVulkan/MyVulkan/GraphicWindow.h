#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h> //�E�B���h�E�̍쐬��GLFW���C�u�������g��
#include <string>
#include "Surface.h"
#include "VulkanInitializer.h" 
#include "SwapchainRenderer.h"
#include "RenderTask.h"

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


    void AddRenderQueue(RenderTask renderTask);
	void ExecuteRenderQueue();

    //�E�B���h�E�̃|�C���^�[��n��
    GLFWwindow* GetPointer();
    RendererBase* GetRenderer();

    //�E�B���h�E�̏I���t���O�������Ă��邩�m�F
    int checkCloseWindow();

private:
    //GLFW�E�B���h�E�̃|�C���^�[
    GLFWwindow* m_pWindow;

    VulkanInitializer* m_pInitializer;

    SurfaceGenerator m_Surface;
    SwapchainRenderer m_GraphicController;
	std::vector<RenderTask> m_RenderTasks;
};
