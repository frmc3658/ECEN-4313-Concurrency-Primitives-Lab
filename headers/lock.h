#ifndef LOCK_H
#define LOCK_H

#include <atomic>

#define SEQCST      std::memory_order_seq_cst
#define RELAXED     std::memory_order_relaxed
#define ACQUIRE     std::memory_order_acquire
#define RELEASE     std::memory_order_release

namespace primitive
{
    class Lock
    {
        protected:
            std::atomic<bool> flag;

        public:
            virtual ~Lock(){}

            bool tas(std::atomic<bool>& x, std::memory_order MEM);
            int fai(std::atomic<int>& x, int amount, std::memory_order MEM);
            

            // TODO: Figure out why the cas() function doesn't link
            template <typename T>
            bool cas(std::atomic<T>& x, T expected, T desired, std::memory_order MEM);

            template <typename T>
            T vcas(std::atomic<T>& x, T expected, T desired, std::memory_order MEM);

            // Pure Virtual Functions provide a
            // consistent interface to all classes
            // of locks
            virtual void lock(void) = 0;
            virtual void unlock(void) = 0;
    };


    enum class lockTypes : int
    {
        mcslock,
        pthreadlock,
        taslock,
        ttaslock,
        ticketlock
    };
}


#endif  /* LOCK_H */