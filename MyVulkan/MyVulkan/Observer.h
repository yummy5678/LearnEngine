#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

// �I�u�U�[�o�[�C���^�[�t�F�[�X
template <typename T>
class Observer 
{
public:
    virtual ~Observer() {}
    virtual void update(const T& state) = 0; // ��Ԃ��ς�����Ƃ��ɌĂ΂��
};

// ��ώ@�҃C���^�[�t�F�[�X
template <typename T>
class Subject 
{
public:
    virtual ~Subject() {}
    virtual void attach(std::shared_ptr<Observer<T>> observer) = 0; // �ώ@�҂�o�^
    virtual void detach(std::shared_ptr<Observer<T>> observer) = 0; // �ώ@�҂�����
    virtual void notify() = 0; // ��Ԃ��ς�����Ƃ��ɒʒm
};