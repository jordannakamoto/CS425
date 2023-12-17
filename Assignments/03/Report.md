# Results for Assignment 03
My solution was just to create more sockets in threads.
I think the client is sending messages to the server as it requests resources... so I'm wondering if there's anyway to buffer those messages.
Maybe if we can wrap the requests in asynchronous code we can have the client make all of it's requests at once and then request threads from a pool.
Or perhaps there's a speedup from introducing dedicated threads for HTTP requests and responses. I'm not sure, perhaps bandwidth is the limiting factor.
I do wish I had time to try implementing the ring buffer.

## Improvement Iterations

| Version | Time | Speedup | Changes |
| :-----: | ---- | :-----: | :------: | ------- |
| base| 6.1s | &mdash; |  | Initial version - no changes |
| 02 | 2.4 | 2.4x | Compiled with -Ofast and added threading |
