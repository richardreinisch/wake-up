# Wake-Up

After many years of traveling and enjoying good sleep—even with jet lag—thanks to the [aXbo](https://www.axbo.at/pages/home), an Austrian sleep phase alarm clock, I wanted to enhance my aXbo by using my own preferred MP3 music as an alarm sound.

> **Note:** aXbo detects when you're in a light sleep phase and wakes you up at the optimal time. This product has been one of the most important tools in my life. I don't know how I would have managed waking up in good condition in different countries around the world (e.g. after 24h travel) without my aXbo.

![Wake-Up setup](wake-up.png "Wake-Up setup")

This project utilizes the [ArduinoSerialMP3Player](https://github.com/salvadorrueda/ArduinoSerialMP3Player) library by Salvador Rueda.

---

## How It Works

1. **Detecting sound output**: The system monitors the aXbo's speaker output via analog input `A0` to detect when the wake-up music starts playing.
2. **Playing MP3 files**: An external MP3 board receives a command to play music through a mini speaker.
3. **User interaction**: The music repeats until the user presses a button connected to `D2`.
4. **Optional shutter control**: A HTTP request can be sent to FHEM to automatically open the shutters.

---

## Setup & Requirements

### Hardware Components

- **MP3 board** (configured accordingly)
- **Optocoupler & switch** for signal detection
- **ESP8266** for control and communication
- **Serial connection** to the MP3 board
- **MicroSD card** with MP3 file named `001.mp3`
- **Case & mini speaker** for output

### Software & Tools

- **PlatformIO** with **Visual Studio Code** for development, flashing and serial debugging

---

## Installation & Usage

1. Flash the ESP8266 with the appropriate firmware.
2. Place the MP3 file (`001.mp3`) on the microSD card.
3. Connect the hardware components as per the schematic.
4. Deploy the device near the aXbo speaker.
5. Enjoy waking up to your preferred music!

---

## Legal Information

- This project makes use of **PlatformIO, Visual Studio Code, NodeMCU, ArduinoSerialMP3Player, FHEM**, and other tools. Refer to their respective licenses for compliance.
- Ensure compliance with **data protection regulations**, especially when handling personal or sensitive information.
- The authors **provide no warranty** and assume **no liability** for any issues arising from the use of this tool.

---
