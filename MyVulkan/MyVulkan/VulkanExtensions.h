#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>


//�f�o�C�X�Ŏg�p����g���@�\
//const std::vector<const char*> deviceExtensions = {
//	VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};


//�@�g���@�\�ɂ́A�C���X�^���X�g���@�\�ƃf�o�C�X�g���@�\��2�̃O���[�v������܂��B
// �ȒP�Ɍ����΁A�C���X�^���X�g���@�\ �� VkInstance �S�̂Ɍ��ѕt�����A
// �f�o�C�X�g���@�\�͒P��� VkDevice �I�u�W�F�N�g�ɂ̂݌��ѕt�����܂��B

/*//////////////////////////////////////////
* �C���X�^���X�̊g���@�\�ꗗ(ChatGPT�ɂč쐬)
*///////////////////////////////////////////
/////////�W���g���@�\/////////
//�EVK_KHR_ANDROID_SURFACE_EXTENSION_NAME
//Android�v���b�g�t�H�[����ł̃T�[�t�F�X�쐬���T�|�[�g���܂��B
// 
//�EVK_KHR_DISPLAY_EXTENSION_NAME
//���ڃf�B�X�v���C�ɕ`�悷�邽�߂̋@�\��񋟂��܂��B
// 
//�EVK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
//�f�B�X�v���C�p�X���b�v�`�F�[�����T�|�[�g���܂��B
// 
//�EVK_KHR_EXTERNAL_FENCE_EXTENSION_NAME
//�O���t�F���X�Ƃ̑��݉^�p�����T�|�[�g���܂��B
// 
//�EVK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME
//Linux�v���b�g�t�H�[���ł̊O���t�F���X�t�@�C���f�B�X�N���v�^�Ƃ̑��݉^�p�����T�|�[�g���܂��B
// 
//�EVK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME
//�O���������Ƃ̑��݉^�p�����T�|�[�g���܂��B
// 
//�EVK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME
//Linux�v���b�g�t�H�[���ł̊O���������t�@�C���f�B�X�N���v�^�Ƃ̑��݉^�p�����T�|�[�g���܂��B
// 
//VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
//�f�o�C�X�̃v���p�e�B���擾���邽�߂̊g���@�\�ł��B
// 
//�EVK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
//�T�[�t�F�X�̋@�\���擾���邽�߂̊g���@�\�ł��B
// 
//�EVK_KHR_SURFACE_EXTENSION_NAME
//Vulkan�T�[�t�F�X�Ƃ̃C���^�[�t�F�[�X��񋟂��܂��B
// 
//�EVK_KHR_SWAPCHAIN_EXTENSION_NAME
//�X���b�v�`�F�[�����T�|�[�g���܂��B
// 
//�EVK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
//Wayland�T�[�t�F�X�Ƃ̃C���^�[�t�F�[�X��񋟂��܂��iLinux�v���b�g�t�H�[���p�j�B
// 
//�EVK_KHR_WIN32_SURFACE_EXTENSION_NAME
//Windows�v���b�g�t�H�[����ł̃T�[�t�F�X�쐬���T�|�[�g���܂��B
// 
//�EVK_KHR_XCB_SURFACE_EXTENSION_NAME
//XCB�T�[�t�F�X�Ƃ̃C���^�[�t�F�[�X��񋟂��܂��iLinux�v���b�g�t�H�[���p�j�B
// 
//�EVK_KHR_XLIB_SURFACE_EXTENSION_NAME
//Xlib�T�[�t�F�X�Ƃ̃C���^�[�t�F�[�X��񋟂��܂��iLinux�v���b�g�t�H�[���p�j�B
// 
//�EVK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
//�p�t�H�[�}���X�N�G�����T�|�[�g���܂��B
// 
//�EVK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
//�T���v���[��YCbCr�ϊ����T�|�[�g���܂��B
// 
//�EVK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
//�v�b�V���f�B�X�N���v�^�̃T�|�[�g��񋟂��܂��B
// 
//�EVK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
//Vulkan���������f�����T�|�[�g���܂��B
// 
//�EVK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
//Vulkan�|�[�^�r���e�B�T�u�Z�b�g���T�|�[�g���܂��B
// 
//�EVK_KHR_PRESENT_WAIT_EXTENSION_NAME
//�v���[���g�ҋ@���T�|�[�g���܂��B

