#include "VulkanInstance.h"
//VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE


InstanceGenerator::InstanceGenerator()
{
	m_ClassName = "InstanceGenerator";
	std::cout << m_ClassName << "のコンストラクタが呼ばれました" << std::endl;
}

InstanceGenerator::~InstanceGenerator()
{

}

void InstanceGenerator::Create(InstanceExtension extensionManager)
{
	std::cout << m_ClassName << "作成関数が呼ばれました" << std::endl;
	m_bCreated = true;	//作成フラグをオンにする(デバッグ用)

	/*/////////////////////
	* レイヤーリストの作成
	*//////////////////////


	extensionManager.GetExtensions();

	m_ApplicationInfo = GetApplicationInfo();
	auto instanceInfo = GetInstanceInfo(&m_ApplicationInfo, extensionManager);

	try
	{

		m_Instance = vk::createInstance(instanceInfo);
		std::cout << m_ClassName << "インスタンスの作成に成功しました。" << std::endl;
		if (VulkanDefine.ValidationEnabled)
		{
			createDebugCallback();
			std::cout << m_ClassName << "コールバックの作成に成功しました。" << std::endl;
		}
	}
	catch (const std::runtime_error& e)
	{
		// インスタンスの作成に失敗した場合のエラーメッセージ
		throw std::runtime_error("インスタンスの作成に失敗しました！");
	}
}

void InstanceGenerator::Destroy()
{
	//作成フラグが立っていない場合は解放処理も行わない
	if (m_bCreated == false) return;
	m_bCreated = false;

	// インスタンスの解放
	vkDestroyInstance(m_Instance, nullptr);
	throw std::runtime_error("インスタンスを解放しました！");
}

vk::Instance InstanceGenerator::GetInstanse()
{
	CheckCreated();
	return m_Instance;
}

void InstanceGenerator::createDebugCallback()
{
	// DispatchLoaderDynamicのセットアップ
	vk::DispatchLoaderDynamic dldi(m_Instance, vkGetInstanceProcAddr);

	vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo{
	vk::DebugUtilsMessengerCreateFlagsEXT{}, // 現在は空でOK
	vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning 
	| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError 
	//| vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
	//| vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
	,
	vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
	debugCallback, // コールバック関数
	nullptr        // pUserData
	};



	callback = m_Instance.createDebugUtilsMessengerEXTUnique(debugUtilsMessengerInfo, nullptr, dldi);
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
	InstanceExtension& extensionManager)
{
	std::cout << "インスタンス作成情報の作成" << std::endl;

	if (VulkanDefine.ValidationEnabled)//検証レイヤーのフラグが立っていれば追加
	{
		extensionManager.UseValidation();
	}
	auto extensions = extensionManager.GetExtensions();
	auto layers = extensionManager.GetValidationLayers();
	/*/////////////////////
	* インスタンスの作成
	*//////////////////////
	vk::InstanceCreateInfo	instanceInfo;
	instanceInfo.pNext;												
	instanceInfo.flags;												
	instanceInfo.pApplicationInfo = appInfo;					// アプリケーション情報へのポインタ
	instanceInfo.enabledLayerCount = layers->size();			// 有効にするレイヤーの数 
    instanceInfo.ppEnabledLayerNames = layers->data();			// 有効にするレイヤーの名前の配列
	instanceInfo.enabledExtensionCount = extensions->size();	// 有効にする拡張機能の数 
	instanceInfo.ppEnabledExtensionNames = extensions->data();	// 有効にする拡張機能の名前の配列 	

	return instanceInfo;
}


