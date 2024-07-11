#pragma once
#pragma warning(disable : 4996)
#include <fstream>

const int MAX_FRAME_DRAWS = 3;




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
