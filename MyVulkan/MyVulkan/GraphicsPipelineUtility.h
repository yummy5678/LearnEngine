#pragma once
#include <vulkan/vulkan.hpp>
#include "GeneratorBase.h"
#include "Utilities.h"
#include "ShaderUtility.h"
#include "ViewportGenerator.h"

class PipelineGenerator : CGeneratorBase
{
public:
	PipelineGenerator();
	~PipelineGenerator();

	void Create(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);
	void Destroy(vk::Device logicalDevice);

	vk::Pipeline		GetPipeline();
	vk::PipelineLayout	GetPipelineLayout();

	vk::GraphicsPipelineCreateInfo	GetPipelineInfo();
	vk::PipelineLayoutCreateInfo	GetLayoutInfo();

private:
	vk::Pipeline					m_Pipeline;
	vk::GraphicsPipelineCreateInfo	m_PipelineCreateInfo;

	vk::PipelineLayout				m_PipelineLayout;
	vk::PipelineLayoutCreateInfo	m_PipelineLayoutCreateInfo;


	std::vector<vk::PipelineShaderStageCreateInfo>		m_ShaderStageInfos;
	vk::PipelineMultisampleStateCreateInfo				m_MultisamplingInfo;
	vk::PipelineVertexInputStateCreateInfo				m_VertexInputCreateInfo;
	vk::PipelineInputAssemblyStateCreateInfo			m_InputAssemblyInfo;
	ViewportGenerator									m_viewportGenerator;
	vk::PipelineRasterizationStateCreateInfo			m_RasterizerCreateInfo;
	std::vector<vk::PipelineColorBlendAttachmentState>	m_ColorBlendAttachment;
	vk::PipelineColorBlendStateCreateInfo				m_ColorBlendCreateInfo;

	vk::GraphicsPipelineCreateInfo CreateGraphicsPipelineInfo(vk::Device logicalDevice, vk::Extent2D extent, vk::RenderPass renderPass);


	vk::PipelineLayout CreatePipelineLayout(vk::Device logicalDevice);

	std::vector<vk::PipelineShaderStageCreateInfo>		GetShaderStageInfo(vk::Device logicalDevice);
	vk::PipelineVertexInputStateCreateInfo				GetVertexInputStateInfo();
	vk::PipelineInputAssemblyStateCreateInfo			GetInputAssemblyStateInfo();
	vk::PipelineRasterizationStateCreateInfo			GetRasterizationStateInfo();

	vk::PipelineMultisampleStateCreateInfo				GetMultisampleStateInfo();
	std::vector<vk::PipelineColorBlendAttachmentState>	GetColorBlendAttachmentState();
	vk::PipelineColorBlendStateCreateInfo				GetColorBlendStateInfo(std::vector<vk::PipelineColorBlendAttachmentState>* colorBlendAttachment);

};

// グラフィックスパイプラインについて
// 頂点情報を入力してから画像の色情報に変換するまでの
// 処理の流れを決めるオブジェクト。
// 以下の流れで情報を変換いく。
// 
// 1.頂点情報の入力(入力アセンブラ)
// 頂点のデータを読み込む。
// インデックスバッファーに格納する。
// 
// 2.頂点シェーダー
// 入力した頂点情報(インデックスバッファー)から画面上の表示位置を計算する。
// 3Dオブジェクトの移動や回転、拡大縮小などの計算はここで行う。
// 頂点シェーダーは自分で用意する必要がある。
// 
// 3.テッセレーション・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・・
// ジオメトリ(物体の形状)を細分化して、オブジェクトの表面を滑らかにしたり、肌感を作る。
// テッセレーション制御シェーダ: テッセレーションの細かさを決める。
// テッセレーションエンジン: パッチを細かく分割する。(この処理はハードウェア(GPU)内で自動的にやってくれる。)
// テッセレーション評価シェーダ(ドメインシェーダ): 新しい頂点の位置を計算する。
// この処理はオプションなので使用しないこともできる。
// 
// 4.ジオメトリシェーダ 
// プリミティブ(三角形などの計算に使いやすい基本図形)に対して追加で加工を加える。
// 頂点数を増減させて、新しい形状にしたりできる。
// 重たい処理なのであまり多用されない。
// この処理はオプションなので使用しないこともできる。
// 
// 5.ラスタライズ
// 3D空間で定義されたジオメトリ(頂点やポリゴンなど)を
// 2Dのフラグメント(ピクセルに対応するデータ)に変換する。
// この処理はハードウェア(GPU)内で自動的にやってくれる。
// 
// 6.フラグメントシェーダー(ピクセルシェーダー)
// ここでは上のラスタライズで色として出力する必要があると
// 判断された点(ピクセル)それぞれに付ける色を計算するシェーダー。
// 基本的に面の方向や材質、視点や光の位置等の情報を考慮して計算する。
// 
// 7.カラー出力 (Color Output)
// 上のフラグメントシェーダーで出した結果を
// フレームバッファに実際に書き込む。
// この処理はハードウェア(GPU)内で自動的にやってくれる。
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 

// シェーダーとは
// 
// 
// 
// 
// 
// 








