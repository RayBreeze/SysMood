#include "process_monitor.h"
#include <windows.h>
#include <psapi.h>
#include <algorithm>
#include <iostream>
#include <vector>

// Helper function to get user name from SID
std::wstring getUserForProcess(HANDLE hProcess) {
    DWORD dwSize = 0;
    HANDLE hToken = NULL;
    PTOKEN_USER ptu = NULL;
    std::wstring user = L"N/A";

    if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
        GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
        ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
        if (ptu && GetTokenInformation(hToken, TokenUser, ptu, dwSize, &dwSize)) {
            wchar_t szName[256];
            wchar_t szDomain[256];
            DWORD cchName = 256;
            DWORD cchDomain = 256;
            SID_NAME_USE eUse;
            if (LookupAccountSidW(NULL, ptu->User.Sid, szName, &cchName, szDomain, &cchDomain, &eUse)) {
                user = std::wstring(szDomain) + L"\\" + std::wstring(szName);
            }
        }
        if (ptu) HeapFree(GetProcessHeap(), 0, ptu);
        CloseHandle(hToken);
    }
    return user;
}

std::vector<ProcessInfo> getProcessList() {
    std::vector<ProcessInfo> processes;
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        return processes;
    }
    cProcesses = cbNeeded / sizeof(DWORD);

    for (unsigned int i = 0; i < cProcesses; i++) {
        if (aProcesses[i] != 0) {
            ProcessInfo pi;
            pi.pid = aProcesses[i];

            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pi.pid);
            if (NULL != hProcess) {
                HMODULE hMod;
                DWORD cbNeeded;
                wchar_t szProcessName[MAX_PATH] = L"<unknown>";

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    GetModuleBaseNameW(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(wchar_t));
                }
                pi.name = szProcessName;

                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    pi.memoryUsage = pmc.WorkingSetSize;
                }

                pi.user = getUserForProcess(hProcess);

                // CPU usage is complex to calculate accurately for a single snapshot.
                // This is a placeholder.
                pi.cpuUsage = 0.0;

                processes.push_back(pi);
                CloseHandle(hProcess);
            }
        }
    }
    return processes;
}

void sortProcesses(std::vector<ProcessInfo>& processes, const std::string& sortBy) {
    if (sortBy == "pid") {
        std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.pid < b.pid;
        });
    } else if (sortBy == "memory") {
        std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.memoryUsage > b.memoryUsage;
        });
    } else if (sortBy == "name") {
        std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.name < b.name;
        });
    }
}

bool terminateProcessById(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    bool success = TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);
    return success;
}
