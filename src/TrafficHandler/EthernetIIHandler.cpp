#include "EthernetIIHandler.h"

EthernetIIHandler::EthernetIIHandler()
    : m_successor(NULL)
    , m_nextSuccessor(NULL)
{
}

bool EthernetIIHandler::handlerRequest(std::string& request)
{
    if(!m_successor)
        return false;

    if(request.empty())
        return false;

}

void EthernetIIHandler::setSuccessor(Handler* successor)
{
    m_successor = successor;
}

Handler* EthernetIIHandler::getNextSuccessor()
{
    return NULL;
}

bool EthernetIIHandler::checkFrameLength(const size_t bufferSize)
{
    if(!bufferSize)
        return false;


    if(bufferSize < MIN_FRAME_SIZE || bufferSize > MAX_FRAME_SIZE)
        return false;

    return true;
}

bool EthernetIIHandler::getDestinationAddress(const std::string& buffer, std::string& destAddress)
{
    destAddress.clear();

    if(!checkFrameLength(buffer.size()))
        return false;

    destAddress = buffer.substr(0, MAC_ADDRESS_SIZE);
    return true;
}

bool EthernetIIHandler::getSourceAddress(const std::string& buffer, std::string& sourceAddress)
{
    sourceAddress.clear();

    if(!checkFrameLength(buffer.size()))
        return false;

    sourceAddress = buffer.substr(MAC_ADDRESS_SIZE, MAC_ADDRESS_SIZE * 2);
    return true;
}

bool EthernetIIHandler::getType(const std::string& buffer, std::string& type)
{
    type.clear();

    if(!checkFrameLength(buffer.size()))
        return false;

    type = buffer.substr(MAC_ADDRESS_SIZE * 2, MAC_ADDRESS_SIZE * 2 + ETHERNET_TYPE_SIZE);
    return true;
}

bool EthernetIIHandler::getData(const std::string& frame, std::string& data)
{
    data.clear();

    if(!checkFrameLength(frame.size()))
        return false;

    unsigned headerSize = MAC_ADDRESS_SIZE * 2 + ETHERNET_TYPE_SIZE;
    unsigned size = frame.size() - CRC_CHECKSUMM - headerSize;

    data = frame.substr(headerSize, size);

    return true;
}

bool EthernetIIHandler::getCRC(const std::string& frame, std::string& crc)
{
    crc.clear();

    if(!checkFrameLength(frame.size()))
        return false;

    crc = frame.substr(frame.size() - CRC_CHECKSUMM, CRC_CHECKSUMM);

    return true;
}
