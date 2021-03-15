#include <iostream>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include "proc/proc.h"

int main() {
    // Get PROCID of target process
    DWORD procId = GetProcId(L"ac_client.exe");

    // Get module Base Address
    uintptr_t moduleBaseAddr = GetModuleBaseAddr(procId, L"ac_client.exe");

    // Get handle to the process
    HANDLE hProcess = INVALID_HANDLE_VALUE;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Resolve the base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr  = moduleBaseAddr + 0x10F4F4;
    std::cout << "dynamicPtrBaseAddr: 0x" << std::hex << moduleBaseAddr << std::endl;


    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
    uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, { 0xF8 });
    std::cout << "Ammo address: 0x" << std::hex << ammoAddr << std::endl;


    // Read values
    int ammoValue;
    int healthValue;

    ReadProcessMemory(hProcess,(BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
    
    std::cout << "Current Ammo Value: " << std::dec <<ammoValue << std::endl;
    std::cout << "Current Health Value: " << std::dec << healthValue << std::endl;

    while (1) {
      ammoValue = 1337;
      healthValue = 0x2BAD;
      WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
      WriteProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
    }     

    return 0;
}
