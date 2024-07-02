#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>	//ウィンドウの作成にGLFWライブラリを使う
#include <string>

//ウィンドウ作成用のクラス
class GameWindow
{
public:
	//コンストラクタとデストラクタ
	GameWindow();
	~GameWindow();

	//ウィンドウの初期化と作成
	void init(std::string wName = "Standard Window", const int width = 800, const int height = 600);
	//ウィンドウの終了処理
	void kill();

	//ウィンドウのポインターを渡す
	GLFWwindow* getWindowPointer();

	//ウィンドウの終了フラグが立っているか確認
	int checkCloseWindow();

private:
	//GLFWウィンドウのポインター
	GLFWwindow* window = nullptr;
};

