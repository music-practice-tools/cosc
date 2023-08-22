#include "osc/OscReceivedElements.h"
#include "osc/OscPrintReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/IpEndpointName.h"
#include "ip/UdpSocket.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

class OSCPacketListener : public osc::OscPacketListener {
/*
    protected:
        virtual void ProcessPacket(const char* data, int size,
            const IpEndpointName& remoteEndpoint)
        {
            //        (void)remoteEndpoint; // suppress unused parameter warning

            std::cout << osc::ReceivedPacket(data, size);

            OscPacketListener::ProcessPacket(data, size, remoteEndpoint);
        }
*/

protected:

    void ProcessMessage(const osc::ReceivedMessage& m,
        const IpEndpointName& remoteEndpoint)
    {
        (void)remoteEndpoint; // suppress unused parameter warning

        try {
            osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

            const char* address = (*(m.AddressPattern()) == '/') ? m.AddressPattern() + 1 : m.AddressPattern();
            std::cout << address << " ";

            while (arg != m.ArgumentsEnd()) {
                if (arg->IsBool()) {
                    bool a = (arg++)->AsBoolUnchecked();
                    std::cout << a << " ";
                }
                else if (arg->IsInt32()) {
                    int a = (arg++)->AsInt32Unchecked();
                    std::cout << a << " ";
                }
                else if (arg->IsFloat()) {
                    float a = (arg++)->AsFloatUnchecked();
                    std::cout << a << " ";
                }
                else if (arg->IsString()) {
                    const char* a = (arg++)->AsStringUnchecked();
                    std::cout << a << " ";
                }
                else {
                }
            }
            std::cout << "\n";
        }
        catch (osc::Exception& e) {
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
};

void ReceivePacket(int address = IpEndpointName::ANY_ADDRESS, unsigned short port = IpEndpointName::ANY_PORT, int timeout = 1000, std::function<void()> send = []() {})
{
    // now wait for responses with timeout                                               s
    OSCPacketListener listener;
    UdpListeningReceiveSocket s(
        IpEndpointName(address, port),
        &listener);

    if (timeout) {
        // Async timer to stop listening
        auto timerThread = std::thread([&]() {
            auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout);
            std::this_thread::sleep_until(delta);
            s.AsynchronousBreak();
            });
        timerThread.detach();

        send();

        s.RunUntilSigInt();

        timerThread.~thread();
    }
    else
    {
        s.RunUntilSigInt();
    }
}
