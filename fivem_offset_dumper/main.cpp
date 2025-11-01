#include <iostream>
#include <thread>
#include <sstream>
#include "process/process.hpp"
#include "memory/memory.hpp"
#include "utils/utils.hpp"

struct pattern_info {
    const char* name;
    const char* signature;
    bool should_dereference;
};

int main() {
    SetConsoleTitleA("Fivem Offset Dumper - @const");
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, 550, 300, TRUE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    const int padding = 7;
    std::cout << std::string(padding, ' ');
    std::cout << "\033[94mFivem offset dumper - @const\033[0m" << std::endl;
    std::cout << "" << std::endl;

    logger::log("Awaiting for FiveM...", logger::info);
    HWND window_handle = nullptr;
    while (!(window_handle = FindWindowA("grcWindow", NULL))) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    DWORD pid;
    GetWindowThreadProcessId(window_handle, &pid);
    if (!pid) return 1;

    std::string process_name = process::get_name(pid);
    int build = 0;
    size_t b_pos = process_name.find("_b");

    if (b_pos != std::string::npos) {
        std::string build_str = process_name.substr(b_pos + 2);
        try {
            build = std::stoi(build_str);
            logger::log("FiveM found! (" + std::to_string(build) + ")", logger::success);
        } catch (...) {
            logger::log("FiveM found! (Build unknown)", logger::success);
        }
    } else {
        logger::log("FiveM found! (" + process_name + ")", logger::success);
    }

    HANDLE process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!process_handle) return 1;

    uintptr_t base_address = process::get_base_address(pid);
    if (!base_address) {
        CloseHandle(process_handle);
        return 1;
    }

    pattern_info patterns[] = {
        {"world", "48 8B 05 ? ? ? ? 33 D2 48 8B 40 08 8A CA 48 85 C0 74 16 48 8B", true},
        {"replay_interface", "48 8D 0D ? ? ? ? 48 ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 8A D8 E8 ? ? ? ? 84 DB 75 13 48 8D 0D ? ? ? ? 48 8B D7 E8 ? ? ? ? 84 C0 74 BC 8B 8F", false},
        {"viewport", "48 8B 15 ? ? ? ? 48 8D 2D ? ? ? ? 48 8B CD", true},
        {"blip_list", "4C 8D 05 ? ? ? ? 0F B7 C1", false},
        {"camera", "4C 8B 35 ? ? ? ? 33 FF 32 DB", true}, //48 8B 05 ? ? ? ? 38 98 ? ? ? ? 8A C3
        {"bullet", "F3 41 0F 10 19 F3 41 0F 10 41 04", true},
        {"aim_cped", "48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 05 E8 ? ? ? ? 8A CB", true },
        {"set_ped_in_to_vehicle", "48 8B C4 44 89 48 ? 44 89 40 ? 48 89 50 ? 48 89 48 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 83 BA", true}, //48 8B C4 48 89
        {"c_sky_settings", "48 8D 0D ? ? ? ? E8 ? ? ? ? 83 25 ? ? ? ? 00 48 8D 0D ? ? ? ? F3", true }
    };

    for (const auto& pattern : patterns) {
        uintptr_t instruction_address = memory::pattern_scan(process_handle, base_address, pattern.signature);
        if (!instruction_address) {
            logger::log(std::string(pattern.name) + ": pattern not found", logger::fail);
            continue;
        }

        uintptr_t resolved_address = memory::get_rip_relative_address(process_handle, instruction_address);
        uintptr_t offset_fix = resolved_address - base_address;

        uintptr_t final_address = pattern.should_dereference ? memory::dereference_pointer(process_handle, resolved_address) : resolved_address;

        std::stringstream ss;
        ss << std::hex << offset_fix;
        logger::log(std::string(pattern.name) + ": 0x" + ss.str(), logger::success);
    }

    CloseHandle(process_handle);
    std::cin.get();
    return 0;
}