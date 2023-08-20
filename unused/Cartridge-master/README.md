# Cartridge
For fellow chiptune fans, this is a library for parsing VGM into an NES APU emulation on ESP32!

----------
# Contents
- [Installation](#installation)
- [Playing NES Music](#playing-nes-music)
- [Converting VGZ for use on ESP32](#converting-vgz-for-use-on-esp32)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License and credits](#license-and-credits)

----------
# Installation

**With Arduino Library Manager:** *Coming soon!*

~1. Open *Sketch > Include Library > Manage Libraries* in the Arduino IDE.~
~2. Search for "Lixie", and select the latest version.~
~3. Click the Install button and Arduino will prepare the library and examples for you!~

**Manual Install:**

1. Click "Clone or Download" above to get an "Cartridge-master.zip" file.
2. Extract it's contents to the libraries folder in your sketchbook.
3. Rename the folder from "Cartridge-master" to "Cartridge".

------------
# Playing NES Music:

**Cartridge cart**(uint8_t **pulse_1_pin**, uint8_t **pulse_2_pin**, uint8_t **noise_pin**, uint8_t **triangle_pin**);

This tells the library which pins you'd like to attach to the NES "voices". By default, it will use:

    PULSE_1 --- > Pin 12
    PULSE_2 --- > Pin 14
    NOISE ----- > Pin 26
    TRIANGLE -- > Pin 27

**cart.init**();

Inititalizes ultrasonic SigmaDelta generation on the pins from before, allowing for a free, hacky DAC!

**cart.play_nes**(uint8_t* **vgm_data**, bool **looping**);

This is where the magic happens. When cart.play_nes(music) is called, the ESP32 fires up the fake NES APU, resets it's registers, parses some data from the VGM header, and begins to play your music! Connect the four pins from above to the positive terminal of a speaker and enjoy! Some tracks have loops built in - set *looping* to true and the track will restart after it ends, either at the beginning or a defined middle portion of the song. **This is a blocking function. Use the cart.frame_counter_cb() below to define code to be run during playback.**

**cart.frame_counter_cb**(void **func**);

Used to define a callback function that should be called every "1st step" of the NES' Frame Counter. Set this to a function for polling buttons, printing a quick debug, or anything else that can be run fairly quickly to avoid slowing down the NES APU.

----------
# Converting VGZ for use on ESP32

By far, the biggest site to get ".VGM" music from is http://vgmrips.net/. These files are VGM format, which means they're a time-based recording of when-and-where APU register writes happen in a real NES when music is played. However, they need a little touchup to work in the Arduino IDE. Drag and drop a \*.vgz file (they're gzipped by default) from their site onto the ".bat" script included in the /converter directory. It'll spit out a file named vgm_arduino.txt that you can paste into the header of your Cartridge sketches! For example:

**Pasted before your setup() function:**

    uint8_t music[] = {0x56, 0x67, 0x6D, 0x20, 0xC4, 0x3E, ...  0xB4, 0x00, 0x95, 0x71, 0xB4, 0x0A, 0xAA};

**In setup() or loop():**

    cart.init();
    cart.play_nes(music);

----------
# Troubleshooting

As of right now, there's two things to note about the Cartridge library.

- It currently does not support the NES DMC for sample playback. (Such as the steel drums from Super Mario Bros. 3) I'm not sure yet if the VGM file format even works with DMC samples.
- It was written by someone who has never written an emulator before, using the horribly inconsistently labeled (but *very* thorough) APU documentation found at NesDev.com. It's not the best, but it's a great demonstration!

If you'd like to fork your own version or submit changes here, feel free to!

----------
# Contributing
Any advice or pull requests are welcome. :)

----------
# License and Credits
**Developed by Connor Nishijima (2018)**

**Released under the [GPLv3 license](http://www.gnu.org/licenses/gpl-3.0.en.html).**
