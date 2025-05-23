#pragma once
#include <memory>
#include <mutex>
namespace mprpc{

template <typename T>
class Singleton{
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>&) = delete;

    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]{
            _instance.reset(new T());
        });
        return _instance;
    }

    void getThis(){
        std::cout << "getThis: " << this << std::endl;
    }

    ~Singleton() = default;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

}