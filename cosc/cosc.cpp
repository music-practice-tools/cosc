//
//  cosc.cpp : 
//      Open Sound Control packet sending and receiving program.
//

// oscpack
// http://www.rossbencina.com/code/oscpack
#include "ip/IpEndpointName.h"

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <functional>

extern void ReceivePacket(int address, unsigned short port, int timeout, std::function<void()> foo);
extern bool SendPacket(const char* host, unsigned short port, const char* path, int argc, char* argv[]);

void usage()
{
    fprintf(stderr, "cosc - send and receive OSC messages\n");
    fprintf(stderr, "Transmit a single command or Receive until timeout in milliseconds, or both\n\n");
    fprintf(stderr, "usage: cosc dst_host{ dst_port | 0 }{ rcv_port | 0 } {timeout ms | 0} path [[type][param]] ...\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  type\n");
    fprintf(stderr, "    i : int\n");
    fprintf(stderr, "    f : float\n");
    fprintf(stderr, "    b : boolean (true/false)\n");
    fprintf(stderr, "    s : string\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  Examples\n");
    fprintf(stderr, "    ./cosc - show usage \n");
    fprintf(stderr, "    ./cosc 127.0.0.1 8000 0 0 /bpm - send only\n");
    fprintf(stderr, "    ./cosc 127.0.0.1 0 9000 0 /dummy - receive until Ctrl-C\n");
    fprintf(stderr, "    ./cosc 127.0.0.1 8000 9000 10000 /togglePlaying i 1 - send and receive for 10 seconds or Ctrl-C\n");
    fprintf(stderr, "\n");
    
    exit(0);
}

int
main(int argc, char* argv[])
{
    // TODO Perform better arg handling eg with GNU getopt and factor out of SendPacket
    if (argc < 6 || (argc - 6) % 2 == 1) {
        usage();
    }

    const char* host = argv[1];
    const unsigned short txport = atoi(argv[2]);
    const unsigned short rxport = atoi(argv[3]);
    const unsigned int timeout = atoi(argv[4]);
    const char* path = argv[5];

    // Yay, functional programming is possible using lambdas and std:functional
    // Note cast needed to stop unexpected error about same lambda types being different!

    std::function<void()> send = (txport)
        ? (std::function<void()>)[=]() {
                if (!SendPacket(host, txport, path, argc - 6, &argv[6])) {
                    usage();
                }
            }
        : []() {};
    

    if (rxport) {
        // we pass Send() to Receive() so it can call late to avoid missed responses from OSC server
        // Probably Better to refactor if possible
        ReceivePacket(IpEndpointName::ANY_ADDRESS, rxport, timeout, send);
    }
    else if (txport) {
        send();
    }

    return 0;
}
