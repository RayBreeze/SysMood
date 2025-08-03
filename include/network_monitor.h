#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
struct NetworkInterfaceStats {
    std::wstring description;
    ULONG64 bytesSent;
    ULONG64 bytesReceived;
    ULONG packetErrors;
    ULONG droppedPackets;
};

struct NetworkConnection {
    std::string protocol;
    std::string localAddress;
    int localPort;
    std::string remoteAddress;
    int remotePort;
    std::string state;
};
#else
struct NetworkInterfaceStats {
    std::string name;
    unsigned long long bytesSent;
    unsigned long long bytesReceived;
    unsigned long packetErrors;
    unsigned long droppedPackets;
};

struct NetworkConnection {
    std::string protocol;
    std::string localAddress;
    int localPort;
    std::string remoteAddress;
    int remotePort;
    std::string state;
};
#endif

std::vector<NetworkInterfaceStats> getNetworkInterfaceStats();
std::vector<NetworkConnection> getActiveConnections();

#endif // NETWORK_MONITOR_H