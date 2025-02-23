#include "memory_socket.h"

int main(int argc, char **argv) {
    if (argc > 1 && strcasecmp(argv[1], "-p") == 0)
    {
        producer();
    }
    else
    {
        consumer();
    }

    return 0;
}
