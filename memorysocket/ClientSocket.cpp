#include "ClientSocket.h"

ClientSocket::ClientSocket() :
_uid(), _endpoint(), _abortRequested(false) {
    auto rand = randombytes_random();
    std::stringstream ss;

    ss << rand;
    ss >> _uid;
}

ClientSocket::~ClientSocket() {
    if (_uid.length()) {
        try {
            bip::shared_memory_object::remove(_uid.c_str());
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

    _endpoint = endpoint;
    try
    {
        bip::managed_shared_memory segment(bip::open_only, endpoint.c_str());
        auto res = segment.find<shm::ring_buffer>("upstream");
        if (res.second == 0) {
            return -1;
        }
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

    try
    {
        bip::managed_shared_memory segment(bip::create_only, uid.c_str(), 65536);
        upstream = segment.construct<shm::ring_buffer>("upstream")();
        if (upstream == nullptr)
        {
            bip::shared_memory_object::remove(uid.c_str());
        }
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