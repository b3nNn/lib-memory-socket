#include "ClientSocket.h"

ClientSocket::ClientSocket() :
_uid(), _endpoint() {
    auto rand = randombytes_random();
    std::stringstream ss;

    ss << rand;
    ss >> _uid;
}

ClientSocket::~ClientSocket() {
    if (_endpoint.length()) {
        try {
            bip::managed_shared_memory segment(bip::open_only, _endpoint.c_str());
            auto res = segment.find<shm::ring_buffer>(_uid.c_str());
            if (res.second == 1) {
                segment.destroy<shm::ring_buffer>(_uid.c_str());
            }
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

int ClientSocket::Connect(const std::string &endpoint)
{
    // create segment and corresponding allocator
    auto rand = randombytes_random();
    std::stringstream ss;
    std::string uid;
    ss << rand;
    ss >> uid;

    std::cout << "client id: " << uid << std::endl;
    // Ringbuffer fully constructed in shared memory. The element strings are
    // also allocated from the same shared memory segment. This vector can be
    // safely accessed from other processes.
    shm::ring_buffer *upstream = nullptr;

    try
    {
        _endpoint = endpoint;
        bip::managed_shared_memory segment(bip::open_only, endpoint.c_str());
        auto res = segment.find<shm::ring_buffer>("upstream");
        if (res.second == 0) {
            return -1;
        }
        upstream = segment.construct<shm::ring_buffer>(uid.c_str())();
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

    return 0;
}

void ClientSocket::Abort()
{
    _abortRequested = true;
}