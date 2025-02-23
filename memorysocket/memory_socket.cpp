#include <iostream>

#include "memory_socket.h"

// Original code from https://gist.github.com/sehe/9376856.
void producer()
{
    // create segment and corresponding allocator
    bip::managed_shared_memory segment(bip::open_or_create, "MySharedMemory", 65536);
    shm::char_alloc char_alloc(segment.get_segment_manager());

    // Ringbuffer fully constructed in shared memory. The element strings are
    // also allocated from the same shared memory segment. This vector can be
    // safely accessed from other processes.
    shm::ring_buffer *queue = segment.find_or_construct<shm::ring_buffer>("queue")();

    const char* messages[] = { "hello world", "the answer is 42", "where is your towel", 0 };

    for (const char** msg_it = messages; *msg_it; ++msg_it)
    {
        queue->push(shm::shared_string(*msg_it, char_alloc));
    }
}

// Original code from https://gist.github.com/sehe/9376856.
void consumer()
{
    // create segment and corresponding allocator
    bip::managed_shared_memory segment(bip::open_or_create, "MySharedMemory", 65536);
    shm::char_alloc char_alloc(segment.get_segment_manager());

    shm::ring_buffer *queue = segment.find_or_construct<shm::ring_buffer>("queue")();

    while (true)
    {
        shm::shared_string v(char_alloc);
        if (queue->pop(v))
            std::cout << "Processed: '" << v << "'\n";
    }
}
