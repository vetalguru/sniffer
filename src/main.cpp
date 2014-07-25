#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <iostream>
#include <arpa/inet.h>

#include "EthernetFrame.h"
#include "IPv4.h"

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
    IPv4 ip;
    while(done)
    {
        ssize_t dataSize = read(raw_socket, buff, sizeof(buff));
        if(dataSize != -1)
        {
            frame.clear();
            frame.decodeFrame(buff, dataSize);
            if(frame.frameType() == 8)
            {
                if(!ip.decodePackage(frame.frameData()))
                    continue;

                struct in_addr sAddr, dAddr;
                sAddr.s_addr = ip.sourceAddress();
                char* ipSource = inet_ntoa(sAddr);
                dAddr.s_addr = ip.destinationAddress();
                char* ipDestination = inet_ntoa(dAddr);
                printf("Source: %s  Destination: %s\n", ipSource, ipDestination);
            }
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