#pragma once

// �R�s�[�֎~�N���X
class NonCopyable
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;

	// �R�s�[�R���X�g���N�^�ƃR�s�[������Z�q���폜
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

};

