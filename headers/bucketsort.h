#ifndef BUCKETSORT_H
#define BUCKETSORT_H


#include <set>
#include <vector>

#include "lock.h"
#include "bar.h"

typedef std::pair<int, int> Range;

class BucketSort
{
    private:
        // Concurrency variables
        int m_numThreads;

        // Concurrency primitives
        primitive::Lock* m_lk;
        primitive::Bar* m_bar;

        // Sorting variables
        std::vector<int>* m_unsortedData;
        std::set<int>* m_sortedData;

        Range getSortRange(int threadID);

    public:
        BucketSort();
        ~BucketSort();

        void init(std::vector<int>* unsortedData, int numThreads,
                   primitive::Lock* lk, primitive::Bar* bar);
        void sort(int threadID);
};


#endif  /* BUCKETSORT_H */
