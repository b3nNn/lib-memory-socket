#include "memory_socket.h"
#include "ServerSocket.h"

int main(int argc, char **argv) {

    if (sodium_init() < 0) {
        return -1;
    }

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
