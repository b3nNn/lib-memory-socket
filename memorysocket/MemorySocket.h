#ifndef LIB_MEMORY_SOCKET_MEMORY_SOCKET_H
#define LIB_MEMORY_SOCKET_MEMORY_SOCKET_H

#include <boost/interprocess/ipc/message_queue.hpp>

namespace bip = boost::interprocess;
namespace ipc
{
    typedef bip::message_queue message_queue;
}

#endif //LIB_MEMORY_SOCKET_MEMORY_SOCKET_H
