#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>

#include "EthernetFrame.h"

#define BUFFER_SIZE 65536

static volatile int done = 1;
void handler(int signum = 0)
{
    done = 0;
}

bool setPromiscModeOn(const int aSocket, const std::string& aInterface)
{
    if(aSocket <= 0 || aInterface.empty())
        return false;

    // Get interface flags
    struct ifreq ifr;
    strcpy(ifr.ifr_name, aInterface.c_str());
    if(ioctl(aSocket, SIOCGIFFLAGS, &ifr) < 0)
        return false;

    // Set flag PROMISC as ON
    ifr.ifr_flags |= IFF_PROMISC;
    if(ioctl(aSocket, SIOCSIFFLAGS, &ifr) < 0)
        return false;

    return true;
}

bool setPromiscModeOff(const int aSocket, const std::string& aInterface)
{
    if(aSocket <= 0 || aInterface.empty())
        return false;

    // Get interface flags
    struct ifreq ifr;
    strcpy(ifr.ifr_name, aInterface.c_str());
    if(ioctl(aSocket, SIOCGIFFLAGS, &ifr) < 0)
        return false;

    // Set flag PROMISC as OFF
    ifr.ifr_flags &= (~IFF_PROMISC);
    if(ioctl(aSocket, SIOCSIFFLAGS, &ifr) < 0)
        return false;

    return true;
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

    if(!setPromiscModeOn(raw_socket, std::string(argv[1])))
    {
        perror("SET PROMICS MODE ON");
        close(raw_socket);
        return 0;
    }

    std::cout << "Listening on " << argv[1] << std::endl;

    unsigned char buff[BUFFER_SIZE];
    EthernetFrame frame;
    while(done)
    {
        ssize_t dataSize = read(raw_socket, buff, sizeof(buff));
        if(dataSize != -1)
        {
            frame.clear();
            frame.decodeFrame(buff, dataSize);
            printf("Dect MAC: %x-%x-%x-%x-%x-%x \t", frame.destinationMACAddress()[0],
                                                     frame.destinationMACAddress()[1],
                                                     frame.destinationMACAddress()[2],
                                                     frame.destinationMACAddress()[3],
                                                     frame.destinationMACAddress()[4],
                                                     frame.destinationMACAddress()[5]);

            printf("Recip MAC: %x-%x-%x-%x-%x-%x\n", frame.recipientMACAddress()[0],
                                                     frame.recipientMACAddress()[1],
                                                     frame.recipientMACAddress()[2],
                                                     frame.recipientMACAddress()[3],
                                                     frame.recipientMACAddress()[4],
                                                     frame.recipientMACAddress()[5]);

        }
    }

    if(!setPromiscModeOff(raw_socket, std::string(argv[1])))
    {
        perror("SET PROMISC MODE OFF");
        close(raw_socket);
        return 0;
    }

    close(raw_socket);
    std::cout << "Finished" << std::endl;

    return 0;
}
