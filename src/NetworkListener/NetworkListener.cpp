#include "NetworkListener.h"
#include <stdio.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "IPv4.h"

#include <iostream>

#define BUFFER_SIZE 65535


NetworkListener::NetworkListener(const std::string& aInterface)
    : m_socket(0)
    , m_interface(aInterface)
    , m_mustBeStopped(false)
{
}

NetworkListener::~NetworkListener()
{
}

bool NetworkListener::start()
{
    if(m_interface.empty())
    {
        perror("EMPTY INTERFACE");
        return false;
    }

    if(m_socket)
    {
        perror("SOCKET BIZY");
        return false;
    }

    m_socket = socket(PF_INET, SOCK_PACKET, htons(0x0003));
    if(m_socket < 0)
    {
        perror("SOCKET ERROR");
        return false;
    }

    if(!setPromiscModeOn())
    {
        perror("SET PROMICS MODE ON ERROR");
        close(m_socket);
        return false;
    }

    std::cout << "Sniffer was started" << std::endl;

    m_mustBeStopped = false;

    unsigned char buff[BUFFER_SIZE];
    IPv4 ip;
    while(!m_mustBeStopped)
    {
        ssize_t dataSize = read(m_socket, buff, sizeof(buff));
        if(dataSize != -1)
        {
            // NEED TO USE CHAIN HERE
        }
    }

    if(m_mustBeStopped)
    {
        if(!m_socket)
        {
            perror("NOT STARTED");
            return false;
        }

        if(!setPromiscModeOff())
        {
            perror("SET PROMICS MODE OFF ERROR");
            close(m_socket);
            return false;
        }

        close(m_socket);

        std::cout << "Sniffer was stopped" << std::endl;
    }

    return true;
}

void NetworkListener::stop()
{
    m_mustBeStopped = true;
}

bool NetworkListener::setPromiscModeOn()
{
    if(m_interface.empty())
    {
        perror("EMPTY INTERFACE");
        return false;
    }

    if(m_socket <= 0)
    {
        perror("SOCKET ERROR");
        return false;
    }

    // Get interface flags
    struct ifreq ifr;
    strcpy(ifr.ifr_name, m_interface.c_str());
    if(ioctl(m_socket, SIOCGIFFLAGS, &ifr) < 0)
    {
        perror("CAN'T GET NETWORK CARD MODE");
        return false;
    }

    // Set flag PROMISC as ON
    ifr.ifr_flags |= IFF_PROMISC;
    if(ioctl(m_socket, SIOCSIFFLAGS, &ifr) < 0)
    {
        perror("CAN'T CHANGE NETWORK CARD MODE");
        return false;
    }

    return true;
}

bool NetworkListener::setPromiscModeOff()
{
    if(m_interface.empty())
    {
        perror("EMPTY INTERFACE");
        return false;
    }

    if(m_socket <= 0)
    {
        perror("SOCKET ERROR");
        return false;
    }

    // Get interface flags
    struct ifreq ifr;
    strcpy(ifr.ifr_name, m_interface.c_str());
    if(ioctl(m_socket, SIOCGIFFLAGS, &ifr) < 0)
    {
        perror("CAN'T GET NETWORK CARD MODE");
        return false;
    }

    // Set flag PROMISC as OFF
    ifr.ifr_flags &= (~IFF_PROMISC);
    if(ioctl(m_socket, SIOCSIFFLAGS, &ifr) < 0)
    {
        perror("CAN'T CHANGE NETWORK CARD MODE");
        return false;
    }

    return true;
}
