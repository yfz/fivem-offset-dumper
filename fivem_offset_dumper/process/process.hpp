#pragma once
#include <windows.h>
#include <string>

namespace process {
    /**
     * @brief Retrieves the base address of the main module in the specified process.
     * @param pid Process ID.
     * @return Base address of the primary module (usually the executable), or 0 on failure.
     */
    uintptr_t get_base_address(DWORD pid);

    /**
     * @brief Gets the name of the main executable module for the given process.
     * @param pid Process ID.
     * @return Name of the executable (e.g., "game.exe"), or empty string if failed.
     */
    std::string get_name(DWORD pid);
}