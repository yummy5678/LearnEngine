#include "Surface.h"

VkSurfaceKHR VulkanCreate::GetWindowSurface(vk::Instance& instance, GLFWwindow* window)
{
	VkSurfaceKHR c_Surface;
	// サーフェスを作成する（サーフェス作成情報構造体を作成し、サーフェス作成関数を実行し、結果を返す）
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &c_Surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("サーフェスの作成に失敗しました！");
	}

	// vk::SurfaceKHR にキャストして返す
	return c_Surface;

}
