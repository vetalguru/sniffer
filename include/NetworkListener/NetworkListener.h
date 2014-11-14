#ifndef NETWORK_LISTENER_H
#define NETWORK_LISTENER_H

#include <sys/types.h>
#include <sys/socket.h>

#include <string>

class NetworkListener
{
public:
    NetworkListener(const std::string& aInterface);
    ~NetworkListener();

    bool start();
    bool stop();

private:
    bool setPromiscModeOn();
    bool setPromiscModeOff();

private:
    int m_socket;
    std::string m_interface;
};


#endif // NETWORK_LISTENER_H
