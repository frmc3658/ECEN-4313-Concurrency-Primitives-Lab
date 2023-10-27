#include "../headers/bucketsort.h"

BucketSort::BucketSort()
{

}


BucketSort::~BucketSort()
{

}


// Range BucketSort::getSortRange(int threadID)
// {
//     // Calculate number of elements to assign to range
//     int numValues = sortedValues->size();
//     int numElements = numValues / (numThreads + 1);

//     // Calculate starting point of range (inclusive)
//     int startIndex = threadID * numElements;
    
//     // Calculate endpoint of range (inclusive)
//     int endIndex;
//     if(threadID == MAIN_THREAD){ endIndex = numValues - 1; }
//     else{ endIndex = ((threadID + 1) * numElements) - 1; }
    

//     // Encapsulate both points as a range
//     Range sortRange = std::make_pair(startIndex, endIndex);

//     return sortRange;
// }


void BucketSort::sort(std::vector<int>*& inputArray, std::vector<primitive::Lock*> locks, std::vector<std::vector<int>>& buckets, int numBuckets)
{
    // if (inputArray->empty() || numBuckets <= 0)
    // {
    //     return;
    // }

    // // Vector of (ordered) sets
    // std::vector<std::set<int>> buckets(numBuckets);
    // std::vector<std::set<int>> mins(numBuckets + 1);
    // mins[0].insert(0);
    // mins[0].insert(INT_MAX);

    // for (size_t i = 0; i < inputArray->size(); i++)
    // {
    //     int element = (*inputArray)[i];

    //     // Find the bucket index for the element
    //     size_t j = 0;
    //     while(j < static_cast<size_t>(numBuckets) && element >= *mins[j].begin())
    //     {
    //         j++;
    //     }

    //     // If element is less than the minimum value in the j-th bucket
    //     if (j > 0)
    //     {
    //         // Insert it into buckets[j-1]
    //         buckets[j - 1].insert(element);
    //     }
    // }

    if (inputArray->empty() || numBuckets <= 0)
    {
        return;
    }

    std::vector<int> mins(numBuckets);
    mins[0] = 0;
    mins[numBuckets] = INT_MAX;

    int min = *std::min_element(inputArray->begin(), inputArray->end());
    int max = *std::min_element(inputArray->begin(), inputArray->end());
    int stride = (max - min) / numBuckets;

    for(int i = 0; i < numBuckets; i++)
    {
        mins[i] = min + (i * stride);
    }

    for(size_t i = 0 ; i < inputArray->size(); i++)
    {
        for(int j = 0; j < numBuckets; j++)
        {
            if((*inputArray)[i] >= mins[j])
            {
                for(size_t q = 0; q < locks.size(); q++)
                {
                    locks[q]->lock();
                }
                
                buckets[j].push_back((*inputArray)[i]);

                for(size_t q = 0; q < locks.size(); q++)
                {
                    locks[q]->unlock();
                }

                break;
            }
        }
    }
}


