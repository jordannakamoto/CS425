# Results for Assignment 01

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Memory (KB) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [01](01.cpp) | 10.52s | &mdash; | 1042844 | Initial version - no changes |
| 02 | 2.31 | 4.56x | 1041916 | Compiled with -Ofast and -funroll-all-loops |

## Profiling Analysis

### Initial Review

# Professor's Notes
Looking at [01's profile](01.prof), the hottest function was `Transform::float4::perspectiveDivide() const`, which consumed around 28% of the program's execution time.  There's not a lot in that function, but it does three floating-point divisions, so perhaps that's something to try optimizing.