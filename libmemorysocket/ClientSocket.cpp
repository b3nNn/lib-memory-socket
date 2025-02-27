#include "ClientSocket.h"

ClientSocket::ClientSocket(const ClientConfiguration &configuration) :
_uid(), _endpoint(), _abortRequested(false), _config(configuration) {
    auto rand = randombytes_random();
    std::stringstream ss;

    ss << rand;
    ss >> _uid;
}

ClientSocket::~ClientSocket() {
    if (_uid.length()) {
        try {
            bip::message_queue::remove(_uid.c_str());
        }
        catch (bip::interprocess_exception & _)
        {
            std::cerr << "interprocess exception" << std::endl;
        }
        catch (std::exception& ex)
        {
            std::cerr << "unknown exception: " << ex.what() << std::endl;
        }
    }
}

int ClientSocket::Connect(const std::string &endpoint)
{
    std::cout << "client id: " << _uid << std::endl;

    _endpoint = endpoint;
    try
    {
        bip::message_queue mq(bip::open_only, endpoint.c_str());
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

    try
    {
        bip::message_queue mq(bip::create_only, _uid.c_str(), 100, 1000);
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

    return 0;
}

int ClientSocket::Write(const void *data, size_t data_size)
{
    if (_endpoint.length() == 0)
    {
        return 0;
    }

    try
    {
        bip::message_queue mq(bip::open_only, _endpoint.c_str());
        mq.send(data, data_size, 0);
    }
    catch (bip::interprocess_exception& ex)
    {
        std::cerr << "interprocess exception: " << ex.what() << std::endl;
        return 0;
    }
    catch (std::exception& ex)
    {
        std::cerr << "unknown exception: " << ex.what() << std::endl;
        return 0;
    }

    return data_size;
}

void ClientSocket::Abort()
{
    _abortRequested = true;
}
