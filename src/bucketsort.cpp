#include "../headers/bucketsort.h"

BucketSort::BucketSort()
{

}


BucketSort::~BucketSort()
{
    delete m_sortedData;
}


void BucketSort::init(std::vector<int>* unsortedData, int numThreads,
                   primitive::Lock* lk, primitive::Bar* bar)
{
    m_unsortedData = unsortedData;
    m_sortedData = nullptr;
    m_numThreads = numThreads;
    m_lk = lk;
    m_bar = bar;
}


Range BucketSort::getSortRange(int threadID)
{
    // Calculate number of elements to assign to range
    int numValues = m_unsortedData->size();
    int numElements = numValues / m_numThreads;
    int extraElements = numValues % m_numThreads;

    // Calculate starting point of range (inclusive)
    int startIndex = threadID * numElements + std::min(threadID, extraElements);

    // Calculate endpoint of range (inclusive)
    int endIndex = startIndex + numElements + (threadID < extraElements ? 1 : 0) - 1;

    // Encapsulate both points as a range
    Range sortRange = std::make_pair(startIndex, endIndex);

    // printf("Thread %d: sortRange =  [%d : %d]\n", threadID, startIndex, endIndex);

    return sortRange;
}


void BucketSort::sort(int threadID)
{
    // Distribute ranges for threads to work on
    Range sortRange = getSortRange(threadID);

    // Create a thread-safe range to perform sorting on
    std::vector<int> unsortedRange(m_unsortedData->begin() + sortRange.first, m_unsortedData->begin() + sortRange.second + 1);

    if(unsortedRange.empty() || m_numThreads < 1)
    {
        // printf("Thread %d: Short circuit return\n", threadID);
        return;
    }

    int numBuckets = (unsortedRange.size() / 10) + (unsortedRange.size() % 10 ? 1 : 0);
    // printf("Thread %d: numBuckets = %d\n", threadID, numBuckets);

    // Vector of (ordered) sets
    std::vector<std::set<int>> buckets(numBuckets);

    for(size_t i = 0; i < unsortedRange.size(); i++)
    {
        // Get next element
        int element = unsortedRange[i];

        // Determine the bucket index for the element based on its value
        int bucketIndex = (element * m_numThreads) / INT_MAX;

        // Insert it into the corresponding bucket
        m_lk->lock();
        buckets.at(bucketIndex).insert(element);
        m_lk->unlock();
    }

    // Wait for all threads to finish inserting elements
    m_bar->wait();

    // Each thread stores its sorted bucket in the original unsortedData vector
    size_t index = sortRange.first;
    for(size_t i = 0; i < buckets.size(); i++)
    {
        m_lk->lock();
        std::set<int>& currentBucket = buckets[i];
        
        for (int element : currentBucket)
        {
            (*m_unsortedData)[index] = element;
            index++;
        }

        m_lk->unlock();
    }

    m_bar->wait();

    // Have the main thread cheese the merge
    if(threadID == 0)
    {
        m_sortedData = new std::set<int>(m_unsortedData->begin(), m_unsortedData->end());

        m_unsortedData->clear();

        for(auto& it : *m_sortedData)
        {
            m_unsortedData->push_back(it);
        }
    }
}
