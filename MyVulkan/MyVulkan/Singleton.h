#pragma once
#include <memory>
#include <mutex>

template <typename T>
class Singleton {
public:
    // インスタンス取得
    static T& getInstance() {
        std::call_once(initFlag, []() {
            instance.reset(new T());
            });
        return *instance;
    }

    // コピーコンストラクタを削除
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    // コンストラクタをprotectedにして、外部からのインスタンス生成を禁止
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    static std::unique_ptr<T> instance;  // 唯一のインスタンス
    static std::once_flag initFlag;      // インスタンス生成のフラグ
};

// 静的メンバ変数の初期化
template <typename T>
std::unique_ptr<T> Singleton<T>::instance = nullptr;

template <typename T>
std::once_flag Singleton<T>::initFlag;


