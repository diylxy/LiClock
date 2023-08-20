/**
 * We provide a midi file as hex array and parse it incrementally. 
 * This sketch has quite some complexity for reading the data. 
 * This can be avoided when you use the MemoryStream and StreamCopy 
 * from the AudioTools project.
 * @author Phil Schatzmann
 */
#include "MidiFileParserMultiTrack.h"
#include "examples/example-midi.h"
#include <algorithm>

using namespace midi;

const int write_size = 256;
MidiFileParserMultiTrack mf;
int pos = 0;
bool debug = false;

void setup() {
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  mf.begin(debug, 256 * 5);

  // preload all data
  while (true) {
    int written = 0;
    int len = std::min((size_t)write_size, (size_t)example_mid_len - pos);
    if (pos < example_mid_len) {
      written = mf.write(example_mid + pos, len);
    } else {
      // mf.endWrite(); // parse final unparsed bytes
      break;
    }
    pos += written;
  }
  // data has been loaded
  printf("Number of events: %ld\n", (long)mf.size());

}

void loop() {

  // Parse midi
  auto state = mf.parseTimed();  // parseTimed() or parse();

  // Process Result
  switch (state.status)
  case MIDI_PARSER_TRACK_MIDI: {
    // process midi
    printf("process");
    printf("  time-cumulated ms: %ld", (long)state.timeInMs());
    printf("  status: %d [%s]", state.midi.status,
           mf.midi_status_name(state.midi.status));
    printf("  channel: %d", state.midi.channel);
    printf("  param1: %d", state.midi.param1);
    printf("  param2: %d\n", state.midi.param2);
    break;
  case MIDI_PARSER_ERROR:
    printf("Error\n");
  case MIDI_PARSER_EOB:
#ifdef ARDUINO
    while(true);
#else
    exit(-1);
#endif
    break;
    default:
    break;
  }
}