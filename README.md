# Welcome to the Lifeclocc repository

## What is Lifeclocc?
Good question! You can find out more at the website https://lifeclo.cc

## How do I use Lifeclocc?
https://lifeclo.cc/manual/

## What's this repo about?
This is the open source repository for the code and schematics for making lifeclocc.

Here you will find the code powering lifeclocc. If you want to get the PCBs, mounts and other bits of source, please head to the source page of my website.

https://manual.lifeclo.cc/source.html


## To update your lifeclocc

### v1 (Maker Edition)
- You need to have the Arduino IDE and Teensyduino IDE installed
- Pull or download the latest master release
- Open the v1 folder in the Arduino IDE
- Select Teensy++ 2.0 under Tools > Board
- (Optional) If you want to reduce power consumption for battery operation, you can change the CPU speed from 16MHz to 4MHz. Do note that if you set it too low, the display will start to flicker.
- Press upload

### v2a (Days Edition)
- You need to have the Arduino IDE installed.
- Pull or download the latest master release
- Open the v2 folder in the Arduino IDE
- Select Arduino Nano under Tools > Board
- Select ATmega168 under Tools > Processor
- Check your displays, are they Common Anode or Common Cathode?
  - If the display is 5161AG or 5161AS, then it is Common Cathode
  - If the display is 5161BG or 5161BS, then it is Common Anode
- In the `constants.h` file, look for the line `#define COMMON_ANODE 1`
  - If you are using Common Anode, leave this at `#define COMMON_ANODE 1`
  - If you are using Common Cathode, leave this at `#define COMMON_ANODE 0`
- Press upload  

### v2b (Seconds Edition)
- Same as v2a, but you need to change one line in `constants.h`
- Change `#define DISPLAY_MODE DAYS` to `#define DISPLAY_MODE SECONDS`
  - This will make the display only show seconds and not weeks
  - You can do this for the v2a board as well, but the labels on the board will be wrong.
- Press upload
