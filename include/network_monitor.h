#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

// Written by: Anderson Yu-Hong Cai (https://github.com/AndersonTsaiTW)
// Written on: 2025-10-11
// License: MIT
// Version: 2.0
//
#include <cstdint>

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0602
#endif

#include <windows.h>
#include <iphlpapi.h>
#include <set>
#include <stdlib.h>

// Note: linking still required for iphlpapi when using g++: -liphlpapi

static bool collect_if_counters(uint64_t &out_sent, uint64_t &out_recv) {
    out_sent = 0;
    out_recv = 0;

    // Fallback to GetIfTable which is widely available (MinGW). We'll
    // allocate the table buffer dynamically as required.
    MIB_IFTABLE *pIfTable = NULL;
    DWORD dwSize = 0;
    DWORD dwRetVal = GetIfTable(NULL, &dwSize, FALSE);
    if (dwRetVal != ERROR_INSUFFICIENT_BUFFER) {
        return false;
    }

    pIfTable = (MIB_IFTABLE*)malloc(dwSize);
    if (pIfTable == NULL) return false;

    dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE);
    if (dwRetVal != NO_ERROR) {
        free(pIfTable);
        return false;
    }

    std::set<DWORD> seenIndexes;
    for (DWORD i = 0; i < pIfTable->dwNumEntries; ++i) {
        MIB_IFROW &row = pIfTable->table[i];

        // De-duplicate by dwIndex (if available). Some stacks list
        // virtual/alias rows that may duplicate counters.
        if (seenIndexes.find(row.dwIndex) != seenIndexes.end()) continue;
        seenIndexes.insert(row.dwIndex);

        // Heuristic: include an interface if it shows any octets OR has a
        // non-zero oper status. This avoids summing unused loopback/host
        // entries while still counting active interfaces.
        uint64_t outOct = (uint64_t)row.dwOutOctets;
        uint64_t inOct = (uint64_t)row.dwInOctets;
        if (outOct == 0 && inOct == 0 && row.dwOperStatus == 0) {
            continue;
        }

    out_sent += outOct;
    out_recv += inOct;
    }

    free(pIfTable);
    return true;
}

// Helper: compute difference with 64-bit wrap-around handling
static inline uint64_t counter_diff(uint64_t end, uint64_t start) {
    if (end >= start) return end - start;
    // wrapped
    return (UINT64_MAX - start) + end + 1ULL;
}

// Public API: samples counters for ~1s and returns KB/s
static void get_network_stats(double &sent_rate_kbps, double &received_rate_kbps) {
    uint64_t sent0 = 0, recv0 = 0;
    uint64_t sent1 = 0, recv1 = 0;

    if (!collect_if_counters(sent0, recv0)) {
        sent_rate_kbps = 0.0;
        received_rate_kbps = 0.0;
        return;
    }

    ULONGLONG t0 = GetTickCount64();
    Sleep(3000); // 3-second interval

    if (!collect_if_counters(sent1, recv1)) {
        sent_rate_kbps = 0.0;
        received_rate_kbps = 0.0;
        return;
    }

    ULONGLONG t1 = GetTickCount64();

    double elapsed_s = (double)(t1 - t0) / 1000.0;
    if (elapsed_s <= 0.0) elapsed_s = 1.0; // safety

    uint64_t ds = counter_diff(sent1, sent0);
    uint64_t dr = counter_diff(recv1, recv0);

    // convert to KB/s
    sent_rate_kbps = ((double)ds / 1024.0) / elapsed_s;
    received_rate_kbps = ((double)dr / 1024.0) / elapsed_s;
}

#else
// Non-Windows platform: provide stub implementation
static void get_network_stats(double &sent_rate_kbps, double &received_rate_kbps) {
    // TODO: Implement network monitoring for Linux/macOS
    sent_rate_kbps = 0.0;
    received_rate_kbps = 0.0;
}
#endif // _WIN32

#endif // NETWORK_MONITOR_H