#ifndef LIB_MEMORY_SOCKET_MEMORY_SOCKET_H
#define LIB_MEMORY_SOCKET_MEMORY_SOCKET_H

#include <boost/lockfree/spsc_queue.hpp> // ring buffer

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/node_allocator.hpp>
#include <boost/interprocess/containers/string.hpp>

namespace bip = boost::interprocess;
namespace shm
{
    typedef bip::node_allocator<char, bip::managed_shared_memory::segment_manager> char_alloc;
    typedef bip::basic_string<char, std::char_traits<char>, char_alloc >      shared_string;

    typedef boost::lockfree::spsc_queue<
        shared_string,
        boost::lockfree::capacity<100>
    > ring_buffer;
}

typedef struct      s_gossip {
    std::string     endpoint;
    std::string     endpoint_hash;
    std::string     endpoint_auth_tag;
    std::string     public_key;
    std::string     public_key_hash;
    std::string     public_key_auth_tag;
}                   t_gossip;

class Gossip {
public:
    Gossip(std::string &endpoint,
    std::string  &endpoint_hash,
    std::string  &endpoint_auth_tag,
    std::string  &public_key,
    std::string  &public_key_hash,
    std::string  &public_key_auth_tag);

    Gossip(Gossip &ref);
};

typedef bip::allocator<t_gossip, bip::managed_shared_memory::segment_manager> gossip_allocator;

void producer();
void consumer();

#endif //LIB_MEMORY_SOCKET_MEMORY_SOCKET_H
