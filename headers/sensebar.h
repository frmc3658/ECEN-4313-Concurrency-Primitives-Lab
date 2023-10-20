#ifndef SENSEBAR_H
#define SENSEBAR_H


#include "bar.h"


namespace primitive
{
    class SenseBar : public Bar
    {
        private:
            std::atomic<int> count;
            std::atomic<int> sense;

        public:
            SenseBar();
            ~SenseBar();

            void init(int num_threads) override;
            void wait(void) override;
    };
}


#endif  /* SENSEBAR_H */