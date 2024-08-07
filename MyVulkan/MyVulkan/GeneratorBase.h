#pragma once
#include <string>
#include <stdexcept>
#include <iostream>

class CGeneratorBase
{
public:
	CGeneratorBase();
	~CGeneratorBase();


protected:
	std::string m_ClassName; // クラス名を保持する変数
	bool m_bCreated;

	//継承先のCreate関数が使用されたか確認するクラス
	std::string GetGeneratorName();
	void CheckCreated();
	bool isCreated();
};

