# Klipper_ESP32

Klipper_ESP32 is an experimental wrapper for the Klipper protocol, enabling communication between Espressif ESP32 microcontrollers and the Klipper host software. This project leverages the original Klipper source code and adapts it for ESP32-based 3D printer control.

---

## ⚠️ Important Warnings

- **Experimental Software:** This project is under active development and is not considered stable. Use at your own risk.
- **No Official Support:** Klipper_ESP32 is not officially endorsed by the Klipper project. Do not request support from the main Klipper community.
- **Potential for Hardware Damage:** Incorrect configuration or usage may damage your 3D printer or electronics. Always double-check your wiring and configuration.
- **Backup Your Data:** Before flashing or updating firmware, back up your printer configuration and any important data.

---

## Prerequisites

- **ESP-IDF v5.2.1 or newer:** Install and set up the [ESP-IDF](https://github.com/espressif/esp-idf.git) development environment.
- **Klipper Firmware Source:** The Klipper source is included as a submodule.
- **Supported Hardware:** ESP32-based development boards.
- **Python 3:** Required for the build process and Klipper's build scripts.

## Klipper Submodule Integration

⚠️ **Important:** This project includes a specifically configured version of Klipper as a Git submodule. **Do NOT** download or use Klipper from the main repository. Always use the Klipper version included in this project (`components/klipper/klipper/`) as it has been modified and configured specifically for ESP32 compatibility.

This project includes the complete Klipper firmware as a Git submodule located in `components/klipper/klipper/`. The CMake build system has been specifically designed to:

1. **Extract Compile-Time Requests:** During the build process, the system automatically extracts compile-time requests from object files using a custom Python script (`extract_compile_time_requests.py`)
2. **Generate Build Artifacts:** The CMake configuration automatically creates an `out/` directory within the Klipper submodule (`components/klipper/klipper/out/`) and populates it with:
   - `klipper.bin` - The compiled firmware binary
   - `klipper.elf` - The ELF executable file
   - `compile_time_request.c` - Generated C file containing compile-time configurations
   - `compile_time_request.txt` - Text file with extracted compile-time requests
   - `klipper.dict` - Dictionary file for protocol communication

3. **Automatic Processing:** The build system mimics the original Klipper Makefile behavior by:
   - Scanning all compiled object files for `.compile_time_request` sections
   - Using `objcopy` to extract these sections
   - Running Klipper's `buildcommands.py` script to generate the final configuration files

**Note:** The `out/` folder is automatically created and managed by the CMake build system. You don't need to create it manually, and it will be populated with the necessary files after a successful build.

---

## Build Instructions

1. **Clone the Repository (with Submodules):**
    ```bash
    git clone --recursive https://github.com/nikhil-robinson/klipper_esp32
    cd klipper_esp32
    ```

2. **Set Up ESP-IDF:**
    - Follow the [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) to install prerequisites and export environment variables.
    - Example (on Windows PowerShell):
      ```powershell
      $env:IDF_PATH="C:\path\to\esp-idf"
      . $env:IDF_PATH\export.ps1
      ```

3. **Configure the Project:**
    - You may customize build options using:
      ```bash
      idf.py menuconfig
      ```

4. **Build the Firmware:**
    ```bash
    idf.py build
    ```

5. **Flash the Firmware:**
    - Connect your ESP32 board via USB.
    - Replace `PORT` with your device's COM port (e.g., `COM3`):
      ```bash
      idf.py -p PORT flash
      ```

6. **Configure Klipper:**
    - Edit `printer.cfg` to match your printer's hardware and requirements.
    - Refer to the example configs in `components/klipper/klipper/config/`.

7. **Connect and Test:**
    - Connect your printer hardware to the ESP32.
    - Power on and use your preferred G-code sender or Klipper host to communicate with the board.

---

## Troubleshooting

- **Build Errors:** Ensure ESP-IDF is correctly installed and environment variables are set.
- **Connection Issues:** Verify the correct COM port and USB drivers.
- **Firmware Not Responding:** Double-check wiring, power supply, and configuration files.

---

## Contributing

Contributions are welcome! Please open issues or pull requests for bug reports, feature requests, or improvements.

---

## License

See individual source files for licensing details.

---

## Acknowledgements

- [Klipper](https://www.klipper3d.org/) by Kevin O'Connor (not affiliated).
- [ESP-IDF](https://github.com/espressif/esp-idf) by Espressif.
