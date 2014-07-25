#ifndef ETHERNETFRAME_H
#define ETHERNETFRAME_H

/*
 * The class describes the Ethernet II frames
 */

#include <vector>

class EthernetFrame
{
    public:
        EthernetFrame();
        ~EthernetFrame();

        bool decodeFrame(unsigned char* aBuffer,const unsigned aBufferSize);
        void clear();

        std::vector<unsigned char> destinationMACAddress() const;
        std::vector<unsigned char> recipientMACAddress() const;
        unsigned short frameType() const;
        std::vector<unsigned char>* frameData() const;
        unsigned long checksumm() const;

    private:
        bool checkFrameBufferLength(const unsigned aBufferSize) const;
        unsigned int dataLength(const unsigned aBufferSize) const;

    private:
        static const unsigned MAC_ADDRESS_LENGTH = 6;
        static const unsigned ETHERNET_TYPE_LENGTH = 2;
        static const unsigned FRAME_CHECK_SEQUENCE_LENGTH = 4;

        static const unsigned MIN_DATA_LENGTH = 46;
        static const unsigned MAX_DATA_LENGTH = 1500;

    private:
        std::vector<unsigned char> m_destinationMAC;
        std::vector<unsigned char> m_recipientMAC;

        unsigned short              m_type;
        std::vector<unsigned char>* m_data;
        unsigned long               m_checksumm;
};

#endif // ETHERNETFRAME_H
