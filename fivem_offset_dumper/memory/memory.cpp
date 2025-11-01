#include "memory.hpp"
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <cstdint>

struct WildcardByte {
    bool is_wildcard;
    uint8_t value;
    WildcardByte() : is_wildcard(true), value(0) {}
    WildcardByte(uint8_t v) : is_wildcard(false), value(v) {}
};

namespace memory {
    static std::vector<WildcardByte> build_signature(const std::string& input) {
        std::vector<WildcardByte> sig;
        std::istringstream parser(input);
        std::string part;
        while (parser >> part) {
            if (part == "?" || part == "??") {
                sig.emplace_back();
            }
            else {
                sig.emplace_back(static_cast<uint8_t>(std::stoi(part, nullptr, 16)));
            }
        }
        return sig;
    }

    static uintptr_t find_in_region(HANDLE proc, const MEMORY_BASIC_INFORMATION& region, const std::vector<WildcardByte>& sig) {
        if (sig.empty()) return 0;
        std::vector<uint8_t> data(region.RegionSize);
        SIZE_T read_count = 0;
        if (!ReadProcessMemory(proc, region.BaseAddress, data.data(), region.RegionSize, &read_count) || read_count == 0) return 0;
        const size_t sig_len = sig.size();
        const size_t max_offset = read_count - sig_len;
        for (size_t pos = 0; pos <= max_offset; ++pos) {
            bool matched = true;
            for (size_t i = 0; i < sig_len; ++i) {
                if (!sig[i].is_wildcard && sig[i].value != data[pos + i]) {
                    matched = false;
                    break;
                }
            }
            if (matched) return reinterpret_cast<uintptr_t>(region.BaseAddress) + pos;
        }
        return 0;
    }

    uintptr_t pattern_scan(HANDLE process_handle, uintptr_t base_address, const std::string& pattern) {
        if (!process_handle || pattern.empty()) return 0;
        auto signature = build_signature(pattern);
        if (signature.empty()) return 0;
        MEMORY_BASIC_INFORMATION info = {};
        uintptr_t cursor = base_address;
        while (VirtualQueryEx(process_handle, reinterpret_cast<LPCVOID>(cursor), &info, sizeof(info))) {
            if (info.State == MEM_COMMIT && (info.Protect & PAGE_GUARD) == 0 && info.RegionSize > 0) {
                uintptr_t result = find_in_region(process_handle, info, signature);
                if (result != 0) return result;
            }
            cursor = reinterpret_cast<uintptr_t>(info.BaseAddress) + info.RegionSize;
        }
        return 0;
    }

    uintptr_t get_rip_relative_address(HANDLE process_handle, uintptr_t instruction_address) {
        if (!process_handle || !instruction_address) return 0;
        int32_t offset = 0;
        SIZE_T bytes_read = 0;
        if (!ReadProcessMemory(process_handle, reinterpret_cast<LPCVOID>(instruction_address + 3), &offset, sizeof(offset), &bytes_read) ||
            bytes_read != sizeof(offset)) return 0;
        return instruction_address + 7 + offset;
    }

    uintptr_t dereference_pointer(HANDLE process_handle, uintptr_t address) {
        if (!process_handle || !address) return 0;
        uintptr_t target = 0;
        SIZE_T bytes_read = 0;
        if (!ReadProcessMemory(process_handle, reinterpret_cast<LPCVOID>(address), &target, sizeof(target), &bytes_read) ||
            bytes_read != sizeof(target)) return 0;
        return target;
    }
}