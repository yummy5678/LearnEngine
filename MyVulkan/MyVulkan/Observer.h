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

    // �I�u�U�[�o�[��ǉ��i�֐������n���`�j
    void addObserver(ObserveFunction function)
    {
        m_Observers.push_back(std::make_shared<ObserveFunction>(std::move(function)));
    }

    // �I�u�U�[�o�[���폜�i�֐��I�u�W�F�N�g�Ŕ�r�j
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

    // ����̃I�u�U�[�o�[�������Ă��邩�m�F
    bool hasObserver(const ObserveFunction& function) const
    {
        return std::any_of(m_Observers.begin(), m_Observers.end(),
            [&function](const std::weak_ptr<ObserveFunction>& weakObserver)
            {
                auto shared = weakObserver.lock();
                return shared && *shared == function;
            });
    }

    // �S�I�u�U�[�o�[�ɒʒm
    void notifyObservers()
    {
        for (auto it = m_Observers.begin(); it != m_Observers.end();)
        {
            auto shared = it->lock();
            if (shared) {
                (*shared)(); // �o�^���ꂽ�֐������s
                ++it;
            }
            else {
                it = m_Observers.erase(it); // �����ȃI�u�U�[�o�[���폜
            }
        }
    }
};