## Documentation

The printing time for the 3D Printable Model is about 85 hours (with 0.4mm nozzle, 0.2mm layer height)
cumulated, and you'll need about 1,5kg of PETG or ASA. You should not use PLA since it is not UV resistant. In
total, the 3D-printable device will cost you about 175€ excl. the print material, the electrical bill and your time.
The UV-C aircleaner can also be found on [thingiverse](https://www.thingiverse.com/thing:4649824)

### Used Hardware

For the .stl files please check the folders within this directory. The also contain a file "required_parts.txt" which
describes the amount required of each part. All other non-3D-printable parts are listed within this document.

#### List of common parts with links:

| #   | Description                 | Note                                                                    | Link                                                                                                                                                                                                                                                          | Price in €  |
| --- | --------------------------- | ----------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------- |
| 1/2 | Inverter 55W                | can be adjusted according to wattage of lamps                           | [amazon.de](https://www.amazon.de/gp/product/B00I0QUFR4/)                                                                                                                                                                                                     | 13,54       |
| 1   | 12V PSU                     |                                                                         | [amazon.de](https://www.amazon.de/gp/product/B00A9UP8P8/)                                                                                                                                                                                                     | 17,99       |
| 1/2 | UVC Mount                   |                                                                         | [amazon.de](https://www.amazon.de/gp/product/B0018LGB4E/)                                                                                                                                                                                                     | 4,96        |
| 2/4 | UVC Lamp 55W                | can be adjusted to up to 95W                                            | [amazon.de](https://www.amazon.de/gp/product/B002VLGJOW/)                                                                                                                                                                                                     | 19,13       |
| 1   | 5V PSU                      | Optional, a 12v to 5v converter can also be used                        | [amazon.de](https://www.amazon.de/gp/product/B079138QY1/)                                                                                                                                                                                                     | 8,46        |
| 1   | AC Plug with fuse           |                                                                         | [amazon.de](https://www.amazon.de/gp/product/B01FQVM9EM/)                                                                                                                                                                                                     | 6,99        |
| 4/8 | 12V 92mm PWM Fan            | either use only 4 in the front or 4 in the front and 4 in the back      | [amazon.de](https://www.amazon.de/ARCTIC-F9-PWM-Standardgeh%C3%A4use-L%C3%BCfterdrehzahl/dp/B00H3SVWF4/) / [mindfactory.de](https://www.mindfactory.de/product_info.php/Titan-TFD-9225H12ZP-KE-RB--92x92x25mm-2700-U-min-10-25-dB-A--schwarz-we_1030365.html) | 3,60 / 9.02 |
| 1   | ESP32 Mini                  |                                                                         | [amazon.de](https://www.amazon.de/AZDelivery-Bluetooth-Internet-Entwicklungsboard-kompatibel/dp/B08BTRQNB3/)                                                                                                                                                  | 8,99        |
| 1   | Rotary Knob KY-040          |                                                                         | [amazon.de](https://www.amazon.de/WayinTop-Encoder-Drehgeber-Drehwinkelgeber-Druckknopf/dp/B07T3672VK/)                                                                                                                                                       | 9,99        |
| -   | M3 Screws 15mm              | use a lot                                                               | -                                                                                                                                                                                                                                                             | -           |
| -   | M3 Screws 30mm              | use a few                                                               | -                                                                                                                                                                                                                                                             | -           |
| -   | Wood Screws 15mm            | use a lot                                                               | -                                                                                                                                                                                                                                                             | -           |
| 1   | Aluminum self adhesive tape | A roll is enough to build one hybrid wooden device                      | [amazon.de](https://www.amazon.de/gp/product/B08C5JPV12)                                                                                                                                                                                                      | 10,00       |
| -   | Cables                      | use a lot and in different variations, ref to the specs of the inverter | -                                                                                                                                                                                                                                                             | -           |

#### 3D Printable Aluminum Hybrid Model:

| #   | Description              | Link                                                                                                                        | Price in € |
| --- | ------------------------ | --------------------------------------------------------------------------------------------------------------------------- | ---------- |
| 4   | Aluminum L Profile       | [ebay.de](https://www.ebay.de/itm/Aluminium-Winkel-Alu-L-Profil-Alu-Schiene-Aluprofil-Winkelprofil-Aluwinkel/282319979439)  | 2,42       |
| 4   | Aluminum Plate 600x200mm | [ebay.de](https://www.ebay.de/itm/0-5-4mm-Aluminium-Blech-Platte-Alublech-Aluplatte-Feinblech-mit-Schutzfolie/264476529936) | 18,45      |

#### 3D Printable Wood Hybrid Model:

| #   | Description                 | Link | Price in € |
| --- | --------------------------- | ---- | ---------- |
| 2   | Wooden Plate 600x230mmx10mm | -    | 5,00       |
| 2   | Wooden Plate 600x210mmx10mm | -    | 5,00       |

#### Parts used from to other sources which need to be printed:

| #   | Description | Link                                                         |
| --- | ----------- | ------------------------------------------------------------ |
| 1   | Knob        | [thingiverse.com](https://www.thingiverse.com/thing:1465938) |
| 1   | Handle      | [thingiverse.com](https://www.thingiverse.com/thing:4619895) |

#### What you'll need in addition to that

- An (electric) screwdriver
- Wood/Metal drills (and electric screwdriver/o.e.)

### How to build it

### Electronic Setup

Wire up the low voltage part like that:

![Low voltage schematics](/hardware/DC_schematics.png)

### ESP Control Unit
