# Octavia_RTD_Module
# Single-Channel RTD Input Module Firmware

## Overview
This repository contains the firmware (`rtd_module.c`) for the Single-Channel RTD Input Module, submitted for the Octavia Carbon I&C Technical Challenge. The module measures temperature using Pt100 or Pt1000 RTDs in a 4-wire configuration, with the ADS1243-HT ADC providing SPI output. The firmware initializes SPI, reads raw data, converts it to temperature, and supports hardware-based RTD type selection via a jumper.

## Schematic Integration
The firmware interfaces with the schematic (`Octavia_RTD_Module.kicad_sch`):
- **SPI Interface**: Connects to the ADS1243-HT (U3) via J1:
  - `CS_PIN` (GPIO 10) → J1 pin 4 (`SPI_CS`).
  - SPI pins (MOSI, MISO, SCLK) → J1 pins 2, 1, 3.
- **RTD Type Selection**: Reads the J3 jumper state:
  - `RTD_TYPE_PIN` (GPIO 9) → J3 pin 1 (`RTD_TYPE`).
  - Logic 0 (jumper closed) = Pt100, Logic 1 (jumper open) = Pt1000.
- **Measurement Chain**: Processes data from RTD1 → INA128 (U2) → ADS1243-HT (U3).

## Code Structure
- **`setup()`**: Initializes SPI communication with the ADS1243-HT (Mode 1, ~1 MHz clock) and sets up the RTD type pin.
- **`read_adc()`**: Reads 24-bit raw data from the ADS1243-HT via SPI.
- **`rtd_to_temp(raw, is_pt100)`**: Converts raw ADC data to temperature using the Callendar-Van Dusen equation, adjusted for Pt100 (R0=100Ω) or Pt1000 (R0=1000Ω).
- **`get_rtd_type()`**: Reads the J3 jumper state to determine the RTD type.
- **`loop()`**: Reads data, determines the RTD type, converts to temperature, and outputs via Serial.

## Usage
1. **Hardware Setup**:
   - Connect an Arduino-compatible microcontroller (e.g., Arduino Uno) to the RTD module via J1 (SPI connector).
   - Ensure J3 is set (closed for Pt100, open for Pt1000).
2. **Software Setup**:
   - Upload `rtd_module.c` to your Arduino IDE.
   - Compile and upload to the microcontroller.
3. **Output**:
   - Open the Serial Monitor (9600 baud) to view temperature readings every second.

## Dependencies
- Arduino SPI library (`#include <SPI.h>`).

## Notes
- Assumes a 2V reference voltage (ADS1243-HT VREFP) and 0.2mA excitation current (set by OPA277).
- The temperature conversion uses a linear approximation (alpha = 3850 ppm/°C). For higher accuracy, add higher-order Callendar-Van Dusen terms.
- Tested with a 16 MHz Arduino; adjust `SPI.setClockDivider()` for other clock speeds.

## License
This project is submitted for the Octavia Carbon I&C Technical Challenge and is not licensed for external use.
