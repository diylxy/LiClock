#pragma once
#include "Midi.h"
#include "MidiFileParser.h"
#include "MidiStreamOut.h"

namespace midi {

/**
 * @brief Parse the MidiFileParser sending the output to MidiStreamOut.
 * This class requires https://github.com/pschatzmann/arduino-midi
 * @ingroup midifile
 * @author Phil Schatzmann
 */
class MidiFileParserOut {
public:
  /// Default Constructor
  MidiFileParserOut(MidiFileParser &file, MidiStreamOut &out) {
    p_file = &file;
    p_out = &out;
  }
  /** @brief Parses the midi message and forwards it to MidiStreamp_out->
   Returns false when we are at the end.
  */
  bool parse() {
    // Parse midi
    auto state = p_file->parseTimed();

    // Process Result
    switch (state.status)
    case MIDI_PARSER_TRACK_MIDI: {
      switch (state.midi.status) {
      case MIDI_STATUS_NOTE_OFF:
        p_out->noteOff(state.midi.param1, state.midi.param2,
                       state.midi.channel);
        break;
      case MIDI_STATUS_NOTE_ON:
        p_out->noteOn(state.midi.param1, state.midi.param2, state.midi.channel);
        break;
      case MIDI_STATUS_NOTE_AT:
        p_out->polyPressure(state.midi.param1, state.midi.channel);
        break;
      case MIDI_STATUS_CC:
        p_out->controlChange(state.midi.param1,
                             state.midi.param2, state.midi.channel);
        break;
      case MIDI_STATUS_PGM_CHANGE:
        p_out->programChange(state.midi.param1, state.midi.channel);
        break;
      case MIDI_STATUS_CHANNEL_AT:
        p_out->channelPressure(state.midi.param1, state.midi.channel);
        break;
      case MIDI_STATUS_PITCH_BEND:
        p_out->pitchBend(state.midi.param1, state.midi.channel);
        break;
      default:
        log("Unknown MIDI Event");
        break;
      }
      active = true;
      break;
    case MIDI_PARSER_ERROR:
      log("Error");
    case MIDI_PARSER_EOB:
      // stop processing loop
      active = false;
      break;
    default:
      break;
    }
    return active;
  }

protected:
  MidiFileParser *p_file = nullptr;
  MidiStreamOut *p_out = nullptr;
  bool active = true;

  void log(const char *msg) {
#ifdef ARDUINO
    Serial.println(msg);
#else
    printf("%s\n", msg);
#endif
  }
};

} // namespace midi