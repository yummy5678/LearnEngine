#pragma once

// �R�s�[�֎~�N���X
class NonCopyable
{
public:
	NonCopyable() = default;
	~NonCopyable() = default;

	// �R�s�[�R���X�g���N�^�ƃR�s�[������Z�q���폜
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

	// ���[�u�͋���
	NonCopyable(NonCopyable&&) noexcept = default;
	NonCopyable& operator=(NonCopyable&&) noexcept = default;

};

