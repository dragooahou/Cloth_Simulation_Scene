//
// Created by etien on 22/02/2021.
//

#ifndef CLOTHSIM_THREADSAFERINGBUFFER_H
#define CLOTHSIM_THREADSAFERINGBUFFER_H

// Fixed size very simple thread safe ring buffer
template <typename T, size_t capacity>
class ThreadSafeRingBuffer {
public:
    // Push an item to the end if there is free space
    //  Returns true if succesful
    //  Returns false if there is not enough space
    inline bool push_back(const T& item)
    {
        bool result = false;
        lock.lock();
        size_t next = (head + 1) % capacity;
        if (next != tail)
        {
            data[head] = item;
            head = next;
            result = true;
        }
        lock.unlock();
        return result;
    }

    // Get an item if there are any
    //  Returns true if succesful
    //  Returns false if there are no items
    inline bool pop_front(T& item)
    {
        bool result = false;
        lock.lock();
        if (tail != head)
        {
            item = data[tail];
            tail = (tail + 1) % capacity;
            result = true;
        }
        lock.unlock();
        return result;
    }

    inline bool empty() {
        return head == tail;
    }

    inline void clear() {
        lock.lock();
        head = 0;
        tail = 0;
        lock.unlock();
    }

private:
    T data[capacity];
    size_t head = 0;
    size_t tail = 0;
    std::mutex lock; // this just works better than a spinlock here (on windows)
};

#endif //CLOTHSIM_THREADSAFERINGBUFFER_H
