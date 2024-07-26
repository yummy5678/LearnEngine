#include "Surface.h"

SurfaceGenerator::SurfaceGenerator()
{
	m_ClassName = "SurfaceGenerator";
}

SurfaceGenerator::~SurfaceGenerator()
{
}

void SurfaceGenerator::CreateWindowSurface(vk::Instance instance, GLFWwindow* window)
{
	m_bCreated = true;

	VkSurfaceKHR c_Surface;
	// サーフェスを作成する（サーフェス作成情報構造体を作成し、サーフェス作成関数を実行し、結果を返す）
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &c_Surface);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("サーフェスの作成に失敗しました！");
	}

	m_Surface = c_Surface;


}

VkSurfaceKHR SurfaceGenerator::GetSurface()
{
	CheckCreated();
	return m_Surface;
}

void SurfaceGenerator::Destroy(vk::Instance instance)
{
	vkDestroySurfaceKHR(instance, m_Surface, nullptr);
}
