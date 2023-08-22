#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#include <iostream>


#define BUF_SIZE 1024

bool SendPacket(const char* host, unsigned short port, const char* path, int argc, char* argv[])
{
    // setup udp socket
    UdpTransmitSocket transmitSocket(IpEndpointName(host, port));

    // setup packet
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    osc::OutboundPacketStream p(buf, BUF_SIZE);

    //p << osc::BeginBundleImmediate;

    p << osc::BeginMessage(path);

    int idx = 0;
    while (idx + 1 < argc) {
        std::string type = std::string(argv[idx]);
        std::string val = std::string(argv[idx + 1]);

        if (type == "s") {
            p << val.c_str();
        }
        else if (type == "i") {
            p << atoi(val.c_str());
        }
        else if (type == "f") {
            p << (float)(atof(val.c_str()));
        }
        else if (type == "b") {
            if (val == "true") p << true;
            else p << false;
        }
        else {
            return false;
        }

        idx += 2;
    }

    p << osc::EndMessage;
    //p << osc::EndBundle;

    transmitSocket.Send(p.Data(), p.Size());

    return true;
}


