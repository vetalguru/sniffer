//////////////////////////////////////////////////
// RFC:  791: http://tools.ietf.org/html/rfc791 //
//////////////////////////////////////////////////

#include "IPv4.h"
#include <cstring>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

IPv4::IPv4()
    : m_isHeaderDecoded(false)
{
    m_data = new std::vector<unsigned char>;
    clear();
}

IPv4::~IPv4()
{
    if(m_data)
    {
        clear();
        delete m_data;
    }
}

bool IPv4::decodePackage(std::vector<unsigned char>* aBuffer)
{
    m_isHeaderDecoded = readHeader(aBuffer);
    if(!m_isHeaderDecoded)
        return false;

    if(needToReadHeaderOptions())
        readHeaderOptions(aBuffer);

    // TODO: Need to check the header checksumm

    return true;
}

void IPv4::clear()
{
    memset(&m_ipHeader, 0, sizeof(m_ipHeader));
    m_data->clear();
    m_headerOptions.clear();
    m_isHeaderDecoded = false;
}

unsigned char IPv4::version() const
{
    if(!m_isHeaderDecoded)
        return 0;

    return m_ipHeader.version;
}

unsigned char IPv4::headerLength() const
{
    if(!m_isHeaderDecoded)
        return 0;

    return m_ipHeader.ihl;
}


unsigned long IPv4::sourceAddress() const
{
    if(!m_isHeaderDecoded)
        return 0;

    return m_ipHeader.saddr;
}

unsigned long IPv4::destinationAddress() const
{
    if(!m_isHeaderDecoded)
        return 0;

    return m_ipHeader.daddr;
}

std::vector<unsigned char>* IPv4::getPakageData() const
{
    if(!m_isHeaderDecoded)
        return NULL;

    return m_data;
}

bool IPv4::readHeader(std::vector<unsigned char>* aBuffer)
{
    if(aBuffer->empty())
        return false;

    m_isHeaderDecoded = false;

    memcpy(&m_ipHeader, &(*aBuffer)[0], sizeof(m_ipHeader));

    if(m_ipHeader.version != 4)
        return false;

    if(m_ipHeader.tot_len < MIN_IP_PACKAGE_LENGTH ||
       m_ipHeader.tot_len > MAX_IP_PACKAGE_LENGTH)
        return false;
}

bool IPv4::needToReadHeaderOptions()
{
    if(!m_isHeaderDecoded || m_ipHeader.ihl > 20)
        return true;

    return false;
}

bool IPv4::readHeaderOptions(std::vector<unsigned char>* aBuffer)
{
    if(!m_isHeaderDecoded || aBuffer->empty())
        return false;

    unsigned optionsLength = headerOptionLength(m_ipHeader.ihl);
    if(!optionsLength)
        return false;

    m_headerOptions.clear();
    m_headerOptions.resize(optionsLength, 0);
    memcpy(&m_headerOptions[0], &aBuffer[0] + MIN_IP_PACKAGE_LENGTH, optionsLength);

    return true;
}

unsigned IPv4::headerOptionLength(const unsigned aIpHeaderLength) const
{
    if(!aIpHeaderLength)
        return 0;

    return aIpHeaderLength - MIN_IP_PACKAGE_LENGTH;
}

bool IPv4::readData(std::vector<unsigned char>* aBuffer)
{
    if(!m_isHeaderDecoded || aBuffer->empty())
        return false;

    unsigned length = dataLength(m_ipHeader.tot_len);
    if(!length)
        return false;

    m_data->clear();
    m_data->resize(length, 0);
    memcpy(&(*m_data)[0], &aBuffer[0] + m_ipHeader.ihl, length);

    return true;
}

unsigned IPv4::dataLength(const unsigned aIpPackageTotalLength)
{
    if(!m_isHeaderDecoded || !aIpPackageTotalLength)
        return 0;

    return m_ipHeader.tot_len - m_ipHeader.ihl;
}
