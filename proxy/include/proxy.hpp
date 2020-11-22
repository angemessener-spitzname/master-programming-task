/* Proxy object.
 * @file
 * @date 2020
 * @author Серафима Рысина
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex>
#include <tuple>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>

template <typename... mutexesTypes>
class wrapper {
    public:
        explicit wrapper(mutexesTypes&... ms) : m_mutexes(std::tie(ms...))
        { std::lock(ms...); }

        explicit wrapper(std::adopt_lock_t, mutexesTypes&... ms) noexcept
            : m_mutexes(std::tie(ms...))
        {}

        ~wrapper() {
            std::apply([](mutexesTypes&... ms) {
                (ms.unlock(), ... );
            }, m_mutexes);
        }

        wrapper(const wrapper&) = delete;
        wrapper& operator=(const wrapper&) = delete;
    private:
        std::tuple<mutexesTypes&...> m_mutexes;
};

template<typename mutex_type>
    class wrapper<mutex_type> {
    public:
        explicit wrapper(mutex_type& _m) : m_m(_m)
        { m_m.lock(); }

        explicit wrapper(std::adopt_lock_t, mutex_type& _m) noexcept
            : m_m(_m)
        {}

        ~wrapper() {
            m_m.unlock();
        }

        wrapper(const wrapper&) = delete;
        wrapper& operator=(const wrapper&) = delete;

        private:
        mutex_type&  m_m;
    };

template<class T>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}

    //{ describe proxy object
    class proxy: private wrapper<std::mutex>
    {
    public:
        proxy(T* ptr, std::mutex& mutex): wrapper<std::mutex>(mutex), ptr_(ptr) {}

        auto operator -> () const {
            return ptr_; 
        }

    private:
        T* ptr_;
    };

    auto operator -> () const
    {
        return proxy(ptr_, mutex_);
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
