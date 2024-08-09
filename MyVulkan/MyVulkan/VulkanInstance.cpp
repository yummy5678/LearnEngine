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

void InstanceGenerator::Create()
{
	std::cout << m_ClassName << "作成関数が呼ばれました" << std::endl;

	m_bCreated = true;	//作成フラグをオンにする(デバッグ用)

	/*/////////////////////
	* レイヤーリストの作成
	*//////////////////////
	auto layers = GetLayers();

	/*/////////////////////
	* 拡張機能リストの作成
	*//////////////////////
	auto extensions = GetExtensions();

	m_ApplicationInfo = GetApplicationInfo();
	auto instanceInfo = GetInstanceInfo(&m_ApplicationInfo, &layers, &extensions);

	try
	{
		std::cout << m_ClassName << "インスタンスの作成に成功しました。" << std::endl;
		m_Instance = vk::createInstance(instanceInfo);
	}
	catch (const std::runtime_error& e)
	{
		// インスタンスの作成に失敗した場合のエラーメッセージ
		throw std::runtime_error("インスタンスの作成に失敗しました！");
	}
}

void InstanceGenerator::Destroy()
{
	if (m_bCreated == false) return;
	throw std::runtime_error("インスタンスを破壊しました！");
	vkDestroyInstance(m_Instance, nullptr);
	m_bCreated = false;
}

vk::Instance InstanceGenerator::GetInstanse()
{
	CheckCreated();
	return m_Instance;
}

std::vector<const char*> InstanceGenerator::GetRequiredInstanceExtensionsPointer()
{
	std::cout << m_ClassName << "GLFW拡張機能のリストの受け取り" << std::endl;

	// インスタンス拡張機能のリストを作成する
	std::vector<const char*> instanceExtensions;

	// インスタンスが使用する拡張機能を設定する
	uint32_t glfwExtensionCount = 0;	// GLFWは複数の拡張機能を要求する場合がある
	const char** glfwExtensions;		// 拡張機能はC文字列の配列として渡されるため、ポインタ(配列)のポインタ(C文字列)が必要

	// GLFWの拡張機能を取得する
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// GLFWの拡張機能をリストに追加する
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
		std::cout<< "拡張機能名：" << glfwExtensions[i] << std::endl;
	}

	return instanceExtensions;
}

std::vector<const char*> InstanceGenerator::GetLayers()
{
	//レイヤリストの取得
	if (VulkanDefine.ValidationEnabled)//検証レイヤーのフラグが立っていれば追加
	{
		m_LayerManager.AddLayer(LAYER_VALIDATION);
	}
	std::vector<const char*> layers = m_LayerManager.GetLayerList();
	CheckLayersSupport(layers);	//レイヤーが使用できるか確認
	return layers;
}

std::vector<const char*> InstanceGenerator::GetExtensions()
{
	// 拡張機能リストの作成
	auto instanceExtensions = GetRequiredInstanceExtensionsPointer();	// GLFWの拡張機能を取得する

	// バリデーションが有効な場合、検証用のデバッグ情報拡張機能を追加する
	if (VulkanDefine.ValidationEnabled)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		std::cout << "デバッグ拡張機能の追加" << std::endl;
	}

	// インスタンスが必要とする拡張機能がサポートされているか確認する
	if (!CheckExtensionsSupport(instanceExtensions))
	{
		throw std::runtime_error("インスタンスは必要な拡張機能をサポートしていません！");
	}

	return instanceExtensions;
}

vk::ApplicationInfo InstanceGenerator::GetApplicationInfo()
{
	std::cout << "アプリケーション情報の作成" << std::endl;

	// アプリケーションの情報を初期化
	// ここは好きな値を書き込む
	// ApplicationInfoの情報はGraphicsDefine.hで定義している
	vk::ApplicationInfo appInfo;
	appInfo.pNext = nullptr;										//拡張機能情報(これは触らなくていい)
	appInfo.pApplicationName = VulkanDefine.ApplicationName;		// アプリケーションの名前
	appInfo.applicationVersion = VulkanDefine.ApplicationVersion;	// アプリケーションのバージョン名 
	appInfo.pEngineName = VulkanDefine.EngineName;					// エンジンの名前
	appInfo.engineVersion = VulkanDefine.EngineVersion;				// エンジンのバージョン名
	appInfo.apiVersion = VulkanDefine.ApiVersion;					// Vulkan APIのバージョン


	return appInfo;
}

const vk::InstanceCreateInfo InstanceGenerator::GetInstanceInfo(
	const vk::ApplicationInfo* appInfo,
	const std::vector<const char*>* layers,
	const std::vector<const char*>* extensions)
{
	std::cout << "インスタンス作成情報の作成" << std::endl;

	/*/////////////////////
	* インスタンスの作成
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;									// 拡張機能の情報 
	instanceInfo.flags;									// インスタンスの作成フラグ 
	instanceInfo.pApplicationInfo = appInfo;			// アプリケーション情報へのポインタ
	instanceInfo.enabledLayerCount = layers->size();		// 有効にするレイヤーの数 
	instanceInfo.ppEnabledLayerNames = layers->data();	// 有効にするレイヤーの名前の配列 
	instanceInfo.enabledExtensionCount = (uint32_t)extensions->size();	// 有効にする拡張機能の数 
	instanceInfo.ppEnabledExtensionNames = extensions->data();			// 有効にする拡張機能の名前の配列 	

	return instanceInfo;
}

// 配列内の文字列が間違っていたり、
// 対応していないものだとエラーを出す
bool InstanceGenerator::CheckLayersSupport(const std::vector<const char*> validationLayers)
{
	std::cout << "指定されたレイヤーが利用出来るか確認" << std::endl;

	//利用可能なレイヤーのリストを持ってくる
	std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

	// 利用可能なレイヤーか引数のレイヤー数が0の場合はfalseを返す
	if (availableLayers.empty() || validationLayers.empty())
	{
		std::cout << "失敗" << std::endl;
		return false;
	}

	int hasCount = validationLayers.size();
	// 指定されたバリデーションレイヤーが利用可能なレイヤーリストに含まれているかチェックする
	for (const auto& validationLayer : validationLayers)
	{
		
		for (const auto& availableLayer : availableLayers)
		{
			// レイヤー名が一致するかどうかをstrcmpで比較する
			if (strcmp(validationLayer, availableLayer.layerName) == 0)
			{
				if (--hasCount == 0)
				{
					// すべての指定されたバリデーションレイヤーが見つかった場合はtrueを返す
					std::cout << "成功" << std::endl;
					return true;
				}
			}
		}
	}

	// 指定されたレイヤーを全て見つけられなかった場合はfalseを返す
	std::cout << "指定されたレイヤーを見つけられませんでした" << std::endl;
	return false;
}

// 配列内の文字列が間違っていたり、
// 対応していないものだとエラーを出す
bool InstanceGenerator::CheckExtensionsSupport(std::vector<const char*> checkExtensions)
{
	std::cout << "インスタンスの指定された拡張機能が利用出来るか確認" << std::endl;

	// Need to get number of extensions to create array of correct size to hold extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Create a list of VkExtensionProperties using count
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// Check if given extensions are in list of available extensions
	for (const auto& checkExtension : checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			if (strcmp(checkExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		if (!hasExtension)
		{
			std::cout << "失敗" << std::endl;
			return false;
		}
	}

	std::cout << "成功" << std::endl;
	return true;
}
