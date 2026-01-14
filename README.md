# FiveM Offset Dumper

A **high-performance, lightweight, and accurate** C++ tool designed to **automatically detect and dump critical FiveM memory offsets** by scanning for unique byte patterns in the running process.

> **Author:** `@const`  
> **Language:** C++17  
> **Platform:** Windows (x64)  
> **Target:** FiveM (all builds: `_bXXXX`, Subsurface, etc.)

---

## Features

- **Fully automated** – detects FiveM window, retrieves PID, opens process handle.
- **Robust pattern scanning** with wildcard support (`?` / `??`).
- **RIP-relative address resolution** (handles `lea rax, [rip + disp32]`).
- **Smart pointer dereferencing** for final static addresses.
- **Build detection** from executable name (e.g., `FiveM_b3095.exe` → build `3095`).
- **Modular design** – clean separation of memory, process, and utility logic.

---

## Supported Offsets

| Name                  | Type               | Dereferenced |
|-----------------------|--------------------|------------|
| `world`               | `CWorld*`          | Yes        |
| `replay_interface`    | `CReplayInterface*`| No         |
| `viewport`            | `CViewport*`       | Yes        |
| `blip_list`           | `CBlipList*`       | No         |
| `camera`              | `CCamera*`         | Yes        |
| `bullet`              | `CBulletProof*`    | Yes        |
| `aim_cped`            | `CAimCPed*`        | Yes        |
| `set_ped_in_to_vehicle`| Function ptr     | Yes        |
| `c_sky_settings`      | `CSkySettings*`    | Yes        |

---

### Run

1. **Start FiveM** (join any server or stay in menu)
2. Run the `fivem_offset_dumper.exe`

```cmd
.\x64\Release\fivem_offset_dumper.exe
```

---

## Example

![offsetdumper](https://github.com/user-attachments/assets/87cbad11-81f3-4da2-b0fd-cd1c8cf1d8f7)

> Offsets are **relative to module base** (`FiveM_*.exe`)

---

## Technical Details

### Pattern Scanning
- Uses `VirtualQueryEx` + `ReadProcessMemory`
- Supports `??` and `?` wildcards
- Scans from module base to end of memory regions

### RIP-Relative Resolution
```nasm
lea rax, [rip + disp32]
```
→ `address = instruction + 7 + *(int32_t*)(instruction + 3)`

### Safety
- Only reads `MEM_COMMIT` regions
- Skips `PAGE_GUARD` pages
- Proper handle cleanup

---

## Updating Patterns

Edit the `patterns` array in `main.cpp`:

```cpp
pattern_info patterns[] = {
    {"new_offset", "48 8B 05 ? ? ? ? 48 89 41 08", true},
};
```

> Use tools like **Cheat Engine**, **x64dbg**, or **ReClass** to find new signatures.

---

## Legal & Ethical Note

This tool is for **educational and reverse engineering purposes only**.

- Do **not** use for cheating in online games.
- Respect Rockstar Games & FiveM ToS.
- Use only in **private, offline, or authorized environments**.

---

**Made with precision for the modding community.**  
`@const – 2025`

--- 

*Happy reversing!*