/////////�x���_�[�ŗL�g���@�\/////////
//�EVK_EXT_DEBUG_UTILS_EXTENSION_NAME
//�f�o�b�O�ƃg���[�X�̂��߂̊g���@�\�ł��B
// 
//�EVK_EXT_VALIDATION_CACHE_EXTENSION_NAME
//�V�F�[�_�[�R���p�C���̃L���b�V����񋟂��܂��B
// 
//�EVK_NV_EXTERNAL_MEMORY_FD_EXTENSION_NAME
//NVIDIA��GPU�ł̊O���������t�@�C���f�B�X�N���v�^�Ƃ̑��݉^�p�����T�|�[�g���܂��B
// 
//�EVK_NV_RAY_TRACING_EXTENSION_NAME
//NVIDIA��GPU�ł̃��C�E�g���[�V���O���T�|�[�g���܂��B
// 
//�EVK_AMD_BUFFER_MARKER_EXTENSION_NAME
//AMD��GPU�ł̃o�b�t�@�}�[�J�[�@�\��񋟂��܂��B
// 
//�EVK_EXT_SWAPCHAIN_COLORSPACE_EXTENSION_NAME
//�X���b�v�`�F�[���̐F��ԕϊ����T�|�[�g���܂��B




/*//////////////////////////////////////////
* �f�o�C�X�̊g���@�\�ꗗ(ChatGPT�ɂč쐬)
*///////////////////////////////////////////
/////////�W���g���@�\/////////
//�EVK_KHR_swapchain
//�X���b�v�`�F�[�����T�|�[�g���邽�߂̋@�\��񋟂��܂��B
// 
//�EVK_KHR_surface
//�\�ʁi�E�B���h�E�j�Ƃ̃C���^�[�t�F�[�X��񋟂��܂��B
// 
//�EVK_KHR_surface_protected
//�ی삳�ꂽ�T�[�t�F�X���T�|�[�g���܂��B
// 
//�EVK_KHR_display
//�f�B�X�v���C�i�O�����j�^�[�j�Ƃ̃C���^�[�t�F�[�X��񋟂��܂��B
// 
//�EVK_KHR_device_group
//�f�o�C�X�O���[�v�Ɋ֘A����@�\��񋟂��܂��B
// 
//�EVK_KHR_get_physical_device_properties2
//�f�o�C�X�̃v���p�e�B���擾���邽�߂̊g���@�\�ł��B
// 
//�EVK_KHR_multiview
//�}���`�r���[�i�����̃r���[���������_�����O�j���T�|�[�g���܂��B
// 
//�EVK_KHR_push_descriptor
//�v�b�V���f�B�X�N���v�^�i�V�F�[�_�[�ɓ��I�Ƀf�[�^��񋟂���j���T�|�[�g���܂��B
// 
//�EVK_KHR_shader_float16_int8
//16�r�b�g���������_�����8�r�b�g�����^�̃V�F�[�_�[���T�|�[�g���܂��B
// 
//�EVK_KHR_sampler_ycbcr_conversion
//�T���v���[��YCbCr�ϊ����T�|�[�g���܂��B
// 
//�EVK_KHR_timeline_semaphore
//�^�C�����C���Z�}�t�H���T�|�[�g���܂��B
// 
//�EVK_KHR_vulkan_memory_model
//Vulkan���������f�����T�|�[�g���܂��B
/////////�x���_�[�ŗL�g���@�\/////////
//�EVK_NV_ray_tracing
//NVIDIA�̃��C�E�g���[�V���O���T�|�[�g���邽�߂̊g���@�\�ł��B
// 
//�EVK_NV_mesh_shader
//���b�V���V�F�[�_�[���T�|�[�g���邽�߂̊g���@�\�ł��B
// 
//�EVK_AMD_shader_shader_ballot
//AMD��GPU�ł̃V�F�[�_�[�Ɋ֘A����@�\��񋟂��܂��B
// 
//�EVK_INTEL_performance_query
//Intel��GPU�ł̃p�t�H�[�}���X�N�G�����T�|�[�g���܂��B




