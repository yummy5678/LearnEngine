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

//�t�@�C���ǂݍ��݊֐�
static std::vector<char> readFile(const std::string& filename)
{
    // �w�肳�ꂽ�t�@�C������X�g���[�����J��
    // std::ios::binary �̓t�@�C�����o�C�i�����[�h�œǂݍ��ނ��Ƃ��w��
    // std::ios::ate �̓t�@�C���̍Ōォ��ǂݎn�߂邱�Ƃ��w��
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    // �t�@�C���X�g���[��������ɊJ���ꂽ�����m�F����
    if (!file.is_open())
    {
        throw std::runtime_error("�t�@�C�����J���܂���ł����I");
    }

    // ���݂̓ǂݎ��ʒu���擾���A������g���ăt�@�C���o�b�t�@�����T�C�Y����
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> fileBuffer(fileSize);

    // �ǂݎ��ʒu���t�@�C���̐擪�Ɉړ�����
    file.seekg(0);

    // �t�@�C���f�[�^���o�b�t�@�ɓǂݍ��ށi���v�� "fileSize" �o�C�g���X�g���[������ǂݍ��ށj
    file.read(fileBuffer.data(), fileSize);

    // �X�g���[�������
    file.close();

    return fileBuffer;
}
