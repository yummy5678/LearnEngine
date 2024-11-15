#include "InstanceExtensionManager.h"




InstanceExtension::InstanceExtension()
{
}

InstanceExtension::~InstanceExtension()
{
}

std::vector<const char*>* InstanceExtension::GetExtensions()
{
	m_ExtensionList.clear();
	
	if (m_bGLFW)
	{
		CreateGLFWExtension();
	}

	if (VulkanDefine.ValidationEnabled)
	{
		CreateValidationExtension();
	}

	if (!CheckExtensionsSupport((m_ExtensionList)))
	{
		//エラー
		//リストの拡張機能の名前をサポートしていなかった
		return nullptr;
	}
	return &m_ExtensionList;
}

void InstanceExtension::UseGLFW()
{
	m_bGLFW = true;
}

void InstanceExtension::CreateGLFWExtension()
{
	std::cout << "GLFW拡張機能のリストの受け取り" << std::endl;

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
		m_ExtensionList.push_back(glfwExtensions[i]);
		std::cout << "拡張機能名：" << glfwExtensions[i] << std::endl;
	}
}

void InstanceExtension::CreateValidationExtension()
{
	// バリデーションが有効な場合、検証用のデバッグ情報拡張機能を追加する
	m_ExtensionList.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	std::cout << "拡張機能名：" << VK_EXT_DEBUG_REPORT_EXTENSION_NAME << std::endl;
}

// 配列内の文字列が間違っていたり、
// 対応していないものだとエラーを出す
//bool InstanceExtensionManager::CheckExtensionSupport(const char* checkName)
//{
//    std::cout << "インスタンスの指定された拡張機能が利用出来るか確認" << std::endl;
//
//    // 拡張機能のリストを取得
//    auto extensions = vk::enumerateInstanceExtensionProperties();
//
//    // 指定された拡張機能がリストに存在するかチェック
//    for (const auto& extension : extensions) {
//        if (std::strcmp(checkName, extension.extensionName) == 0) 
//        {
//            std::cout << "成功" << std::endl;
//            return true;
//        }
//    }
//
//    std::cout << "失敗" << std::endl;
//    return false;
//}

bool InstanceExtension::CheckExtensionsSupport(std::vector<const char*> checkExtensionNames)
{
	std::cout << "インスタンスの指定された拡張機能が利用出来るか確認" << std::endl;

	// 拡張機能のリストを取得
	auto extensions = vk::enumerateInstanceExtensionProperties();

	// Check if given extensions are in list of available extensions
	for (const auto& checkExtension : checkExtensionNames)
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
