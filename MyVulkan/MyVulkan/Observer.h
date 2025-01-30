#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

using ObserveFunction = std::function<void()>;

class Subject
{
private:
    std::vector<std::weak_ptr<ObserveFunction>> m_Observers;

public:
    Subject() = default;
    ~Subject() = default;

    // オブザーバーを追加（関数だけ渡す形）
    void addObserver(ObserveFunction function)
    {
        m_Observers.push_back(std::make_shared<ObserveFunction>(std::move(function)));
    }

    // オブザーバーを削除（関数オブジェクトで比較）
    void removeObserver(const ObserveFunction& function)
    {
        auto it = std::remove_if(m_Observers.begin(), m_Observers.end(),
            [&function](const std::weak_ptr<ObserveFunction>& weakObserver)
            {
                auto shared = weakObserver.lock();
                return shared && *shared == function;
            });
        m_Observers.erase(it, m_Observers.end());
    }

    // 特定のオブザーバーを持っているか確認
    bool hasObserver(const ObserveFunction& function) const
    {
        return std::any_of(m_Observers.begin(), m_Observers.end(),
            [&function](const std::weak_ptr<ObserveFunction>& weakObserver)
            {
                auto shared = weakObserver.lock();
                return shared && *shared == function;
            });
    }

    // 全オブザーバーに通知
    void notifyObservers()
    {
        for (auto it = m_Observers.begin(); it != m_Observers.end();)
        {
            auto shared = it->lock();
            if (shared) {
                (*shared)(); // 登録された関数を実行
                ++it;
            }
            else {
                it = m_Observers.erase(it); // 無効なオブザーバーを削除
            }
        }
    }
};