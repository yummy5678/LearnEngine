#include "VulkanInstance.h"




vk::ApplicationInfo VulkanCreate::GetApplicationInfo()
{
	// アプリケーションの情報を初期化
	// ここは好きな値を書き込む
	vk::ApplicationInfo appInfo = {};
	appInfo.pApplicationName = "Vulkan App";                     // アプリケーションの名前
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);       // アプリケーションのバージョン名 
	appInfo.pEngineName = "No Engine";                           // エンジンの名前
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);            // エンジンのバージョン名
	appInfo.apiVersion = VK_API_VERSION_1_0;                     // Vulkan APIのバージョン

	return appInfo;
}




VkInstanceCreateInfo VulkanCreate::GetInstanceInfo(vk::ApplicationInfo appInfo)
{
	// エラーチェック
	if (validationEnabled && !VulkanUtility::CheckValidationLayerSupport(validationLayers))
	{
		throw std::runtime_error("Required Validation Layers not supported!");
	}


	/*/////////////////////
	* インスタンスの作成
	*//////////////////////
	vk::InstanceCreateInfo createInfo = {};
	createInfo.pNext;											// 拡張機能の情報 
	createInfo.flags;											// インスタンスの作成フラグ 
	createInfo.pApplicationInfo = &appInfo;						// アプリケーション情報へのポインタ
	createInfo.enabledLayerCount = 0;							// 有効にするレイヤーの数 
	createInfo.ppEnabledLayerNames = nullptr;					// 有効にするレイヤーの名前の配列 
	createInfo.enabledExtensionCount = 0;						// 有効にする拡張機能の数 
	createInfo.ppEnabledExtensionNames = nullptr;				// 有効にする拡張機能の名前の配列 	

	// インスタンス拡張機能のリストを作成する
	static std::vector<const char*> instanceExtensions = std::vector<const char*>();//ここの値をスコープ外に移動させたい

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

	// バリデーションが有効な場合、検証用のデバッグ情報拡張機能を追加する
	if (validationEnabled)
	{
		instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	// インスタンスが必要とする拡張機能がサポートされているか確認する
	if (!VulkanUtility::CheckInstanceExtensionSupport(&instanceExtensions))
	{
		throw std::runtime_error("VkInstance は必要な拡張機能をサポートしていません！");
	}

	// 有効な拡張機能の数と名前の配列を設定する
	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// バリデーションが有効な場合、有効なレイヤーの数と名前の配列を設定する
	if (validationEnabled)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}

	auto hoge = createInfo;
	
    return hoge;//ここを抜けるとppEnabledLayerNamesが0xddddddddddになる
}








bool VulkanUtility::CheckValidationLayerSupport(const std::vector<const char*> validationLayers)
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

bool VulkanUtility::CheckInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	// Vulkanのインスタンスがサポートする利用可能な拡張機能の数を取得する
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// 利用可能な拡張機能の情報を保持するためのVkExtensionPropertiesのリストを作成する
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// 指定された拡張機能が利用可能な拡張機能リストに含まれているかチェックする
	for (const auto& checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			// 拡張機能名が一致するかどうかをstrcmpで比較する
			if (strcmp(checkExtension, extension.extensionName) == 0)
			{
				hasExtension = true;
				break;
			}
		}

		// 指定された拡張機能が見つからない場合はfalseを返す
		if (!hasExtension)
		{
			return false;
		}
	}

	// すべての指定された拡張機能が見つかった場合はtrueを返す
	return true;
}
