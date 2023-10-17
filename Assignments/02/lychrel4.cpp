#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <thread>
#include <mutex>

#include "LychrelData.h"


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

void process_data(LychrelData& data, Records& records, size_t& maxIter) {
    while (true) {
        dataMutex.lock();
        
        std::vector<Number> numbers;
        if (!data.getNext(4, numbers)) {
            dataMutex.unlock();
            break;
        }
        dataMutex.unlock();

        // Process each number in the group
        for (auto& number : numbers) {
            // Mathematical Processing
            size_t iter = 0;
            Number n = number;
            bool foundPalindrome = process_lychrel(n, iter, MaxIterations);

            // Record keeping
            if (foundPalindrome && iter >= maxIter) {
                recordMutex.lock();

                Record record{number, n};
                if (iter > maxIter) {
                    records.clear();
                    maxIter = iter;
                }
                records.push_back(record);
                recordMutex.unlock();
            }
        }
    }
}




//
// --- main ---
//
int main() {
    LychrelData data;
    std::vector<Record> records;
    size_t maxIter = 0;

    std::vector<std::thread> threads;
    for (size_t i = 0; i < MaxThreads; ++i) {
        threads.push_back(std::thread(process_data, std::ref(data), std::ref(records), std::ref(maxIter)));
    }
    // Join threads to wait for them to finish.
    for (auto& t : threads) {
        t.join();
    }

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