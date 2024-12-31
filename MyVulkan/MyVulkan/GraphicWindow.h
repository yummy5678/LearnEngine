#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h> //ウィンドウの作成にGLFWライブラリを使う
#include <string>
#include "Surface.h"
#include "VulkanInitializer.h" 
#include "SwapchainRenderer.h"
#include "RenderTask.h"

//ウィンドウ作成用のクラス
class GraphicWindow
{
public:
    //コンストラクタとデストラクタ
    GraphicWindow(VulkanInitializer& initializer);
    ~GraphicWindow();

    //ウィンドウの初期化と作成
    void init(const std::string wName, const int width, const int height);
    //ウィンドウの終了処理
    void kill();


    void AddRenderQueue(RenderTask renderTask);
	void ExecuteRenderQueue();

    //ウィンドウのポインターを渡す
    GLFWwindow* GetPointer();
    RendererBase* GetRenderer();

    //ウィンドウの終了フラグが立っているか確認
    int checkCloseWindow();

private:
    //GLFWウィンドウのポインター
    GLFWwindow* m_pWindow;

    VulkanInitializer* m_pInitializer;

    SurfaceGenerator m_Surface;
    SwapchainRenderer m_GraphicController;
	std::vector<RenderTask> m_RenderTasks;
};
