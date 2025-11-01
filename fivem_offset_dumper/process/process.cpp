#include "process.hpp"
#include <TlHelp32.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

namespace process {
    static HANDLE open_process_handle(DWORD pid) {
        return OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    }

    static bool enum_modules(HANDLE hProc, HMODULE* modules, DWORD max_bytes, DWORD* bytes_needed) {
        return EnumProcessModules(hProc, modules, max_bytes, bytes_needed) != 0;
    }

    static bool get_module_name(HANDLE hProc, HMODULE hMod, char* buffer, DWORD buffer_size) {
        return GetModuleBaseNameA(hProc, hMod, buffer, buffer_size) > 0;
    }

    uintptr_t get_base_address(DWORD pid) {
        HANDLE hProc = open_process_handle(pid);
        if (!hProc) return 0;

        HMODULE modules[1024];
        DWORD needed = 0;

        if (enum_modules(hProc, modules, sizeof(modules), &needed) && needed >= sizeof(HMODULE)) {
            CloseHandle(hProc);
            return reinterpret_cast<uintptr_t>(modules[0]);
        }

        CloseHandle(hProc);
        return 0;
    }

    std::string get_name(DWORD pid) {
        HANDLE hProc = open_process_handle(pid);
        if (!hProc) return {};

        HMODULE first_module = nullptr;
        DWORD needed = 0;
        char name_buffer[MAX_PATH] = { 0 };

        if (enum_modules(hProc, &first_module, sizeof(first_module), &needed)) {
            get_module_name(hProc, first_module, name_buffer, sizeof(name_buffer));
        }

        CloseHandle(hProc);
        return std::string(name_buffer);
    }
}