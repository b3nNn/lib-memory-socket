#include "ServerSocket.h"
#include "sodium.h"
#include <chrono>
#include <iostream>
#include <sstream>

ServerSocket::ServerSocket(const ServerConfiguration &config) :
_endpoint(), _abortRequested(false), _pk("3BrJvM6piGcCXJWK1Q+mpm0iwrZe4G2B/eT2dgaceao="), _hasOwnership(false), _config(config)
{
    if (_config.GetPublicKey().empty())
    {
        unsigned char pk[crypto_sign_PUBLICKEYBYTES];
        unsigned char sk[crypto_sign_SECRETKEYBYTES];

        crypto_sign_keypair(pk, sk);

        char *encoded_str_char = Base64Encoding::Encode(&pk, sizeof(pk));

        if (encoded_str_char == nullptr) {
            throw "base64 error";
        }
        std::cout << "public key is: [" << _pk << "]" << std::endl;
    }
}

ServerSocket::~ServerSocket() {
    if (_endpoint.length() && _hasOwnership)
    {
        bip::message_queue mq(bip::open_only, _endpoint.c_str());

        try
        {
            bip::shared_memory_object::remove(_endpoint.c_str());
        }
        catch (bip::interprocess_exception& _)
        {
            std::cerr << "interprocess exception" << std::endl;
        }
        catch (std::exception& ex)
        {
            std::cerr << "unknown exception: " << ex.what() << std::endl;
        }
    }
}

int ServerSocket::Listen(const std::string &endpoint) {
    _endpoint = endpoint;

    try
    {
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );
        bip::message_queue mq(bip::create_only, endpoint.c_str(), 100, 1000);
        _hasOwnership = true;
    }
    catch (bip::interprocess_exception& _)
    {
        std::cerr << "interprocess exception" << std::endl;
        return -1;
    }
    catch (std::exception& ex)
    {
        std::cerr << "unknown exception: " << ex.what() << std::endl;
        return -1;
    }

    return this->ListenForever();
}

int ServerSocket::ListenForever()
{
    // create segment and corresponding allocator
    bip::message_queue mq(bip::open_only, _endpoint.c_str());

    // std::cout << "ts is: " << *ts.first << std::endl;
    auto counter = 0;
    while (!_abortRequested)
    {
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );
        char buffer[1000];
        unsigned int priority;
        bip::message_queue::size_type recvd_size;
        try
        {
            if (!mq.try_receive(&buffer, 1000, recvd_size, priority))
            {
                continue;
            }
        }
        catch (bip::interprocess_exception& _)
        {
            std::cerr << "interprocess exception" << std::endl;
            return -1;
        }
        catch (std::exception& ex)
        {
            std::cerr << "unknown exception: " << ex.what() << std::endl;
            return -1;
        }

        counter++;
        if ((counter % 1000) == 0)
        {
            std::cout << "consumed " << counter / 1000 << "k messages" << std::endl;
        }
    }
    return 0;
}

void ServerSocket::Abort()
{
    _abortRequested = true;
}
