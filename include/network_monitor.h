#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

#include <string>
#include <vector>
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

std::vector<NetworkInterfaceStats> getNetworkInterfaceStats();
std::vector<NetworkConnection> getActiveConnections();

#endif // NETWORK_MONITOR_H
