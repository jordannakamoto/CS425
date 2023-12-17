#include <vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class RingBuffer {
public:
    RingBuffer(size_t size) : buffer(size), head(0), tail(0), count(0) {}

    void add(T item) {
        std::unique_lock<std::mutex> lock(mutex);
        not_full.wait(lock, [this] { return count < buffer.size(); });

        buffer[tail] = item;
        tail = (tail + 1) % buffer.size();
        ++count;

        not_empty.notify_one();
    }

    T remove() {
        std::unique_lock<std::mutex> lock(mutex);
        not_empty.wait(lock, [this] { return count > 0; });

        T item = buffer[head];
        head = (head + 1) % buffer.size();
        --count;

        not_full.notify_one();
        return item;
    }

private:
    std::vector<T> buffer;
    size_t head, tail, count;
    std::mutex mutex;
    std::condition_variable not_empty, not_full;
};