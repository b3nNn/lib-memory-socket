#ifndef LIB_MEMORY_SOCKET_SERVERSOCKET_H
#define LIB_MEMORY_SOCKET_SERVERSOCKET_H

#include <csignal>
#include <iostream>

#include "ISocket.h"
#include "MemorySocket.h"
#include "ServerConfiguration.h"
#include "Base64Encoding.h"

class ServerSocket : public ISocket {
public:
    ServerSocket(const ServerConfiguration &config);
    ~ServerSocket();
    int Listen(const std::string &endpoint);
    void Abort() override;
private:
    int ListenForever();

    std::string _endpoint;
    std::string _pk;
    bool        _abortRequested;
    bool        _hasOwnership;
    const ServerConfiguration _config;
};

#endif //LIB_MEMORY_SOCKET_SERVERSOCKET_H
