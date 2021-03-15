//
// Created by vitor on 02/01/2021.
//

#include "proc.h"
#include <tlhelp32.h>

#include <stdio.h>
#include <iostream>
#include <string>

#ifndef TH32CS_SNAPNOHEAPS
// define missing in Tlhelp32.h, but needed
#define TH32CS_SNAPNOHEAPS 0x40000000
#endif

DWORD GetProcId(const wchar_t* procName) {
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);
        if (Process32First(hSnap, &procEntry)) {
            do {
                if(!_wcsicmp(procEntry.szExeFile, procName)) {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while(Process32Next(hSnap, &procEntry));
        }
    }

    CloseHandle(hSnap);
    if (procId != 0) {
        printf("Found processId for %ws: %lu\n", procName, procId);
    } else {
        printf("Process not found %ws\n", procName);
    }
    return procId;
}

uintptr_t GetModuleBaseAddr(DWORD procId, const wchar_t* modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = INVALID_HANDLE_VALUE;

    DWORD dwFlags = TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32;

    hSnap = CreateToolhelp32Snapshot(dwFlags, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry = { 0 };
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, modName)) {
                    modBaseAddr = (uintptr_t) modEntry.modBaseAddr;
                    break;
                }
            } while(Module32Next(hSnap, &modEntry));
        }
    }

    CloseHandle(hSnap);
    return modBaseAddr;
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
    uintptr_t addr = ptr;

    for (unsigned int i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }

    return addr;
}