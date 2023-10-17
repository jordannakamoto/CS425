#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <thread>
#include <mutex>

#include "LychrelData.h"

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

/* Number.h class
   * represents a natural number by storing its digits in a std::dequeue
   * now we can reverse the number and check if its a palindrome digit-wise instead of with binary number operations */


//
// --- main ---
//
int main() {
    LychrelData data; // Data is an mmapped set of 2 arrays that exposes the getNext, thread safe process incrementor to [n+1][n+1]

    std::cerr << "Processing " << data.size() << " values ...\n";

    // Output Params
    size_t maxIter = 0;
    Records records;

    for (auto i = 0; i < data.size(); ++i) { // for all numbers in our dataset

        // Insert threading <----
        
        //  ----------- Lycrel Process -----------------------------------------------------
        // This summing code works like long addition because of our Number class
        Number number = data[i];
        size_t iter = 0; Number n = number;                         // iterate until a palindrome is found
        while (!n.is_palindrome() && ++iter < MaxIterations) {      // <-
            Number sum(n.size());
            Number r = n.reverse();
            auto rd = n.begin(); 
            bool carry = false;
            std::transform(n.rbegin(), n.rend(), sum.rbegin(), 
                [&](auto d) {
                    auto v = d + *rd++ + carry; // add the digit to the reverse digit and add a carry if there is one
                    carry = v > 9;              // see if theres a carry for the next step
                    if (carry) { v -= 10; }     
                    return v;                   // v is written to sum starting at sum.rbegin()
                }
            );
            if (carry) { sum.push_front(1); }
            n = sum;
        }
        // ----------------------------------------------------------------

        // ------------- Tracking our MaxIterations ---------------------------------------------------
        // Reports maxIterations
        // ? maybe identify some set threshold of iterations to label the upper set of problem dimension over operation no.s
        if (iter < maxIter || iter == MaxIterations) { continue; }

        // store the max records
        Record record{number, n};
        if (iter > maxIter) {
            records.clear();
            maxIter = iter;
        }
        records.push_back(record);
        // ----------------------------------------------------------------
    }


    // END PROCESSING: Print Result output

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