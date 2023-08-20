# Arduino MIDI File Parser

A simple MIDI File Parser which is based on the [Midi Parser from Alexandre Bique](https://github.com/abique/midi-parser).

The original library has been converted to a Header Only C++ Library. Unlike the original implementation, we can feed the parser incrementally with data. This is important if you want to use it on a microcontroller that has only a limited amount of RAM.

It can be used as __Arduino library or as regular cmake library__, so you can also compile, run and debug it on a desktop computer.

## Documentation 

- [MidiFileParser](https://pschatzmann.github.io/arduino-midi-fileparser/docs/html/group__midifile.html)


## Installation in Arduino

For Arduino you can download the library as zip and call include Library -> zip library. Or you can git clone this project into the Arduino libraries folder e.g. with
```
cd  ~/Documents/Arduino/libraries
git clone https://github.com/pschatzmann/arduino-midi-fileparser
```

## Building the Example on the Desktop

You can build the example sketch on the desktop 
```
cd ~/Documents/Arduino/libraries/arduino-midi-fileparser/examples/midi-parser
mkdir build
cd build
cmake ..
make
```



