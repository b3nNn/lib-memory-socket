#include "ServerSocket.h"
#include "sodium.h"
#include <chrono>
#include <iostream>
#include <sstream>

ServerSocket::ServerSocket() :
_endpoint(), _abortRequested(false), _pk("3BrJvM6piGcCXJWK1Q+mpm0iwrZe4G2B/eT2dgaceao="), _hasOwnership(false)
{
    if (_pk.empty())
    {
        unsigned char pk[crypto_sign_PUBLICKEYBYTES];
        unsigned char sk[crypto_sign_SECRETKEYBYTES];

        std::stringstream ss;
        crypto_sign_keypair(pk, sk);

        const size_t bin_len = sizeof(pk);
        const size_t  base64_max_len = sodium_base64_encoded_len(bin_len, sodium_base64_VARIANT_ORIGINAL);
        std::string base64_str(base64_max_len-1,0);
        char *encoded_str_char = sodium_bin2base64(
                 base64_str.data(),
                 base64_max_len,
                 pk,
                 bin_len,
                 sodium_base64_VARIANT_ORIGINAL
         );

        if (encoded_str_char == NULL) {
            std::cerr << "base64 error" << std::endl;
        }
        _pk = std::string(encoded_str_char);
        std::cout << "public key is: [" << _pk << "]" << std::endl;
    }
}

ServerSocket::~ServerSocket() {
    if (_endpoint.length() && _hasOwnership)
    {
        bip::managed_shared_memory segment(bip::open_only, _endpoint.c_str());

        try
        {
            bip::shared_memory_object::remove(_endpoint.c_str());
        }
        catch (boost::interprocess::lock_exception& _)
        {
            std::cerr << "mem lock exception" << std::endl;
        }
        catch (std::exception& ex)
        {
            std::cerr << "unknown exception: " << ex.what() << std::endl;
        }
    }
}

int ServerSocket::Listen(const std::string &endpoint) {
    // Ringbuffer fully constructed in shared memory. The element strings are
    // also allocated from the same shared memory segment. This vector can be
    // safely accessed from other processes.
    shm::ring_buffer *upstream = nullptr;

    auto recovery = false;
    try
    {
        _endpoint = endpoint;
        bip::managed_shared_memory segment(bip::create_only, endpoint.c_str(), 65536);
        bip::shared_memory_object::remove(endpoint.c_str());
    }
    catch (boost::interprocess::lock_exception& _)
    {
        std::cerr << "socket is busy" << std::endl;
        return -1;
    }
    catch (std::exception& ex)
    {
        recovery = true;
        std::cerr << "socket is busy: " << ex.what() << std::endl;
    }

    if (recovery)
    {
        try {
            bip::managed_shared_memory segment(bip::open_only, endpoint.c_str());
            auto res = segment.find<shm::shared_string>("pk");
            auto ts = segment.find<uint64_t>("ts");
            if (res.second == 0 || ts.second == 0)
            {
                std::cout << "recover no public key or ts" << std::endl;
                bip::shared_memory_object::remove(endpoint.c_str());
            }
            else
            {
                std::string pk(res.first->begin(), res.first->end());
                std::cout << "recover public key: [" << pk  << "]" << std::endl;
                std::cout << "recover ts: " << *ts.first << std::endl;

                if (pk != _pk)
                {
                    return -1;
                }

                std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
                    std::chrono::system_clock::now().time_since_epoch()
                );

                std::cout << "public keys are matching" << std::endl;

                if ((ms.count() - *ts.first) < 15000) // 15 seconds
                {
                    std::cout << "timeout period not yet expired" << std::endl;
                    return -1;
                }

                bip::shared_memory_object::remove(endpoint.c_str());
            }
        }
        catch (std::exception& ex)
        {
            std::cerr << "socket is busy: " << ex.what() << std::endl;
            return -1;
        }
    }

    try
    {
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );
        bip::managed_shared_memory segment(bip::create_only, endpoint.c_str(), 65536);
        _hasOwnership = true;
        auto pk = segment.construct<shm::shared_string>("pk")(_pk.c_str(), segment.get_segment_manager());
        upstream = segment.construct<shm::ring_buffer>("upstream")();
        auto ts = segment.construct<uint64_t>("ts")(ms.count());
    }
    catch (boost::interprocess::lock_exception& _)
    {
        std::cerr << "mem lock exception" << std::endl;
        return -1;
    }
    catch (std::exception& ex)
    {
        std::cerr << "unknown exception: " << ex.what() << std::endl;
        return -1;
    }

    if (upstream == nullptr)
    {
        return -1;
    }

    return this->ListenForever();
}

int ServerSocket::ListenForever()
{
    // create segment and corresponding allocator
    bip::managed_shared_memory segment(bip::open_only, _endpoint.c_str());
    shm::char_alloc char_alloc(segment.get_segment_manager());
    // Ringbuffer fully constructed in shared memory. The element strings are
    // also allocated from the same shared memory segment. This vector can be
    // safely accessed from other processes.
    auto res = segment.find<shm::ring_buffer>("upstream");
    auto ts = segment.find<uint64_t>("ts");

    if (!res.second || !ts.second)
    {
        return -1;
    }

    std::cout << "ts is: " << *ts.first << std::endl;
    auto counter = 0;
    while (!_abortRequested)
    {
        std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
        );
        *ts.first = ms.count();
        shm::shared_string v(char_alloc);
        if (!res.first->pop(v))
        {
            continue;
        }

        counter++;
        if ((counter % 1000000) == 0)
        {
            std::cout << "consumed " << counter / 1000000 << "M messages" << std::endl;
        }
    }
    return 0;
}

void ServerSocket::Abort()
{
    _abortRequested = true;
}
