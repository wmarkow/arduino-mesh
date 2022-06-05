#include "MeshNode.h"

class ComplexMeshNode : public MeshNode
{
private:
    Interface *interface1;

protected:
    bool flood(IotPacket* packet);

public:
    ComplexMeshNode();

    /** Network support */
    void setIpAddress(uint8_t ipAddress);
    PingResult ping(uint8_t dst);
    bool sendTcp(uint8_t dstAddress, uint8_t* data, uint8_t length);
    void sendUdp(uint8_t dstAddress, uint8_t* data, uint8_t length);

    /** Interfaces support */
    uint8_t getInterfaceCount();
    Interface* getInterface(uint8_t index);
    void setInterface(uint8_t index, Interface* interfacePtr);
    
    /** Various features */
    void loop();
    void setWiresharkEnabled(bool enabled);
};