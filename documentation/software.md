# Software
The firmware of the ESP32 control unit is used to control the fan speed knob and to provide over-the-air features like automatic updates.

This documentation shows how to build the firmware on your local machine.

## Prerequisites

### PlatformIO
Platform IO is used to build the ESP firmware. PlatformIO is a cross-platform, cross-architecture, multiple framework, professional tool for embedded software development.
The documentation of platform io can be found here: https://docs.platformio.org/en/latest/

In order to build the software you need the PlatformIO CLI installed on your system. The main documentation can be found here: https://docs.platformio.org/en/latest/core/installation.html#piocore-install-shell-commands

Short documentation for Linux and MacOS:

Platformio is a python cli tool. You need python3 to run it and it can be installed using pip:

```
pip install platformio
```

For more complex installation variante (virtual python env or Windows) please refer to the  main documentation (see link above)



### Drivers - MacOS specific
#### ESP32
For the ESP32 uses the CH340 usb2serial chipset no further drivers are needed.

#### ESP8266
In order to acceess a D1 Mini/NodeMCU a USB to serial driver needs to be installed to use the CP210x USB to UART Bridge VCP. It can be found here:
https://www.silabs.com/Support%20Documents/Software/Mac_OSX_VCP_Driver.zip

Further documentation:
* https://cityos-air.readme.io/docs/1-mac-os-usb-drivers-for-nodemcu#13-nodemcu-v10-driver

#### Troubleshooting

After the installation of the drivers, it should be possible for you to connect the device via USB establish a serial connection. When entering

```
ls /dev/cu.*
```

you should see a device that is named something like:

```
/dev/cu.usbserial-0203810B
```

If this is not the case, try the following:

- [Check](https://www.dignited.com/50330/usb-data-cable-vs-usb-charging-cable/) if your USB cable is actually able to transfer data and it's not just a charger cable.
- Follow the instructions listed [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/macos-setup.html) to make sure your system is configured properly.
- If nothing helps, manually install the correct ESP32 driver for your system from [here](https://www.ftdichip.com/Drivers/VCP.htm).

## Building

To build the firmware change in the CleanAir directory and issue the platformio run subcommand:

```
cd CleanAir
platformio run
```

To upload the firmware to your ESP32 issue the following command:

```
platformio run --target upload
```

Please note that you need to have configured the correct serial port to do the initial upload. To get a list of all available ports on your system you can run

```
platformio device list
```

If your upload port is not select automatically you can add the port configuration to your upload command:

```
platformio run --target upload --upload-port <upload port>
```


## Usage




## Further documentation