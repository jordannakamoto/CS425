#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <thread>
#include <mutex>

#include "LychrelData.h"


/* Number
* represents a natural number by storing its digits in a std::dequeue
* now we can reverse the number and check if its a palindrome digit-wise instead of with binary number operations */
  
// A structure recording the starting number, and its final palindrome.
//   An vector of these records (typedefed to "Records") is built during
//   program execution, and output before the program terminates.
struct Record {
    Number n;
    Number palindrome;
};
using Records = std::vector<Record>;

// Application specific constants
const size_t MaxIterations = 7500;
const size_t MaxThreads = 10;

// Mutex
std::mutex dataMutex;  // Protects access to `data`.
std::mutex recordMutex;  // Protects access to `maxRecord`.

bool process_lychrel(Number& n, size_t& iter, const size_t MaxIterations) {
    while (!n.is_palindrome() && ++iter < MaxIterations) {
        Number sum(n.size());
        Number r = n.reverse();
        auto rd = n.begin(); 
        bool carry = false;
        std::transform(n.rbegin(), n.rend(), sum.rbegin(), 
            [&](auto d) {
                auto v = d + *rd++ + carry;
                carry = v > 9;
                if (carry) { v -= 10; }     
                return v;
            }
        );
        if (carry) { sum.push_front(1); }
        n = sum;
    }
    return iter < MaxIterations;
}

void process_data(LychrelData& data, Records& records, size_t& maxIter,size_t startIdx, size_t endIdx) {
    {
    for (size_t i = startIdx; i < endIdx; ++i) {
       Number number = data[i];
        
        // Mathematical Processing
        size_t iter = 0;
        Number n = number;
        bool foundPalindrome = process_lychrel(n, iter, MaxIterations);
        
        // Record keeping
        if (foundPalindrome && iter >= maxIter) {
             std::lock_guard<std::mutex> lock(recordMutex);
            Record record{number, n};
            if (iter > maxIter) {
                records.clear();
                maxIter = iter;
            }
            records.push_back(record);
        }
    }
    }
}


//
// --- main ---
//
int main() {
    LychrelData data;  // Assume LychrelData is defined somewhere.
    std::vector<Record> records;
    size_t maxIter = 0;
    size_t dataSize = data.size();

    size_t dataPerThread = dataSize / MaxThreads;
    constexpr size_t LastID = MaxThreads - 1;

    for (size_t i = 0; i < LastID; ++i) {
        size_t startIdx = i * dataPerThread;
        size_t endIdx = startIdx + dataPerThread;
        std::thread{process_data, std::ref(data), std::ref(records), std::ref(maxIter),startIdx, endIdx}.detach();
    }

    // Handle remaining data with the main thread
    size_t startIdx = (MaxThreads - 1) * dataPerThread;
    size_t endIdx = dataSize;
    std::thread{process_data, std::ref(data), std::ref(records), std::ref(maxIter), startIdx, endIdx}.join();

    // Maximum Lycrel Iterations from input number set
    std::cout << "\nmaximum number of iterations = " << maxIter << "\n";
    // Records struct
    for (auto& [number, palindrome] : records) {
        std::cout 
            << "\t" << number 
            << " : [" << palindrome.size() << "] "
            << palindrome << "\n";
    }
}