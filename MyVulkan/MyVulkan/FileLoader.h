#pragma once
#include <fstream>



namespace GraphicsUtility
{
    static std::vector<char> readFile(const std::string& filename)
    {

        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            throw std::runtime_error("ÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩ");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> fileBuffer(fileSize);

        file.seekg(0);

        file.read(fileBuffer.data(), fileSize);

        file.close();

        return fileBuffer;
    }
}
