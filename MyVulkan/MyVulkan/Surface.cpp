#include "Surface.h"

vk::UniqueSurfaceKHR SurfaceUtilities::createUnique(vk::Instance& instance, GLFWwindow* window)
{
	VkSurfaceKHR rawSurface;
	// サーフェスを作成する（サーフェス作成情報構造体を作成し、サーフェス作成関数を実行し、結果を返す）
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &rawSurface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("サーフェスの作成に失敗しました！");
	}

	// コンストラクタの第二引数で親となるインスタンスの情報を渡す
	return vk::UniqueSurfaceKHR(rawSurface, instance);

}
