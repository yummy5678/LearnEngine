#pragma once
#include <string>
#include <stdexcept>
#include <iostream>

class GeneratorBase
{
public:
	GeneratorBase();
	~GeneratorBase();


protected:
	std::string m_ClassName; // �N���X����ێ�����ϐ�
	bool m_bCreated;

	//�p�����Create�֐����g�p���ꂽ���m�F����N���X
	std::string GetGeneratorName();
	void CheckCreated();
	bool isCreated();
};

