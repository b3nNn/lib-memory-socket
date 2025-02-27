#ifndef LIB_MEMORY_SOCKET_CLIENTSOCKET_H
#define LIB_MEMORY_SOCKET_CLIENTSOCKET_H

#include <sstream>
#include <iostream>

#include "ClientConfiguration.h"
#include "ISocket.h"
#include "MemorySocket.h"
#include "sodium.h"

class ClientSocket : public ISocket {
public:
    ClientSocket(const ClientConfiguration &configuration);
    ~ClientSocket() override;
    int Connect(const std::string &endpoint);
    int Write(const void *data, size_t data_size);
    void Abort() override;
private:
    std::string _uid;
    std::string _endpoint;
    bool _abortRequested;
    const ClientConfiguration _config;
};

#endif //LIB_MEMORY_SOCKET_CLIENTSOCKET_H
