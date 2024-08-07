#include "VulkanInstance.h"



InstanceGenerator::InstanceGenerator()
{
	m_ClassName = "InstanceGenerator";
	std::cout << m_ClassName << "のコンストラクタが呼ばれました" << std::endl;
}

InstanceGenerator::~InstanceGenerator()
{
	std::cout << m_ClassName << "のデストラクタが呼ばれました" << std::endl;
}

void InstanceGenerator::CreateInstance()
{
	m_ApplicationInfo = CreateApplicationInfo();
	auto instanceInfo = CreateInstanceInfo(&m_ApplicationInfo);

	try 
	{
		m_Instance = vk::createInstance(instanceInfo);
	}
	catch (const std::runtime_error& e)
	{
		// インスタンスの作成に失敗した場合のエラーメッセージ
		throw std::runtime_error("インスタンスの作成に失敗しました！");
	}

}

void InstanceGenerator::Create()
{
	std::cout << m_ClassName << "作成関数が呼ばれました" << std::endl;

	m_bCreated = true;	//作成フラグをオンにする(デバッグ用)
	CreateInstance();
}

void InstanceGenerator::Destroy()
{
	vkDestroyInstance(m_Instance, nullptr);
}

vk::Instance InstanceGenerator::GetInstanse()
{
	CheckCreated();
	return m_Instance;
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
	// ApplicationInfoの情報はGraphicsDefine.hで定義している
	vk::ApplicationInfo appInfo;
	appInfo.pNext = nullptr;										//拡張機能情報(これは触らなくていい)
	appInfo.pApplicationName = VulkanDefine::ApplicationName;       // アプリケーションの名前
	appInfo.applicationVersion = VulkanDefine::ApplicationVersion;  // アプリケーションのバージョン名 
	appInfo.pEngineName = VulkanDefine::EngineName;                 // エンジンの名前
	appInfo.engineVersion = VulkanDefine::EngineVersion;			// エンジンのバージョン名
	appInfo.apiVersion = VulkanDefine::ApiVersion;					// Vulkan APIのバージョン

	return appInfo;
}

const vk::InstanceCreateInfo InstanceGenerator::CreateInstanceInfo(const vk::ApplicationInfo* appInfo)
{
	// エラーチェック
	if (VulkanDefine::ValidationEnabled && !CheckValidationLayerSupport(validationLayers))
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}


	/*/////////////////////
	* インスタンスの作成
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;								// 拡張機能の情報 
	instanceInfo.flags;								// インスタンスの作成フラグ 
	instanceInfo.pApplicationInfo = appInfo;		// アプリケーション情報へのポインタ
	instanceInfo.enabledLayerCount = 0;				// 有効にするレイヤーの数 
	instanceInfo.ppEnabledLayerNames = nullptr;		// 有効にするレイヤーの名前の配列 
	instanceInfo.enabledExtensionCount = 0;			// 有効にする拡張機能の数 
	instanceInfo.ppEnabledExtensionNames = nullptr;	// 有効にする拡張機能の名前の配列 	


	// GLFWの拡張機能を取得する
	auto instanceExtensions = GetRequiredInstanceExtensionsPointer();

	// バリデーションが有効な場合、検証用のデバッグ情報拡張機能を追加する
	if (VulkanDefine::ValidationEnabled)
	{
		instanceExtensions->push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// インスタンスが必要とする拡張機能がサポートされているか確認する
	if (!CheckInstanceExtensionSupport(instanceExtensions))
	{
		throw std::runtime_error("VkInstance は必要な拡張機能をサポートしていません！");
	}

	// 有効な拡張機能の数と名前の配列を設定する
	instanceInfo.enabledExtensionCount = (uint32_t)instanceExtensions->size();
	instanceInfo.ppEnabledExtensionNames = instanceExtensions->data();

	// バリデーションが有効な場合、有効なレイヤーの数と名前の配列を設定する
	if (VulkanDefine::ValidationEnabled)
	{
		instanceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	}

	return instanceInfo;
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

//
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
