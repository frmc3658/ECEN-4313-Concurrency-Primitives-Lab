#ifndef BAR_H
#define BAR_H


#include <atomic>


#define SEQCST      std::memory_order_seq_cst
#define RELAXED     std::memory_order_relaxed
#define ACQUIRE     std::memory_order_acquire
#define RELEASE     std::memory_order_release


namespace primitive
{
    class Bar
    {
        protected:
            int numThreads;

        public:
            virtual ~Bar(){}

            virtual void init(int num_threads) = 0;
            virtual void wait(void) = 0;

            int fai(std::atomic<int>& x, int amount, std::memory_order MEM);
    }; 


    enum class barTypes : int
    {
        pthreadBar,
        sensebar,
        senserelbar
    };
}

#endif/* BARRIER_H */