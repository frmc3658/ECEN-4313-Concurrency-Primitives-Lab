#include "../headers/lock.h"


namespace primitive
{
    bool Lock::tas(std::atomic<bool>& x, std::memory_order MEM)
    {
        bool expected = false;
        bool desired = true;

        return x.compare_exchange_weak(expected, desired, MEM);
    }


    int Lock::fai(std::atomic<int>& x, int amount, std::memory_order MEM)
    {
        return x.fetch_add(amount, MEM);
    }


    template <typename T>
    bool Lock::cas(std::atomic<T>& x, T expected, T desired, std::memory_order MEM)
    {
        return x.compare_exchange_strong(expected, desired, MEM);
    }


    template <typename T>
    T Lock::vcas(std::atomic<T> & x, T expected, T desired, std::memory_order MEM)
    {
        x.compare_exchange_strong(expected, desired, MEM);
        return expected;
    }


}