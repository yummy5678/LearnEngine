#pragma once
#pragma warning(disable : 4996)
#include <fstream>

const int MAX_FRAME_DRAWS = 3;




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
