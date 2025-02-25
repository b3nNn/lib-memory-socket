#ifndef LIB_MEMORY_SOCKET_CLIENTSOCKET_H
#define LIB_MEMORY_SOCKET_CLIENTSOCKET_H

#include <sstream>
#include <iostream>

#include "ISocket.h"
#include "memory_socket.h"
#include "sodium.h"

class ClientSocket : public ISocket {
public:
    ClientSocket();
    ~ClientSocket() override;
    int Connect(const std::string &endpoint);
    int Write(const void *data, size_t data_size);
    void Abort() override;
private:
    std::string _uid;
    std::string _endpoint;
    bool _abortRequested;
};

#endif //LIB_MEMORY_SOCKET_CLIENTSOCKET_H
