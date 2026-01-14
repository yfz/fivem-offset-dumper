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

![offsetdumper](https://private-user-images.githubusercontent.com/77736474/508559576-5a14d26e-63d9-4fa5-9210-c4ff30271927.gif?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3Njg0MDk4MTcsIm5iZiI6MTc2ODQwOTUxNywicGF0aCI6Ii83NzczNjQ3NC81MDg1NTk1NzYtNWExNGQyNmUtNjNkOS00ZmE1LTkyMTAtYzRmZjMwMjcxOTI3LmdpZj9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNjAxMTQlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjYwMTE0VDE2NTE1N1omWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTgxMDcwZmYwNWNjNjcxZWUwYzMyZTBkM2Y5ZTBmMzZlODhlZTcwNWZmMWNjZmJmYzBiYTUyYTIwNDdiMTczYzImWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.qH9j0OvltzXWzp513qeSLkTXi0k9Ehj_RO_EfqHxr0Q)

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