#include "VulkanExtensionManager.h"


InstanceExtensionManager::InstanceExtensionManager()
{
}

InstanceExtensionManager::~InstanceExtensionManager()
{
}

void InstanceExtensionManager::Add(const char* extensionName)
{
	if (CheckExtensionSupport(extensionName))
	{
		std::cout << "インスタンス拡張機能:" << extensionName << "が追加されました" << std::endl;
		m_ExtensionList.insert(extensionName);
	}
	else
	{
		std::cout << "インスタンス拡張機能:" << extensionName << "が確認できませんでした" << std::endl;
	}

}

bool InstanceExtensionManager::Delete(const char* extensionName)
{
    return false;
}

std::unordered_set<const char*> InstanceExtensionManager::GetList()
{
    return m_ExtensionList;
}

// 配列内の文字列が間違っていたり、
// 対応していないものだとエラーを出す
bool InstanceExtensionManager::CheckExtensionSupport(const char* checkName)
{
    std::cout << "インスタンスの指定された拡張機能が利用出来るか確認" << std::endl;

    // 拡張機能のリストを取得
    auto extensions = vk::enumerateInstanceExtensionProperties();

    // 指定された拡張機能がリストに存在するかチェック
    for (const auto& extension : extensions) {
        if (std::strcmp(checkName, extension.extensionName) == 0) 
        {
            std::cout << "成功" << std::endl;
            return true;
        }
    }

    std::cout << "失敗" << std::endl;
    return false;
}
