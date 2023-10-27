#ifndef STDBAR_H
#define STDBAR_H


#include <barrier>
#include "bar.h"


namespace primitive
{
    class StdBar : public  Bar
    {
        private:
            std::barrier<>* m_bar;
        
        public:
            StdBar();
            StdBar(int numThreads);
            virtual ~StdBar();

            void init(int numThreads) override;
            void wait(void) override;
    };
}

#endif  /* STDBAR_H */