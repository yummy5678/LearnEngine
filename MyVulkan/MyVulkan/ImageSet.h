#pragma once
#include <vulkan/vulkan.hpp>

// �摜�̃o�b�t�@�Ƃ��̐ݒ���ꏏ�Ɉ������߂̍\����
struct ImageSet
{
	vk::Image		buffer	= VK_NULL_HANDLE;
	vk::ImageView	view	= VK_NULL_HANDLE;
};

// �`��R�}���h�p�ɃJ���[�C���[�W�Ɛ[�x�C���[�W���ꏏ�Ɏ�舵�����߂̍\����
struct RenderingImageSet
{
	ImageSet	color;
	ImageSet	depth;
};
