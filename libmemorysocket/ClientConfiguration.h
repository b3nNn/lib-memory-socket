#ifndef LIB_MEMORY_SOCKET_CLIENTCONFIGURATION_H
#define LIB_MEMORY_SOCKET_CLIENTCONFIGURATION_H
#include <string>


class ClientConfiguration {
public:
    ClientConfiguration(const std::string &publicKey);
private:
    const std::string _publicKey;
};



#endif //LIB_MEMORY_SOCKET_CLIENTCONFIGURATION_H
