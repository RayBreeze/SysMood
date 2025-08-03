#include "network_monitor.h"

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <vector>
#include <iostream>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")

std::vector<NetworkInterfaceStats> getNetworkInterfaceStats() {
    std::vector<NetworkInterfaceStats> stats;
    PMIB_IF_TABLE2 ifTable;
    if (GetIfTable2(&ifTable) == NO_ERROR) {
        for (ULONG i = 0; i < ifTable->NumEntries; i++) {
            MIB_IF_ROW2 row = ifTable->Table[i];
            NetworkInterfaceStats interfaceStats;
            interfaceStats.description = row.Description;
            interfaceStats.bytesSent = row.OutOctets;
            interfaceStats.bytesReceived = row.InOctets;
            interfaceStats.packetErrors = row.InErrors + row.OutErrors;
            interfaceStats.droppedPackets = row.InDiscards + row.OutDiscards;
            stats.push_back(interfaceStats);
        }
        FreeMibTable(ifTable);
    }
    return stats;
}

std::vector<NetworkConnection> getActiveConnections() {
    std::vector<NetworkConnection> connections;
    PMIB_TCPTABLE2 tcpTable;
    if (GetTcpTable2(&tcpTable, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
        for (DWORD i = 0; i < tcpTable->dwNumEntries; i++) {
            MIB_TCPROW2 row = tcpTable->table[i];
            NetworkConnection conn;
            conn.protocol = "TCP";
            char localAddr[INET_ADDRSTRLEN];
            char remoteAddr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &row.dwLocalAddr, localAddr, sizeof(localAddr));
            inet_ntop(AF_INET, &row.dwRemoteAddr, remoteAddr, sizeof(remoteAddr));
            conn.localAddress = localAddr;
            conn.localPort = ntohs(row.dwLocalPort);
            conn.remoteAddress = remoteAddr;
            conn.remotePort = ntohs(row.dwRemotePort);
            switch (row.dwState) {
                case MIB_TCP_STATE_CLOSED: conn.state = "CLOSED"; break;
                case MIB_TCP_STATE_LISTEN: conn.state = "LISTEN"; break;
                case MIB_TCP_STATE_SYN_SENT: conn.state = "SYN_SENT"; break;
                case MIB_TCP_STATE_SYN_RCVD: conn.state = "SYN_RCVD"; break;
                case MIB_TCP_STATE_ESTAB: conn.state = "ESTABLISHED"; break;
                case MIB_TCP_STATE_FIN_WAIT1: conn.state = "FIN_WAIT1"; break;
                case MIB_TCP_STATE_FIN_WAIT2: conn.state = "FIN_WAIT2"; break;
                case MIB_TCP_STATE_CLOSE_WAIT: conn.state = "CLOSE_WAIT"; break;
                case MIB_TCP_STATE_CLOSING: conn.state = "CLOSING"; break;
                case MIB_TCP_STATE_LAST_ACK: conn.state = "LAST_ACK"; break;
                case MIB_TCP_STATE_TIME_WAIT: conn.state = "TIME_WAIT"; break;
                case MIB_TCP_STATE_DELETE_TCB: conn.state = "DELETE_TCB"; break;
                default: conn.state = "UNKNOWN"; break;
            }
            connections.push_back(conn);
        }
        FreeMibTable(tcpTable);
    }
    return connections;
}
#else
#include <fstream>
#include <sstream>
#include <arpa/inet.h>
#include <netinet/in.h> // For in_addr

std::vector<NetworkInterfaceStats> getNetworkInterfaceStats() {
    std::vector<NetworkInterfaceStats> stats;
    std::ifstream file("/proc/net/dev");
    std::string line;
    // Skip first two lines
    std::getline(file, line);
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string iface_name;
        ss >> iface_name;
        if (iface_name.back() == ':') {
            iface_name.pop_back();
        }
        NetworkInterfaceStats s;
        s.name = iface_name;
        ss >> s.bytesReceived >> s.droppedPackets >> s.packetErrors; // Simplified for brevity
        // ... skip some fields
        ss >> s.bytesSent;
        stats.push_back(s);
    }
    return stats;
}

std::vector<NetworkConnection> getActiveConnections() {
    std::vector<NetworkConnection> connections;
    std::ifstream file("/proc/net/tcp");
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int an_int;
        char a_char;
        std::string local_addr_hex, remote_addr_hex, state_hex;
        ss >> an_int >> a_char >> local_addr_hex >> remote_addr_hex >> state_hex;

        NetworkConnection conn;
        conn.protocol = "TCP";

        // Local address
        long local_addr_long = std::stol(local_addr_hex, nullptr, 16);
        struct in_addr local_in_addr;
        local_in_addr.s_addr = local_addr_long;
        conn.localAddress = inet_ntoa(local_in_addr);
        conn.localPort = 0; // Port is also in hex, not parsed for brevity

        // Remote address
        long remote_addr_long = std::stol(remote_addr_hex, nullptr, 16);
        struct in_addr remote_in_addr;
        remote_in_addr.s_addr = remote_addr_long;
        conn.remoteAddress = inet_ntoa(remote_in_addr);
        conn.remotePort = 0;

        // State
        int state_int = std::stoi(state_hex, nullptr, 16);
        switch (state_int) {
            case 1: conn.state = "ESTABLISHED"; break;
            case 2: conn.state = "SYN_SENT"; break;
            case 3: conn.state = "SYN_RECV"; break;
            case 4: conn.state = "FIN_WAIT1"; break;
            case 5: conn.state = "FIN_WAIT2"; break;
            case 6: conn.state = "TIME_WAIT"; break;
            case 7: conn.state = "CLOSE"; break;
            case 8: conn.state = "CLOSE_WAIT"; break;
            case 9: conn.state = "LAST_ACK"; break;
            case 10: conn.state = "LISTEN"; break;
            case 11: conn.state = "CLOSING"; break;
            default: conn.state = "UNKNOWN"; break;
        }

        connections.push_back(conn);
    }
    return connections;
}
#endif