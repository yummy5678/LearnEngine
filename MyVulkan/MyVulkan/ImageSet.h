#pragma once
#include <vulkan/vulkan.hpp>


// �J���[�C���[�W�Ɛ[�x�C���[�W���ꏏ�Ɏ�舵�����߂̍\����
// �������ꂾ���B
struct ImageViewSet
{
	vk::ImageView color;
	vk::ImageView depth;
};
