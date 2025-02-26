#include "ServerConfiguration.h"

ServerConfiguration::ServerConfiguration(const std::string &publicKey, const std::vector<std::string> &authorizedKeys)
    : _publicKey(publicKey), _authorizedKeys(authorizedKeys.begin(), authorizedKeys.end()) {
}
