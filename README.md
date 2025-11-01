# FiveM Offset Dumper

A **high-performance, lightweight, and accurate** C++ tool designed to **automatically detect and dump critical FiveM (GTA V modded client) memory offsets** by scanning for unique byte patterns in the running process.

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
- **Colorful console output** with logging system.
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

## Prerequisites

- **Windows 10/11** (64-bit)
- **Visual Studio 2022** (or newer) with **C++ Desktop Development** workload
- **FiveM running** (any build)

---

## Build & Run Instructions

### Step 1: Clone or Download

```bash
git clone https://github.com/yfz/fivem_offset_dumper.git
cd fivem_offset_dumper
```

> Or download as ZIP and extract.

---

### Step 2: Open Solution

1. Open **Visual Studio**
2. Click **"Open a project or solution"**
3. Navigate to the folder and select:
   ```
   fivem_offset_dumper.sln
   ```

---

### Step 3: Build

1. Set configuration to:
   - **Release**
   - **x64**
2. Press **Ctrl + Shift + B** or go to **Build → Build Solution**

> Output: `x64\Release\fivem_offset_dumper.exe`

---

### Step 4: Run

1. **Start FiveM** (join any server or stay in menu)
2. Run the compiled `.exe` **as Administrator** (recommended for stable memory reading)

```cmd
.\x64\Release\fivem_offset_dumper.exe
```

---

## Example

![offsetdumper](https://github.com/user-attachments/assets/45d726d2-03bf-48a4-ad54-c53209c97f33)

> Offsets are **relative to module base** (`FiveM_*.exe`)

---

## Project Structure

```
fivem_offset_dumper/
│
├── main.cpp              → Entry point & pattern logic
├── memory/
│   ├── memory.cpp        → Pattern scanner, RIP resolver
│   └── memory.hpp
├── process/
│   ├── process.cpp       → PID → handle → base address
│   └── process.hpp
├── utils/
│   └── utils.hpp         → Logger with colors
└── fivem_offset_dumper.sln
```

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

## Troubleshooting

| Issue | Solution |
|------|----------|
| `pattern not found` | Build changed → update signature |
| Access denied | Run as **Administrator** |
| Crashes on start | FiveM not fully loaded → wait in menu |
| Wrong offsets | Use **x64 only**, not x86 |

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
