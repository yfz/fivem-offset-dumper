#pragma once
#include <windows.h>
#include <cstdint>
#include <string>

namespace memory {
    /**
     * @brief Scans the target process for a byte pattern starting from `base_address`.
     *        Wildcards are represented by "?" or "??". Example: "48 8B 05 ? ? ? ?".
     * @param process_handle Handle to the target process (must have read access).
     * @param base_address   Starting address for the scan (usually module base).
     * @param pattern        Space-separated hex string with optional wildcards.
     * @return Address of the first match, or 0 if not found.
     */
    uintptr_t pattern_scan(HANDLE process_handle, uintptr_t base_address, const std::string& pattern);

    /**
     * @brief Resolves a RIP-relative address (x64).
     *        Used for instructions like `lea rax, [rip + disp32]`.
     *        The displacement is read 3 bytes after `instruction_address`.
     *        Final address = instruction_address + 7 + disp32.
     * @param process_handle       Handle to the target process.
     * @param instruction_address  Address of the instruction.
     * @return Resolved absolute address, or 0 on failure.
     */
    uintptr_t get_rip_relative_address(HANDLE process_handle, uintptr_t instruction_address);

    /**
     * @brief Reads a 64-bit pointer from the target process.
     * @param process_handle  Handle to the target process.
     * @param address         Address containing the pointer.
     * @return Value of the pointer, or 0 if read failed.
     */
    uintptr_t dereference_pointer(HANDLE process_handle, uintptr_t address);
}