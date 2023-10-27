#ifndef BUCKETSORT_H
#define BUCKETSORT_H


#include <set>
#include <vector>

#include "lock.h"

typedef std::pair<int, int> Range;

extern int MAIN_THREAD;
extern std::vector<int>* sortedValues;
extern int numThreads;

class BucketSort
{
    private:
        std::vector<int> pool;


    public:
        BucketSort();
        ~BucketSort();

        // Range getSortRange(int threadID);
        void sort(std::vector<int>*& inputArray, std::vector<primitive::Lock*> locks, std::vector<std::vector<int>>& buckets, int numBuckets);
};


#endif  /* BUCKETSORT_H */