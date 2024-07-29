#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "GeneratorBase.h"
#include "VulkanValidation.h"

// インスタンス拡張機能のリストを作成する
class InstanceGenerator : public CGeneratorBase
{
public:
	InstanceGenerator();
	~InstanceGenerator();

	void Create();
	void Destroy();

	vk::Instance GetInstanse();

private:
	vk::ApplicationInfo		m_ApplicationInfo;
	vk::InstanceCreateInfo	m_InstanceInfo;
	vk::Instance			m_Instance;

	// インスタンス拡張機能のリストを作成する
	std::vector<const char*> instanceExtensions;

	//インスタンスの作成
	void CreateInstance();

	// GLFW でサーフェスを作るのに必要なインスタンス拡張を取得
	std::vector<const char*>*	GetRequiredInstanceExtensionsPointer();

	vk::ApplicationInfo				CreateApplicationInfo();
	const vk::InstanceCreateInfo	CreateInstanceInfo(const vk::ApplicationInfo* appInfo = nullptr);

	bool CheckValidationLayerSupport(const std::vector<const char*> validationLayers);
	bool CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions);

};

//VkInstanceオブジェクトは 、そのオブジェクトのいずれかから作成されたすべてのオブジェクトが破棄されたら破棄できます。