# Results for Assignment 01

## Improvement Iterations

Here's a table showing the improvements I did to make the application go faster. All I did was add compiler flags.  The **Time** column in the table represents the _wall-clock time_ for a program run.

| Version | Time | Speedup | Memory (KB) | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| [01](01.cpp) | 10.52s | &mdash; | 1042844 | Initial version - no changes |
| 02 | 2.31 | 4.56x | 1041916 | Compiled with -Ofast and -funroll-all-loops |
