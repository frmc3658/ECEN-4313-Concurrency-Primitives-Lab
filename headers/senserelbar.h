#ifndef SENSERELBAR_H
#define SENSERELBAR_H


#include "bar.h"


namespace primitive
{
    class SenseRelBar : public Bar
    {
        private:
            std::atomic<int> count;
            std::atomic<int> sense;

        public:
            SenseRelBar();
            ~SenseRelBar();

            void init(int numThreads) override;
            void wait(void) override;
    };
}


#endif  /* SENSERELBAR_H */