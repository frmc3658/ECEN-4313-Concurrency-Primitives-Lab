#include "../headers/bar.h"


namespace primitive
{
    int Bar::fai(std::atomic<int>& x, int amount, std::memory_order MEM)
    {
        return x.fetch_add(amount, MEM);
    }
}