//
// Created by vitor on 02/01/2021.
//

#ifndef ASSAULTCUBETRAINER_PROC_H
#define ASSAULTCUBETRAINER_PROC_H
#include <vector>
#include <windows.h>

DWORD GetProcId(const wchar_t* procName);
uintptr_t GetModuleBaseAddr(DWORD procId, const wchar_t* modName);
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>);


#endif //ASSAULTCUBETRAINER_PROC_H
