#ifndef LIB_MEMORY_SOCKET_SERVERCONFIGURATION_H
#define LIB_MEMORY_SOCKET_SERVERCONFIGURATION_H
#include <string>
#include <vector>


class ServerConfiguration {
public:
    ServerConfiguration(const std::string &publicKey, const std::vector<std::string> &authorizedKeys);

    const std::string &GetPublicKey() const {
        return _publicKey;
    }

private:
    const std::string _publicKey;
    const std::vector<std::string>  _authorizedKeys;
};



#endif //LIB_MEMORY_SOCKET_SERVERCONFIGURATION_H
