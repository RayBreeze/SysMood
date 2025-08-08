#include "process_monitor.h"

#ifdef _WIN32
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
                // Proper calculation requires sampling process times at two or more points and computing the difference,
                // e.g., using GetProcessTimes and system tick counts over intervals. See MSDN docs: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes
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

bool terminateProcessById(int pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        return false;
    }
    bool success = TerminateProcess(hProcess, 1);
    CloseHandle(hProcess);
    return success;
}

#else
#include <dirent.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <signal.h>

std::vector<ProcessInfo> getProcessList() {
    std::vector<ProcessInfo> processes;
    DIR* dir = opendir("/proc");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR) {
                try {
                    int pid = std::stoi(entry->d_name);
                    std::ifstream status_file("/proc/" + std::to_string(pid) + "/status");
                    if (status_file) {
                        ProcessInfo p;
                        p.pid = pid;
                        std::string line;
                        while (std::getline(status_file, line)) {
                            if (line.rfind("Name:", 0) == 0) {
                                p.name = line.substr(6);
                            } else if (line.rfind("VmRSS:", 0) == 0) {
                                p.memoryUsage = std::stol(line.substr(7));
                            } else if (line.rfind("Uid:", 0) == 0) {
                                // Getting user name from UID is more involved, so we'll skip it for now.
                                p.user = "N/A";
                            }
                        }
                        p.cpuUsage = 0.0; // Placeholder
                        processes.push_back(p);
                    }
                } catch (const std::invalid_argument& e) {
                    // Not a process directory
                }
            }
        }
        closedir(dir);
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

bool terminateProcessById(int pid) {
    return kill(pid, SIGKILL) == 0;
}

#endif