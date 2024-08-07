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
	std::string m_ClassName; // �N���X����ێ�����ϐ�
	bool m_bCreated;

	//�p�����Create�֐����g�p���ꂽ���m�F����N���X
	std::string GetGeneratorName();
	void CheckCreated();
	bool isCreated();
};

