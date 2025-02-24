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
    shm::ring_buffer *queue = nullptr;

    try {
        queue = segment.find_or_construct<shm::ring_buffer>("queue")();
    } catch (boost::interprocess::lock_exception& _) {
        std::cerr << "mem lock exception" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "unknown exception: " << ex.what() << std::endl;
    }

    std::cout << "free memory: " << segment.get_free_memory() << std::endl;
    std::cout << "memory size: " << segment.get_size() << std::endl;

    if (queue == nullptr) {
        return;
    }

    std::cout << "write available: " << queue->write_available() << std::endl;

    const char* messages[] = { "hello world", "the answer is 42", "where is your towel", 0 };
    auto counter = 0;

    while (true)
    {
        if ((counter % 1000000) == 0 && counter > 0)
        {
            std::cout << "produced " << counter / 1000000 << "M messages" << std::endl;
        }

        for (const char** msg_it = messages; *msg_it; ++msg_it)
        {
            if (!queue->push(shm::shared_string(*msg_it, char_alloc))) {
                // std::cout << "free memory: " << segment.get_free_memory() << std::endl;
                // std::cout << "memory size: " << segment.get_size() << std::endl;
                // std::cout << "read available: " << queue->read_available() << std::endl;
                // std::cout << "queue full" << std::endl;
                break;
            }
            counter++;
        }
    }
}

// Original code from https://gist.github.com/sehe/9376856.
void consumer()
{
    // create segment and corresponding allocator
    bip::managed_shared_memory segment(bip::open_or_create, "MySharedMemory", 65536);
    shm::char_alloc char_alloc(segment.get_segment_manager());

    shm::ring_buffer *queue = nullptr;

    std::cout << "free memory: " << segment.get_free_memory() << std::endl;
    std::cout << "memory size: " << segment.get_size() << std::endl;

    try {
        queue = segment.find_or_construct<shm::ring_buffer>("queue")();
    } catch (boost::interprocess::lock_exception& _) {
        std::cerr << "mem lock exception" << std::endl;
    } catch (std::exception& ex) {
        std::cerr << "unknown exception: " << ex.what() << std::endl;
    }

    if (queue == nullptr) {
        try {
            queue = segment.find_or_construct<shm::ring_buffer>("queue")();
        } catch (std::exception& _) {
            return;
        }
    }

    std::cout << "read available: " << queue->read_available() << std::endl;

    auto counter = 0;
    while (true)
    {
        if ((counter % 1000000) == 0 && counter > 0)
        {
            std::cout << "consumed " << counter / 1000000 << "M messages" << std::endl;
        }

        shm::shared_string v(char_alloc);
        if (!queue->pop(v))
        {
            continue;
            // std::cout << "Processed: '" << v << "'\n";
        }

        counter++;
    }
}
