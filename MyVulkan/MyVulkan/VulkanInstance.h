#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include "GeneratorBase.h"
#include "GraphicsDefine.h"
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
	std::vector<std::string>* GetExtensionsPointer();
private:
	vk::ApplicationInfo		m_ApplicationInfo;	//このアプリの名前などを入れる構造体
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

// インスタンスについて
// Vulkanの機能を利用するのに必要になるもの
// まず最初にインスタンスを作成する必要がある
// インスタンスは一つのアプリケーションに１つとは限らず、複数個作成できる。
// この後デバイス、GPUや周辺機器の情報を見て、必要な情報を作成する。
//

// アプリケーションインフォ(ApplicationInfo)について
// メッセージの表示用にアプリケーションの名前などを設定できる。
// これを作成しなくてもインスタンスを作成することはできるが、
// 名前を付けたほうがやる気になれるので作ることを推奨する。
// 



// VkInstanceオブジェクトは 、
// そのオブジェクトのいずれかから作成された
// すべてのオブジェクトが破棄されたら破棄できます。