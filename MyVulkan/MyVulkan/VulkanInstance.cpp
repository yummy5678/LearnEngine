#include "VulkanInstance.h"


InstanceGenerator::InstanceGenerator()
{
	
}

InstanceGenerator::~InstanceGenerator()
{
}

void InstanceGenerator::CreateInstance()
{
	m_ApplicationInfo = CreateApplicationInfo();
	auto instanceInfo = CreateInstanceInfo(&m_ApplicationInfo);
	m_Instance = vk::createInstanceUnique(instanceInfo);
	// インスタンスの作成に失敗した場合のエラーメッセージ
	if (!m_Instance) 
	{
		throw std::runtime_error("Vulkanインスタンスの作成に失敗しました！");
	}

}

void InstanceGenerator::Create()
{
	CreateInstance();
}

vk::Instance InstanceGenerator::GetInstanse()
{
	return m_Instance.get();
}

std::vector<const char*>* InstanceGenerator::GetRequiredInstanceExtensionsPointer()
{
	// インスタンス拡張機能のリストを作成する
	static std::vector<const char*> instanceExtensions;	//static変数

	// インスタンスが使用する拡張機能を設定する
	uint32_t glfwExtensionCount = 0;	// GLFWは複数の拡張機能を要求する場合がある
	const char** glfwExtensions;		// 拡張機能はC文字列の配列として渡されるため、ポインタ(配列)のポインタ(C文字列)が必要

	// GLFWの拡張機能を取得する
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFWの拡張機能をリストに追加する
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	return &instanceExtensions;
}

vk::ApplicationInfo InstanceGenerator::CreateApplicationInfo()
{
	// アプリケーションの情報を初期化
	// ここは好きな値を書き込む
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = "Vulkan App";                     // アプリケーションの名前
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);       // アプリケーションのバージョン名 
	appInfo.pEngineName = "No Engine";                           // エンジンの名前
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);            // エンジンのバージョン名
	appInfo.apiVersion = VK_API_VERSION_1_0;                     // Vulkan APIのバージョン

	return appInfo;
}

const vk::InstanceCreateInfo InstanceGenerator::CreateInstanceInfo(const vk::ApplicationInfo* appInfo)
{
	// エラーチェック
	if (validationEnabled && !CheckValidationLayerSupport(validationLayers))
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}


	/*/////////////////////
	* インスタンスの作成
	*//////////////////////
	m_InstanceInfo.pNext;											// 拡張機能の情報 
	m_InstanceInfo.flags;											// インスタンスの作成フラグ 
	m_InstanceInfo.pApplicationInfo = appInfo;						// アプリケーション情報へのポインタ
	m_InstanceInfo.enabledLayerCount = 0;							// 有効にするレイヤーの数 
	m_InstanceInfo.ppEnabledLayerNames = nullptr;					// 有効にするレイヤーの名前の配列 
	m_InstanceInfo.enabledExtensionCount = 0;						// 有効にする拡張機能の数 
	m_InstanceInfo.ppEnabledExtensionNames = nullptr;				// 有効にする拡張機能の名前の配列 	


	// GLFWの拡張機能を取得する
	auto instanceExtensions = GetRequiredInstanceExtensionsPointer();

	// バリデーションが有効な場合、検証用のデバッグ情報拡張機能を追加する
	if (validationEnabled)
	{
		instanceExtensions->push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// インスタンスが必要とする拡張機能がサポートされているか確認する
	if (!CheckInstanceExtensionSupport(instanceExtensions))
	{
		throw std::runtime_error("VkInstance は必要な拡張機能をサポートしていません！");
	}

	// 有効な拡張機能の数と名前の配列を設定する
	m_InstanceInfo.enabledExtensionCount = (uint32_t)instanceExtensions->size();
	m_InstanceInfo.ppEnabledExtensionNames = instanceExtensions->data();

	// バリデーションが有効な場合、有効なレイヤーの数と名前の配列を設定する
	if (validationEnabled)
	{
		m_InstanceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
		m_InstanceInfo.ppEnabledLayerNames = validationLayers.data();
	}

	return m_InstanceInfo;
}

bool InstanceGenerator::CheckValidationLayerSupport(const std::vector<const char*> validationLayers)
{
	// バリデーションレイヤーの数を取得し、適切なサイズのベクターを作成する
	uint32_t validationLayerCount;
	vkEnumerateInstanceLayerProperties(&validationLayerCount, nullptr);

	// もしバリデーションレイヤーが見つからない場合かつ少なくとも1つのレイヤーが必要な場合はfalseを返す
	if (validationLayerCount == 0 && validationLayers.size() > 0)
	{
		return false;
	}

	// 利用可能なレイヤーの情報を保持するためのVkLayerPropertiesのリストを作成する
	std::vector<VkLayerProperties> availableLayers(validationLayerCount);
	vkEnumerateInstanceLayerProperties(&validationLayerCount, availableLayers.data());

	// 指定されたバリデーションレイヤーが利用可能なレイヤーリストに含まれているかチェックする
	for (const auto& validationLayer : validationLayers)
	{
		bool hasLayer = false;
		for (const auto& availableLayer : availableLayers)
		{
			// レイヤー名が一致するかどうかをstrcmpで比較する
			if (strcmp(validationLayer, availableLayer.layerName) == 0)
			{
				hasLayer = true;
				break;
			}
		}

		// 指定されたバリデーションレイヤーが見つからない場合はfalseを返す
		if (!hasLayer)
		{
			return false;
		}
	}

	// すべての指定されたバリデーションレイヤーが見つかった場合はtrueを返す
	return true;
}

bool InstanceGenerator::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	// インスタンスが使用する拡張機能を設定する
	uint32_t glfwExtensionCount = 0;	// GLFWは複数の拡張機能を要求する場合がある
	const char** glfwExtensions;		// 拡張機能はC文字列の配列として渡されるため、ポインタ(配列)のポインタ(C文字列)が必要

	// GLFWの拡張機能を取得する
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFWの拡張機能をリストに追加する
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	return &instanceExtensions;
}
