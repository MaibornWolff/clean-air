# Documentation

The documentation covers all information around the UV-C clean air units.

## DIY Hardware

### Calculations

Estimations about the irradiance and considerations about the room size for which such a device could be feasible are located [here](theory.pdf).

### 3D printable device

The small 3D printable model is a UV-C cleaner with up to 4 lamps and up to 8 fans. Based on our calculations the small device is capable of handling rooms with a size of up to 25,3m² (assuming 2.5m ceiling height), using 4x 55W UV-C lamps and fans with a total throughput of 380m³/h.

See [3D model documentation](3dmodel.md).

### Wooden device

The wooden device is a full sized DIY case, specced with 6 UV-C lamps and 4 fans.

See [wooden model documentation](allWoodModel.md).

### Electronics

- GPIO Pin 33 is to control the main fans via PWM signal.
- GPIO Pin 26 is to control the auxiliary fan via PWM signal.
- GPIO Pin 23 is to read the temperature coming from the temp sensor.
- GPIO Pin 21 connects to the "clock" of the rotary encoder (if in wrong dir, needs to get swapped with wire from Pin 17)
- GPIO Pin 17 connects to the "DT" of the rotary encoder (if in wrong dir, needs to get swapped with wire from Pin 21)
- GPIO Pin 16 connects to the "SW" of the rotary encoder.

Wire up the low voltage part like that:

![Low voltage schematics](/hardware/DC_schematics.png)

The high voltage part is more dependent on the inverter as well as the plugs you use. On top of that it is more dangerous to work with 230V. If you do not know what you are doing you might rather want to look for professional help! Due to that a schematic for the high voltage part will not be provided. The individual recommended parts can be found within the specific devices documentation.

## Software

Use a tool like e.g. [ESPHome-Flasher](https://github.com/esphome/esphome-flasher) to flash the latest release onto your Wemos. If you'd like to build the software yourself, go to the Building section.

### Building

See [building documentation](software.md).
