#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Singleton {
public:
    // �C���X�^���X�擾
    static T& getInstance() {
        std::call_once(initFlag, []() {
            instance.reset(new T());
            });
        return *instance;
    }

    // �R�s�[�R���X�g���N�^���폜
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    // �R���X�g���N�^��protected�ɂ��āA�O������̃C���X�^���X�������֎~
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    static std::unique_ptr<T> instance;  // �B��̃C���X�^���X
    static std::once_flag initFlag;      // �C���X�^���X�����̃t���O
};

// �ÓI�����o�ϐ��̏�����
template <typename T>
std::unique_ptr<T> Singleton<T>::instance = nullptr;

template <typename T>
std::once_flag Singleton<T>::initFlag;


