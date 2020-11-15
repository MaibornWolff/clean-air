## Instructions for Linux

- Install platformio.

- Install (udev rules)[https://docs.platformio.org/en/latest/faq.html#platformio-udev-rules].

- Add yourself to the dialout and tty group:

```sh
sudo usermod -a -G tty $USER
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
```

Disconnect your board, then log out and log in again for the changes to take place. 

Check if your ESP32 is connected:

```sh
ll /dev | grep ttyUSB*
```

You should then be able to flash the code onto your board by running

```sh
platformio run --target upload --target monitor
```

Afterwards, an access point should spawn (namend ```CleanAirDevice```). The password is ```12345678```. Afterwards, the configuration page should be reachable under e.g. ```http://172.217.28.1/_ac``` (you need to fill in the correct IP address, of course).