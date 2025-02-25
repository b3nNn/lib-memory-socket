#ifndef LIB_MEMORY_SOCKET_ISOCKET_H
#define LIB_MEMORY_SOCKET_ISOCKET_H

class ISocket {
public:
    virtual ~ISocket() = default;
    virtual void    Abort() = 0;
};

#endif //LIB_MEMORY_SOCKET_ISOCKET_H
