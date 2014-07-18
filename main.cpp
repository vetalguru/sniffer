#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <iostream>

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

    int raw_socket = socket(PF_INET, SOCK_PACKET, htons(0x0003));
    if(raw_socket < 0)
    {
        perror("SOCKET ERROR");
        return 0;
    }

    // Get interface flags
    struct ifreq ifr;
    strcpy(ifr.ifr_name, argv[1]);
    if(ioctl(raw_socket, SIOCGIFFLAGS, &ifr) < 0)
          return false;

    // Set flag PROMISC as ON
    ifr.ifr_flags |= IFF_PROMISC;
    if(ioctl(raw_socket, SIOCSIFFLAGS, &ifr) < 0)
    {
          perror("IOCTL SET FLAGS ERROR");
          return false;
    }

    std::cout << "Listening on " << argv[1] << std::endl;

    char buff[BUFFER_SIZE];
    while(done)
    {
        ssize_t dataSize = read(raw_socket, buff, sizeof(buff));
        if(dataSize != -1)
            write(STDOUT_FILENO, buff, dataSize);
    }

    // Set flag PROMISC as OFF
    ifr.ifr_flags &= (~IFF_PROMISC);
    if (ioctl(raw_socket, SIOCSIFFLAGS, &ifr) < 0)
    {
          perror("IOCTL SET FLAGS ERROR");
          return 0;
    }

    close(raw_socket);
    std::cout << "Finished" << std::endl;

    return 0;
}
