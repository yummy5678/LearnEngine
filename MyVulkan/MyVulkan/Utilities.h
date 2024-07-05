#pragma once
#include <fstream>

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// Indices (locations) of Queue Families (if they exist at all)
struct QueueFamilyIndices {
	int graphicsFamily = -1;			// Location of Graphics Queue Family
	int presentationFamily = -1;		// Location of Presentation Queue Family

	// Check if queue families are valid
	bool isValid()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};

struct SwapChainDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;		// Surface properties, e.g. image size/extent
	std::vector<VkSurfaceFormatKHR> formats;			// Surface image formats, e.g. RGBA and size of each colour
	std::vector<VkPresentModeKHR> presentationModes;	// How images should be presented to screen
};

struct SwapchainImage {
	VkImage image;
	VkImageView imageView;
};

//ファイル読み込み関数
static std::vector<char> readFile(const std::string& filename)
{
    // 指定されたファイルからストリームを開く
    // std::ios::binary はファイルをバイナリモードで読み込むことを指示
    // std::ios::ate はファイルの最後から読み始めることを指示
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // ファイルストリームが正常に開かれたかを確認する
    if (!file.is_open())
    {
        throw std::runtime_error("ファイルを開けませんでした！");
    }

    // 現在の読み取り位置を取得し、それを使ってファイルバッファをリサイズする
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> fileBuffer(fileSize);

    // 読み取り位置をファイルの先頭に移動する
    file.seekg(0);

    // ファイルデータをバッファに読み込む（合計で "fileSize" バイトをストリームから読み込む）
    file.read(fileBuffer.data(), fileSize);

    // ストリームを閉じる
    file.close();

    return fileBuffer;
}
