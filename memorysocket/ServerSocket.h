#ifndef LIB_MEMORY_SOCKET_SERVERSOCKET_H
#define LIB_MEMORY_SOCKET_SERVERSOCKET_H

#include <csignal>
#include <iostream>

#include "ISocket.h"
#include "memory_socket.h"

class ServerSocket : public ISocket {
public:
    ServerSocket();
    ~ServerSocket();
    int Listen(const std::string &endpoint);
    void Abort() override;
private:
    int ListenForever();

    std::string _endpoint;
    std::string _pk;
    bool        _abortRequested;
};

#endif //LIB_MEMORY_SOCKET_SERVERSOCKET_H
