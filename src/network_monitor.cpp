#include "network_monitor.h"
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
