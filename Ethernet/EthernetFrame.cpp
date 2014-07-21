#include "EthernetFrame.h"

#include <cstring>

EthernetFrame::EthernetFrame()
    : m_type(0)
    , m_checksumm(0)
{
    m_data = new std::vector<unsigned char>;
}

EthernetFrame::~EthernetFrame()
{
    if(m_data)
    {
        m_data->clear();
        delete m_data;
    }
}

bool EthernetFrame::decodeFrame(unsigned char* aBuffer, const unsigned aBufferSize)
{
    if(!checkFrameBufferLength(aBufferSize) || !aBuffer)
        return false;

    if(!m_data)
        return false;

    clear();

    unsigned int offset = 0;

    m_destinationMAC.resize(MAC_ADDRESS_LENGTH, 0);
    memcpy(&m_destinationMAC[0], aBuffer, MAC_ADDRESS_LENGTH);
    offset += MAC_ADDRESS_LENGTH;

    m_recipientMAC.resize(MAC_ADDRESS_LENGTH, 0);
    memcpy(&m_recipientMAC[0], aBuffer + offset, MAC_ADDRESS_LENGTH);
    offset += MAC_ADDRESS_LENGTH;

    memcpy((void*)&m_type, aBuffer + offset, ETHERNET_TYPE_LENGTH);
    offset += ETHERNET_TYPE_LENGTH;

    unsigned dataBufferLength = dataLength(aBufferSize);
    if(!dataBufferLength)
        return false;

    m_data->resize(dataBufferLength, 0);
    memcpy(&(*m_data)[0], aBuffer + offset, dataBufferLength);
    offset += dataBufferLength;

    memcpy((void*)&m_checksumm, aBuffer + offset, FRAME_CHECK_SEQUENCE_LENGTH);

    // TODO: Need to check checksumm (CRC-32)

    return true;
}

void EthernetFrame::clear()
{
    m_destinationMAC.clear();
    m_recipientMAC.clear();
    m_type = 0;
    m_data->clear();
    m_checksumm = 0;
}

std::vector<unsigned char> EthernetFrame::destinationMACAddress() const
{
    return m_destinationMAC;
}

std::vector<unsigned char> EthernetFrame::recipientMACAddress() const
{
    return m_recipientMAC;
}

unsigned short EthernetFrame::frameType() const
{
    return m_type;
}

std::vector<unsigned char>* EthernetFrame::frameData() const
{
    return m_data;
}

unsigned long EthernetFrame::checksumm() const
{
    return m_checksumm;
}

bool EthernetFrame::checkFrameBufferLength(const unsigned aBufferSize) const
{
    unsigned minSize = MIN_DATA_LENGTH + MAC_ADDRESS_LENGTH * 2 + ETHERNET_TYPE_LENGTH + FRAME_CHECK_SEQUENCE_LENGTH;
    unsigned maxSize = MAX_DATA_LENGTH + MAC_ADDRESS_LENGTH * 2 + ETHERNET_TYPE_LENGTH + FRAME_CHECK_SEQUENCE_LENGTH;

    if(aBufferSize < minSize && aBufferSize > maxSize)
        return false;

    return true;
}

unsigned int EthernetFrame::dataLength(const unsigned aBufferSize) const
{
    if(!checkFrameBufferLength(aBufferSize))
        return 0;

    return aBufferSize - (MAC_ADDRESS_LENGTH * 2) - ETHERNET_TYPE_LENGTH - FRAME_CHECK_SEQUENCE_LENGTH;
}
