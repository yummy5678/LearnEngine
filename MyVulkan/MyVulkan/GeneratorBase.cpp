#include "GeneratorBase.h"


CGeneratorBase::CGeneratorBase():
	m_ClassName("UnNamedGenerator"),	//�p����̃N���X�Ŗ��O�����������Y��Ă����Ƃ��p�̖��O��t���Ă���
	m_bCreated(false)
{
}

CGeneratorBase::~CGeneratorBase()
{
}

std::string CGeneratorBase::GetGeneratorName()
{
	return m_ClassName;
}

void CGeneratorBase::CheckCreated()
{
	//Generator�N���X�̃����o�[�ϐ����쐬�O�Ɏ󂯎�낤�Ƃ����ꍇ�G���[���o������
	if(!m_bCreated) throw std::runtime_error(m_ClassName +"�N���X�̒l���擾����O��Create�֐����g�p���Ă��������I");
}

bool CGeneratorBase::isCreated()
{
	return m_bCreated;
}
