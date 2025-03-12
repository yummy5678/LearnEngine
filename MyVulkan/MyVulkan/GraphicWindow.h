#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>         //ウィンドウの作成にGLFWライブラリを使う
#include <string>
#include "Surface.h"
#include "VulkanInitializer.h" 
#include "RenderTargetBase.h"
#include "SwapchainRenderer.h"
#include "DrawCommand.h"
#include "RenderFunction.h"

//ウィンドウ作成用のクラス
class GraphicWindow : public RenderTarget
{
public:
    //コンストラクタとデストラクタ
    GraphicWindow(VulkanInitializer& initializer);
    ~GraphicWindow();

    //ウィンドウの初期化と作成
    void init(const std::string wName, const int width, const int height);
    //ウィンドウの終了処理
    void kill();

    void AddDrawTask(std::shared_ptr<RenderFunction> function);
	void ExecuteDrawTask();
    void Presentation();

    RenderingImageSet   GetImageSet() override;
    vk::Format			GetColorFormat() override;
    vk::Format			GetDepthFormat() override;
    vk::Extent2D		GetExtent()	override;

    //ウィンドウのポインターを渡す
    GLFWwindow*     GetPointer();

    //ウィンドウの終了フラグが立っているか確認
    int checkCloseWindow();

private:
    //GLFWウィンドウのポインター
    GLFWwindow*                 m_pWindow;

    VulkanInitializer*          m_pInitializer;

    SurfaceGenerator            m_Surface;
    SwapchainRenderer           m_Swapchain;

    std::vector<vk::Fence>      m_Fences;
    std::vector<DrawCommand>    m_DrawCommands;

    // スワップチェーンのフレーム数
    uint32_t                    m_SwapcheinFrameCount;

    // 画像の組が複数枚あるとき(主にスワップチェイン)の描画用インデックス
    RenderingImageSet           m_CurrentImageSet;

    // 今から描画するイメージインデックス
    uint32_t				    m_CurrentIndex;	// 用途 : フェンス、セマフォ
    // 次フレームで描画するイメージインデックス
    //uint32_t					m_NextIndex;	// 用途 : コマンド、画像


    //std::vector<RenderTask> m_DrawTasks;
    std::vector<std::shared_ptr<RenderFunction>> m_RenderFunctions;
   
};


