#include "Surface.h"


SurfaceGenerator::SurfaceGenerator(VulkanInitializer& initializer)
{
	m_ClassName = "SurfaceGenerator";
	
	// 拡張機能マネージャーにサーフェスの使用する拡張機能を追加
	initializer.GetPInstanceExtension()->UseGLFW();
}

SurfaceGenerator::SurfaceGenerator()
{
	m_ClassName = "SurfaceGenerator";
}

SurfaceGenerator::~SurfaceGenerator()
{
	Destroy(m_Instance);
}

void SurfaceGenerator::CreateWindowSurface(vk::Instance instance, GLFWwindow* m_pWindow)
{
	m_bCreated = true;
	m_Instance = instance;

	VkSurfaceKHR c_Surface;
	// サーフェスを作成する（サーフェス作成情報構造体を作成し、サーフェス作成関数を実行し、結果を返す）
	VkResult result = glfwCreateWindowSurface(instance, m_pWindow, nullptr, &c_Surface);

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

vk::SurfaceCapabilitiesKHR SurfaceGenerator::GetCapabilities(vk::PhysicalDevice physicalDevice)
{
	CheckCreated();
	return physicalDevice.getSurfaceCapabilitiesKHR(m_Surface);
}

std::vector<vk::SurfaceFormatKHR> SurfaceGenerator::GetFomats(vk::PhysicalDevice physicalDevice)
{
	CheckCreated();
	return physicalDevice.getSurfaceFormatsKHR(m_Surface);
}

void SurfaceGenerator::Destroy(vk::Instance instance)
{
	vkDestroySurfaceKHR(instance, m_Surface, nullptr);
}

//std::vector<const char*> SurfaceGenerator::GetGLFWSurfaceExtensions()
//{
//	std::cout << m_ClassName << "GLFW拡張機能のリストの受け取り" << std::endl;
//
//	// インスタンス拡張機能のリストを作成する
//	std::vector<const char*> instanceExtensions;
//
//	// インスタンスが使用する拡張機能を設定する
//	uint32_t glfwExtensionCount = 0;	// GLFWは複数の拡張機能を要求する場合がある
//	const char** glfwExtensions;		// 拡張機能はC文字列の配列として渡されるため、ポインタ(配列)のポインタ(C文字列)が必要
//
//	// GLFWの拡張機能を取得する
//	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//	// GLFWの拡張機能をリストに追加する
//	for (size_t i = 0; i < glfwExtensionCount; i++)
//	{
//		instanceExtensions.push_back(glfwExtensions[i]);
//		std::cout << "拡張機能名：" << glfwExtensions[i] << std::endl;
//	}
//
//	return instanceExtensions;
//}
