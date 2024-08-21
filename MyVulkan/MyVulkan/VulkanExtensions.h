#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>


//デバイスで使用する拡張機能
//const std::vector<const char*> deviceExtensions = {
//	VK_KHR_SWAPCHAIN_EXTENSION_NAME
//};


//　拡張機能には、インスタンス拡張機能とデバイス拡張機能の2つのグループがあります。
// 簡単に言えば、インスタンス拡張機能 は VkInstance 全体に結び付けられ、
// デバイス拡張機能は単一の VkDevice オブジェクトにのみ結び付けられます。

/*//////////////////////////////////////////
* インスタンスの拡張機能一覧(ChatGPTにて作成)
*///////////////////////////////////////////
/////////標準拡張機能/////////
//・VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
//Androidプラットフォーム上でのサーフェス作成をサポートします。
// 
//・VK_KHR_DISPLAY_EXTENSION_NAME
//直接ディスプレイに描画するための機能を提供します。
// 
//・VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
//ディスプレイ用スワップチェーンをサポートします。
// 
//・VK_KHR_EXTERNAL_FENCE_EXTENSION_NAME
//外部フェンスとの相互運用性をサポートします。
// 
//・VK_KHR_EXTERNAL_FENCE_FD_EXTENSION_NAME
//Linuxプラットフォームでの外部フェンスファイルディスクリプタとの相互運用性をサポートします。
// 
//・VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME
//外部メモリとの相互運用性をサポートします。
// 
//・VK_KHR_EXTERNAL_MEMORY_FD_EXTENSION_NAME
//Linuxプラットフォームでの外部メモリファイルディスクリプタとの相互運用性をサポートします。
// 
//VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
//デバイスのプロパティを取得するための拡張機能です。
// 
//・VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME
//サーフェスの機能を取得するための拡張機能です。
// 
//・VK_KHR_SURFACE_EXTENSION_NAME
//Vulkanサーフェスとのインターフェースを提供します。
// 
//・VK_KHR_SWAPCHAIN_EXTENSION_NAME
//スワップチェーンをサポートします。
// 
//・VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
//Waylandサーフェスとのインターフェースを提供します（Linuxプラットフォーム用）。
// 
//・VK_KHR_WIN32_SURFACE_EXTENSION_NAME
//Windowsプラットフォーム上でのサーフェス作成をサポートします。
// 
//・VK_KHR_XCB_SURFACE_EXTENSION_NAME
//XCBサーフェスとのインターフェースを提供します（Linuxプラットフォーム用）。
// 
//・VK_KHR_XLIB_SURFACE_EXTENSION_NAME
//Xlibサーフェスとのインターフェースを提供します（Linuxプラットフォーム用）。
// 
//・VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
//パフォーマンスクエリをサポートします。
// 
//・VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME
//サンプラーのYCbCr変換をサポートします。
// 
//・VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
//プッシュディスクリプタのサポートを提供します。
// 
//・VK_KHR_VULKAN_MEMORY_MODEL_EXTENSION_NAME
//Vulkanメモリモデルをサポートします。
// 
//・VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
//Vulkanポータビリティサブセットをサポートします。
// 
//・VK_KHR_PRESENT_WAIT_EXTENSION_NAME
//プレゼント待機をサポートします。

/////////ベンダー固有拡張機能/////////
//・VK_EXT_DEBUG_UTILS_EXTENSION_NAME
//デバッグとトレースのための拡張機能です。
// 
//・VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
//シェーダーコンパイルのキャッシュを提供します。
// 
//・VK_NV_EXTERNAL_MEMORY_FD_EXTENSION_NAME
//NVIDIA製GPUでの外部メモリファイルディスクリプタとの相互運用性をサポートします。
// 
//・VK_NV_RAY_TRACING_EXTENSION_NAME
//NVIDIA製GPUでのレイ・トレーシングをサポートします。
// 
//・VK_AMD_BUFFER_MARKER_EXTENSION_NAME
//AMD製GPUでのバッファマーカー機能を提供します。
// 
//・VK_EXT_SWAPCHAIN_COLORSPACE_EXTENSION_NAME
//スワップチェーンの色空間変換をサポートします。




/*//////////////////////////////////////////
* デバイスの拡張機能一覧(ChatGPTにて作成)
*///////////////////////////////////////////
/////////標準拡張機能/////////
//・VK_KHR_swapchain
//スワップチェーンをサポートするための機能を提供します。
// 
//・VK_KHR_surface
//表面（ウィンドウ）とのインターフェースを提供します。
// 
//・VK_KHR_surface_protected
//保護されたサーフェスをサポートします。
// 
//・VK_KHR_display
//ディスプレイ（外部モニター）とのインターフェースを提供します。
// 
//・VK_KHR_device_group
//デバイスグループに関連する機能を提供します。
// 
//・VK_KHR_get_physical_device_properties2
//デバイスのプロパティを取得するための拡張機能です。
// 
//・VK_KHR_multiview
//マルチビュー（複数のビューを持つレンダリング）をサポートします。
// 
//・VK_KHR_push_descriptor
//プッシュディスクリプタ（シェーダーに動的にデータを提供する）をサポートします。
// 
//・VK_KHR_shader_float16_int8
//16ビット浮動小数点および8ビット整数型のシェーダーをサポートします。
// 
//・VK_KHR_sampler_ycbcr_conversion
//サンプラーのYCbCr変換をサポートします。
// 
//・VK_KHR_timeline_semaphore
//タイムラインセマフォをサポートします。
// 
//・VK_KHR_vulkan_memory_model
//Vulkanメモリモデルをサポートします。
/////////ベンダー固有拡張機能/////////
//・VK_NV_ray_tracing
//NVIDIAのレイ・トレーシングをサポートするための拡張機能です。
// 
//・VK_NV_mesh_shader
//メッシュシェーダーをサポートするための拡張機能です。
// 
//・VK_AMD_shader_shader_ballot
//AMD製GPUでのシェーダーに関連する機能を提供します。
// 
//・VK_INTEL_performance_query
//Intel製GPUでのパフォーマンスクエリをサポートします。




