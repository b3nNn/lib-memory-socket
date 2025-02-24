#include "memory_socket.h"
#include "ServerSocket.h"

int main(int argc, char **argv) {
    if (argc > 1 && strcasecmp(argv[1], "-p") == 0)
    {
        auto socket = new ServerSocket();
        producer();
    }
    else
    {
        consumer();
    }

    return 0;
}
