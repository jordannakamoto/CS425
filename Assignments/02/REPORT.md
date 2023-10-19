# Report for Assignment 02

## Improvement Iterations

Apparently the -O3 compiler instruction was supposed to be included in the makefile... so I missed that part.
I ran a trial of the data source from Final and it took around 43 minutes before I gave up on it - unfortunately I did not have time to complete the entirety of the project using the Final data, but I did analyze my fastest programs on it.

Running program versions on the smaller Test dataset...

|              Version              | Time                                           | Speedup | Changes                                    |
|:---------------------------------:|------------------------------------------------|:-------:|--------------------------------------------|
|             01 - Base             | 60.48s user 0.18s system 99% cpu 1:00.85 total | &mdash; | Initial version - no changes               |
|           02 - threaded           | 81.85s user 0.34s system 683% cpu 12.021 total |  5.06x  | Added threading to data processing loop    |
|           03 - dynamic            | 87.40s user 0.49s system 725% cpu 12.111 total |  5.02x  | Added dynamic scheduling to threading load |
|        04 - getNext(n,...)        | 85.02s user 0.40s system 663% cpu 12.872 total |  4.72x  | Tested dynamic scheduling with chunks of 4 |
| 05 - threaded + -O3 compiler flag | 3.39s user 0.02s system 684% cpu 0.498 total   | 122.18x | v02 compiled with flag -O3                 |
| 06 - dynamic + -O3 compiler flag  | 3.69s user 0.02s system 711% cpu 0.522 total   | 116.57x | v03 compiled with flag -O3                 |

## Conclusion 1
* It seems that on the smaller data set, the basic threading with the .detach() function performs the best. Perhaps this is due to less overhead and locking/unlocking.
  
Final dataset...
|              Version              | Time                                           | Changes                                    |
|:---------------------------------:|------------------------------------------------|--------------------------------------------|
|             01 - Base             | > 43 minutes total                             | Initial version - no changes               |
| 05 - threaded + -O3 compiler flag | 905.14s user 5.77s system 662% cpu 2:17.42 total | v02 compiled with flag -O3               |
| 06 - dynamic + -O3 compiler flag  | 817.47s user 5.43s system 676% cpu 2:01.66 total | v03 compiled with flag -O3               |
| 07 - dynamic + -O3 compiler flag  | 830.47s user 5.09s system 678% cpu 2:03.19 total | v04 compiled with flag -O3               |


## Conclusion 2

* On the larger dataset, the dynamic scheduling version runs the fastest!
* I think maybe because I'm using a vector to store chunks of 4 after the datafile is unlocked... I'm actually adding data padding which would negate the benefit of grouped fetching. I would have to form a hypothesis on how the data is shaped to proceed with this strategy.

## i. Problem Solving Intuition
1. Break the code into parts so I understand what's moving
2. Find the for-loop that is going over the indexed set of LychrelData
3. Review provided classes - they look already good to go and designed for parallelism and speed, even says thread safety is ready in LychrelData.h
4. Isolate mathematical processing code block out of data processing
5. Import threads and > mutexes for asynchronous data access and records writing
6. Chunk data into equal batches for threads to work on
7. Report out
8. Implement dynamic scheduling using getNext()
9. Report out
10. Implement dynamic scheduling with grouping to improve data-locality(caching) and reduce mutex locking/unlocking overhead

## ii. Insight
* Maybe having separate threads to handle different parts of the program might add some benefit...
* We be able to create some maximum iteration count where we can postpone operation on complex number patterns until the last cycle and thread the remaining sums/palindrome checking?
*  I didn't give myself enough time to thoroughly finish all parts of the project
