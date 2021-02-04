# Custom electronics design

All sensors and actors need to be connected to the ESP32, some require additional resistors or power supplies. To simplify the wiring and safe build space, a custom pcb board does all the pin swaps and routing needed to connect everything to the ESP32. 
Features:
- ESP32 
- 2 Fan outputs
- 1 ds18b20 temperature Sensor input
- 1 Rotary Encoder connector
- 8 ws2812b adressable RGB LEDs
- 12V - 5V DC/DC converter
- 3 IO Pins 

PowerIn: 12V


## Production
The Gerber files and BOM/CPL can be found here //insert link. Manufacturing is possible including SMT Placement.
## Using the board

### Housing
The board can be mounted using 4 M3 screws in the 3d printed housing, which is then mounted flush to the surface of the clean-air device. The housing consist of two materials, one dark, absorbing main body and 8 diffuse surfaces, wthe neopixels light is channeled to for a clear visibility.

