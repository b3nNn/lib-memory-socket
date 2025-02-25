#include "memory_socket.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "sodium.h"

ISocket *gl_socket = nullptr;

static void signal_abort_handler(int signum)
{
    if (gl_socket != nullptr)
    {
        std::cout << "abort requested" << std::endl;
        gl_socket->Abort();
    }
}

int main(int argc, char **argv) {

    if (sodium_init() < 0) {
        return -1;
    }

    // Install a signal handler
    std::signal(SIGINT, signal_abort_handler);
    std::signal(SIGTERM, signal_abort_handler);
    std::signal(SIGABRT, signal_abort_handler);
    std::signal(SIGHUP, signal_abort_handler);

    if (argc > 1 && strcasecmp(argv[1], "-p") == 0)
    {
        auto client = new ClientSocket();
        gl_socket = client;
        if (client->Connect("my-endpoint") != 0) {
            std::cout << "server is unreachable" << std::endl;
        }
        // producer();
        delete client;
    }
    else
    {
        auto socket = new ServerSocket();
        gl_socket = socket;
        if (socket->Listen("my-endpoint") != 0) {
            std::cout << "socket is busy" << std::endl;
        }
        // consumer();
        delete socket;
    }

    return 0;
}
