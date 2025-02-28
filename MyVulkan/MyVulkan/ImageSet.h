#pragma once
#include <vulkan/vulkan.hpp>


struct ImageSet
{
	vk::Image		image;
	vk::ImageView	imageView;
};

// �J���[�C���[�W�Ɛ[�x�C���[�W���ꏏ�Ɏ�舵�����߂̍\����
// �������ꂾ���B
struct RenderingImageSet
{
	ImageSet	color;
	ImageSet	depth;
};
