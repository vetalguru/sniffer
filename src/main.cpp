#include <signal.h>
#include <iostream>

#include "NetworkListener.h"
#include "IPv4.h"

#define BUFFER_SIZE 65536

static volatile int done = 1;
void handler(int signum = 0)
{
    done = 0;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <interface>" << std::endl;
        return 0;
    }

    signal(SIGINT,  handler);
    signal(SIGQUIT, handler);
    signal(SIGTERM, handler);

    NetworkListener listener(argv[1]);
    listener.start();

    return 0;
}
