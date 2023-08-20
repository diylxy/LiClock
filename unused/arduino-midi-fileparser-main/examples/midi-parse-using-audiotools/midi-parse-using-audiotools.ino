/**
 * We provide a midi file as hex array and parse it incrementally.
 * We use the MemoryStream and StreamCopy from the AudioTools project.
 * Please not that we need to use psram!
 * @author Phil Schatzmann
 */
#include "AudioTools.h" // https://github.com/pschatzmann/arduino-audio-tools
#include "MidiFileParserMultiTrack.h"
#include "examples/example-midi.h"

using namespace midi;

const int write_size = 256;
MidiFileParser parser;  // use MidiFileParserMultiTrack or MidiFileParser
MemoryStream midi_data(example_mid, example_mid_len);
StreamCopy copy(parser, midi_data, write_size);
bool debug = false;

void setup() {
  Serial.begin(115200);
  parser.begin(debug, 256 * 5);
}

void loop() {
  // Try to keep buffer filled
  if (parser.availableForWrite() > write_size) {
    copy.copy();
  }

  // Parse midi
  auto state = parser.parse(); // parseTimed() or parse();

  // Process Result
  switch (state.status)
  case MIDI_PARSER_TRACK_MIDI: {
    // process midi
    char msg[100];
    sprintf(msg, "process  time-cumulated ms: %ld status: %d [%s]   channel: %d  param1: %d   param2: %d",
            (long)state.timeInMs(), state.midi.status,
            parser.midi_status_name(state.midi.status), state.midi.channel,
            state.midi.param1, state.midi.param2);
    Serial.println(msg);
    break;
  case MIDI_PARSER_ERROR:
    Serial.println("Error\n");
  case MIDI_PARSER_EOB:
    stop();
    break;
  default:
    break;
  }
}