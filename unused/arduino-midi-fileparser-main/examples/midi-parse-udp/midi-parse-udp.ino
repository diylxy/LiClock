/**
 * We provide a midi file as hex array and parse it incrementally. 
 * The midi envents are then sent out via UDP
 * Dependencies.
 * https://github.com/pschatzmann/arduino-midi
 * https://github.com/pschatzmann/arduino-audio-tools
 */
#include "AudioTools.h"
#include "MidiFileParser.h"
#include "MidiStreamOut.h"
#include "MidiFileParserOut.h"
#include "MidiUdp.h"
#include "WiFi.h"
#include "examples/example-midi.h"

using namespace midi;

const bool debug = false;
const char *SSID = "SSID";
const char *PWD = "password";
URLStream url(SSID, PWD);
MidiFileParser mf;
// final output
IPAddress ip(192, 168, 1, 36);
MidiUdp udp(ip, 21928);           // UDP Destination
MidiStreamOut stream_out(udp);    // Ouput stream for midi
MidiFileParserOut out(mf, stream_out);  // Output for parser
// data flow
StreamCopy copier(mf, url, 80); // copy in to out


void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  url.begin("https://www.midiworld.com/download/4849");
  mf.begin(debug, 80 * 10);

}


void loop() {
  // try to keep the parser filled
  if (mf.availableForWrite()>=80)
    copier.copy();

  // Parse and process the next event
  out.parse();
}