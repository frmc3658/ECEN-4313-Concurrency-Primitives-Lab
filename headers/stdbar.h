#ifndef STDBAR_H
#define STDBAR_H


#include <barrier>
#include "bar.h"


namespace primitive
{
    class StdBar : public  Bar
    {
        private:
            std::barrier<>* bar;
        
        public:
            StdBar();
            StdBar(int numThreads);
            ~StdBar();

            void init(int numThreads) override;
            void wait(void) override;
    };
}

#endif  /* STDBAR_H */