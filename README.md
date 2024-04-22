# Klipper_ESP32

Klipper_ESP32 is an implementation of the Klipper protocol for Espressif's ESP32 microcontroller set.

## Important Notice

Klipper_ESP32 acts as a wrapper on the original Klipper protocol from the master branch of the Klipper repository, utilizing the underlying Klipper source to facilitate communication between ESP32-based chips and the Klipper software. Please note that this project is currently experimental and not stable. We are not responsible for any damages caused by using this framework.

## Installation and Usage

### Prerequisites

Before using Klipper_ESP32, ensure you have the following prerequisites installed:

- [ESP-IDF >= 5.2.1](https://github.com/espressif/esp-idf.git) development environment set up.
- Klipper firmware source code.

### Setup Steps

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/nikhil-robinson/klipper_esp32
    ```

2. **Navigate to the Project Directory:**

    ```bash
    cd klipper-esp32
    ```

3. **Configure ESP-IDF:**

    Follow the instructions in the ESP-IDF documentation to set up your environment.

4. **Build the Firmware:**

    ```bash
    idf.py build
    ```

5. **Flash the Firmware:**

    Connect your ESP32 board and run:

    ```bash
    idf.py -p PORT flash
    ```

    Replace `PORT` with the port your ESP32 is connected to.

6. **Configure Klipper Settings:**

    Configure your 3D printer settings in the `printer.cfg` file.

7. **Connect and Start Printing:**

    Connect your 3D printer hardware to the ESP32 board, power on your printer, and start sending G-code commands to Klipper running on ESP32.

## Contributing

Contributions to Klipper_ESP32 are welcome! If you have suggestions for improvements or new features, feel free to open an issue or submit a pull request.

## Licensing

Please refer to the individual source files within the klipper _esp32 for detailed licensing information.

## Acknowledgements

This project is not officially endorsed by the Klipper project. Please refrain from directing any support requests to the Klipper project.

- [Klipper](https://www.klipper3d.org/) by [Kevin O'Connor](https://www.patreon.com/koconnor)
