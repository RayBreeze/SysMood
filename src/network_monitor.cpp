#include "network_monitor.h"

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netlistmgr.h>
#include <vector>
#include <iostream>
#include <string>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Ws2_32.lib")

std::vector<NetworkInterfaceStats> getNetworkInterfaceStats() {
    std::vector<NetworkInterfaceStats> stats;
    
    // Use older, more compatible API
    PMIB_IFTABLE pIfTable = NULL;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    // Get the required size
    dwRetVal = GetIfTable(pIfTable, &dwSize, 0);
    if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
        pIfTable = (MIB_IFTABLE*)HeapAlloc(GetProcessHeap(), 0, dwSize);
        if (pIfTable != NULL) {
            dwRetVal = GetIfTable(pIfTable, &dwSize, 0);
            if (dwRetVal == NO_ERROR) {
                for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
                    MIB_IFROW row = pIfTable->table[i];
                    NetworkInterfaceStats interfaceStats;
                    interfaceStats.description = reinterpret_cast<const char*>(row.bDescr);
                    interfaceStats.bytesSent = row.dwOutOctets;
                    interfaceStats.bytesReceived = row.dwInOctets;
                    interfaceStats.packetErrors = row.dwInErrors + row.dwOutErrors;
                    interfaceStats.droppedPackets = row.dwInDiscards + row.dwOutDiscards;
                    stats.push_back(interfaceStats);
                }
            }
            HeapFree(GetProcessHeap(), 0, pIfTable);
        }
    }
    return stats;
}

std::vector<NetworkConnection> getActiveConnections() {
    std::vector<NetworkConnection> connections;
    
    // Use older, more compatible API
    PMIB_TCPTABLE pTcpTable = NULL;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    // Get the required size
    dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE);
    if (dwRetVal == ERROR_INSUFFICIENT_BUFFER) {
        pTcpTable = (MIB_TCPTABLE*)HeapAlloc(GetProcessHeap(), 0, dwSize);
        if (pTcpTable != NULL) {
            dwRetVal = GetTcpTable(pTcpTable, &dwSize, TRUE);
            if (dwRetVal == NO_ERROR) {
                for (DWORD i = 0; i < pTcpTable->dwNumEntries; i++) {
                    MIB_TCPROW row = pTcpTable->table[i];
                    NetworkConnection conn;
                    conn.protocol = "TCP";
                    
                    // Convert addresses
                    struct in_addr localAddr, remoteAddr;
                    localAddr.s_addr = row.dwLocalAddr;
                    remoteAddr.s_addr = row.dwRemoteAddr;
                    
                    conn.localAddress = inet_ntoa(localAddr);
                    conn.localPort = ntohs((u_short)row.dwLocalPort);
                    conn.remoteAddress = inet_ntoa(remoteAddr);
                    conn.remotePort = ntohs((u_short)row.dwRemotePort);
                    
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
            }
            HeapFree(GetProcessHeap(), 0, pTcpTable);
        }
    }
    return connections;
}
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