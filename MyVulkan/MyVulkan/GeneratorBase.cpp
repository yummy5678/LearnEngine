#include "GeneratorBase.h"


GeneratorBase::GeneratorBase():
	m_ClassName("UnNamedGenerator"),	//継承先のクラスで名前を初期化し忘れていたとき用の名前を付けておく
	m_bCreated(false)
{
}

GeneratorBase::~GeneratorBase()
{
}

std::string GeneratorBase::GetGeneratorName()
{
	return m_ClassName;
}

void GeneratorBase::CheckCreated()
{
	//Generatorクラスのメンバー変数を作成前に受け取ろうとした場合エラーを出したい
	if(!m_bCreated) throw std::runtime_error(m_ClassName +"クラスの値を取得する前にCreate関数を使用してください！");
}

bool GeneratorBase::isCreated()
{
	return m_bCreated;
}
