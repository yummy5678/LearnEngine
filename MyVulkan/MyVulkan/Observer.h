#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

// オブザーバーインターフェース
template <typename T>
class Observer 
{
public:
    virtual ~Observer() {}
    virtual void update(const T& state) = 0; // 状態が変わったときに呼ばれる
};

// 被観察者インターフェース
template <typename T>
class Subject 
{
public:
    virtual ~Subject() {}
    virtual void attach(std::shared_ptr<Observer<T>> observer) = 0; // 観察者を登録
    virtual void detach(std::shared_ptr<Observer<T>> observer) = 0; // 観察者を解除
    virtual void notify() = 0; // 状態が変わったときに通知
};