#include <iostream>

#include "memory_socket.h"

// Original code from https://gist.github.com/sehe/9376856.
void producer()
{
    // // create segment and corresponding allocator
    // bip::managed_shared_memory segment(bip::open_or_create, "my-unique-socket-identifier", 65536);
    // shm::char_alloc char_alloc(segment.get_segment_manager());
    //
    // // Ringbuffer fully constructed in shared memory. The element strings are
    // // also allocated from the same shared memory segment. This vector can be
    // // safely accessed from other processes.
    // shm::ring_buffer *queue = nullptr;
    //
    // try {
    //     queue = segment.find_or_construct<shm::ring_buffer>("queue")();
    // } catch (boost::interprocess::lock_exception& _) {
    //     std::cerr << "mem lock exception" << std::endl;
    // } catch (std::exception& ex) {
    //     std::cerr << "unknown exception: " << ex.what() << std::endl;
    // }
    //
    // if (queue == nullptr) {
    //     return;
    // }
    //
    // const char* messages[] = { "hello world", "the answer is 42", "where is your towel", 0 };
    // auto counter = 0;
    // while (true)
    // {
    //     for (const char** msg_it = messages; *msg_it; ++msg_it)
    //     {
    //         if (!queue->push(shm::shared_string(*msg_it, char_alloc))) {
    //             break;
    //         }
    //         counter++;
    //
    //         if ((counter % 1000000) == 0)
    //         {
    //             std::cout << "produced " << counter / 1000000 << "M messages" << std::endl;
    //         }
    //     }
    // }
}

// Original code from https://gist.github.com/sehe/9376856.
void consumer()
{
    // // create segment and corresponding allocator
    // bip::managed_shared_memory segment(bip::open_or_create, "my-unique-socket-identifier", 65536);
    // shm::char_alloc char_alloc(segment.get_segment_manager());
    // shm::ring_buffer *queue = nullptr;
    //
    // try {
    //     queue = segment.find_or_construct<shm::ring_buffer>("queue")();
    // } catch (boost::interprocess::lock_exception& _) {
    //     std::cerr << "mem lock exception" << std::endl;
    // } catch (std::exception& ex) {
    //     std::cerr << "unknown exception: " << ex.what() << std::endl;
    // }
    //
    // if (queue == nullptr) {
    //     return;
    // }
    //
    // auto counter = 0;
    // while (true)
    // {
    //     shm::shared_string v(char_alloc);
    //     if (!queue->pop(v))
    //     {
    //         continue;
    //     }
    //
    //     counter++;
    //     if ((counter % 1000000) == 0)
    //     {
    //         std::cout << "consumed " << counter / 1000000 << "M messages" << std::endl;
    //     }
    // }
}
