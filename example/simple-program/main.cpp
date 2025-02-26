#include "MemorySocket.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "sodium.h"

#ifdef _MSC_VER
//not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

std::shared_ptr<ISocket> gl_socket = nullptr;
bool gl_abort_requested = false;

static void signal_abort_handler(int signum)
{
    if (gl_socket != nullptr)
    {
        std::cout << "abort requested" << std::endl;
        gl_socket->Abort();
        gl_abort_requested = true;
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
#ifndef WIN32
    std::signal(SIGHUP, signal_abort_handler);
#endif
    if (argc > 1 && strcasecmp(argv[1], "-p") == 0)
    {
        auto pk = "";
        auto config = std::make_unique<ClientConfiguration>(pk);
        auto client = std::make_shared<ClientSocket>(*config);
        gl_socket = client;
        if (client->Connect("my-endpoint") != 0) {
            std::cout << "server is unreachable" << std::endl;
        }
        else {
            const char *long_msg = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut congue aliquet nisi, eu ultricies enim eleifend eu. Aenean quis quam sem. Donec eu cursus nisi, non sagittis odio. Aliquam pulvinar luctus nisi, a dictum nisl laoreet quis. Sed sagittis quis mi vitae facilisis. Aliquam volutpat erat id augue mollis rutrum. Pellentesque ac ullamcorper massa. Proin ut porta elit. Duis semper sollicitudin ipsum a efficitur. Nullam ultrices aliquam justo, eu accumsan ante venenatis et. Integer ac justo sit.";
            while (!gl_abort_requested)
            {
                if (!client->Write((const void *)long_msg, strlen(long_msg)))
                {
                    std::cout << "socket was closed by server" << std::endl;
                    break;
                }
            }
        }
    }
    else
    {
        auto pk = "3BrJvM6piGcCXJWK1Q+mpm0iwrZe4G2B/eT2dgaceao=";
        auto authorizedKeys = std::vector<std::string>{};
        auto config = std::make_unique<ServerConfiguration>(pk, authorizedKeys);
        auto socket = std::make_shared<ServerSocket>(*config);
        gl_socket = socket;
        if (socket->Listen("my-endpoint") != 0) {
            std::cout << "socket is busy" << std::endl;
        }
    }

    gl_socket = nullptr;

    return 0;
}
