#ifndef LIB_MEMORY_SOCKET_MEMORY_SOCKET_H
#define LIB_MEMORY_SOCKET_MEMORY_SOCKET_H

#include <boost/lockfree/spsc_queue.hpp> // ring buffer

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

namespace bip = boost::interprocess;
namespace shm
{
    typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> char_alloc;
    typedef bip::basic_string<char, std::char_traits<char>, char_alloc >      shared_string;

    typedef boost::lockfree::spsc_queue<
        shared_string,
        boost::lockfree::capacity<200>
    > ring_buffer;
}

void producer();
void consumer();

#endif //LIB_MEMORY_SOCKET_MEMORY_SOCKET_H
