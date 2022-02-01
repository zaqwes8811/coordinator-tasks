#include "app/config.h"

#include <iostream>
#include <vector>
#include <gtest/gtest.h>

#ifdef TBB_THREADING_FRAMEWORK
#include <tbb/parallel_for.h>
#include <tbb/atomic.h>
#endif

/** 
 * To be used with tbb::parallel_for, this increments count
 * if the value at the supplied index is zero.
 */
class ZeroCounter
{
public:
    ZeroCounter(std::vector<int> * vec, tbb::atomic<int> * count) :
        vec(vec),
        count(count)
    { } 

    void operator()(int index) const
    {
        if((*vec)[index] == 0)
            ++(*count);
    }

    std::vector<int> * vec;
    tbb::atomic<int> * count;
};

TEST(Parallel, TBB) {
    // Create a vector and fill it with sample values
    std::vector<int> a;
    a.push_back(0);
    a.push_back(3);
    a.push_back(0);

    // Counter to track the number of zeroes
    tbb::atomic<int> count;
    count = 0;

    // Create our function object and execute the parallel_for
    ZeroCounter counter(&a, &count);
    tbb::parallel_for(size_t(0), a.size(), counter);

    std::cout << count << std::endl;
}

TEST(TBBBegin, TBB) {
    // Create a vector and fill it with sample values
    std::vector<int> a;
    a.push_back(0);
    a.push_back(3);
    a.push_back(0);

    // Counter to track the number of zeroes
    tbb::atomic<int> count;
    count = 0;

    // Create our function object and execute the parallel_for
    ZeroCounter counter(&a, &count);
    tbb::parallel_for(size_t(0), a.size(), counter);

    std::cout << count << std::endl;
}