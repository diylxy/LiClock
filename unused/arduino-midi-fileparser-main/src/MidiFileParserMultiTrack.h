#pragma once
#include "MidiFileParser.h"
#include <iterator>
#include <list>
#include <vector>

namespace midi {

/// @brief Multichannel processing status
enum ProcessingStatus { Inserting, Parsing, EndOfData };

/**
 * A list of events for each track held in an array
 */
class multichannel_segment_tracks {
public:
  /// Adds an midi event to the indicated track
  void add(int track, midi_time_event event) {
    // add new track
    addTrack(track);
    // add event
    if (event.status!=0){
      tracks[track].push_back(event);
    }
  }

  /// Provides the number of tracks
  int trackCount() { return tracks.size(); }

  /// Moves the iterators to the first element
  void begin() {
    // set iterators to first element
    process_status = EndOfData;
    for (int track = 0; track < tracks.size(); track++) {
      if (iterators.size() < track + 1) {
        iterators.push_back(tracks[track].begin());
      } else {
        iterators[track] = tracks[track].begin();
      }
      // check if there is any ndata
      if (iterators[0] != tracks[0].end()) {
        process_status = Parsing;
      }
    }
  }

  /// Provides the next event
  midi_time_event &nextEvent() {
    if (process_status == Inserting) {
      return no_data;
    }

    int min_idx = 0;
    process_status = EndOfData;
    // find entry with min time
    uint64_t min_time = 0xFFFFFFFFFFFFFFFF;
    for (int j = 0; j < trackCount(); j++) {
      if (iterators[j] != tracks[j].end() &&
          (*iterators[j]).time_ms < min_time) {
        min_idx = j;
        process_status = Parsing;
      }
    }

    if (process_status == EndOfData) {
      return no_data;
    }

    // increment iterator and return result
    midi_time_event *p_result = &(*iterators[min_idx]);
    iterators[min_idx]++;
    return *p_result;
  }

  /// Count number for midi events
  size_t size() {
    size_t result = 0;
    for (int j = 0; j < trackCount(); j++) {
      result += tracks[j].size();
    }
    return result;
  }

  /// Returns true if there are no midi events
  bool isEmpty() { return size() == 0; }

  /// Provides the last recorded time for the indicated track
  uint64_t lastTime(int track) {
    addTrack(track);
    auto &events = tracks[track];
    if (events.empty())
      return 0;
    return events.back().time_ms;
  }

  /// Releases all data
  void end() {
    tracks.clear();
    iterators.clear();
  }

  /// Provides the current processing status
  ProcessingStatus status() { return process_status; }

  /// Provides the reference to the nodata time event which signals that we provided no data
  midi_time_event &getNoDataEvent() { return no_data; }

protected:
  std::vector<std::list<midi_time_event>> tracks;
  std::vector<std::list<midi_time_event>::iterator> iterators;
  midi_time_event no_data;
  ProcessingStatus process_status = Inserting;

  void addTrack(int track) {
    if (track >= tracks.size()) {
      std::list<midi_time_event> event_list;
      tracks.push_back(event_list);
    }
  }
};

/**
 * @brief Midi file parser which stores the data in RAM before making them
 * available for parsing. Please note that this class might need a considerable
 * amount of ram so you might consider to activate the PSRAM. Make sure that you 
 * complete all write() calls before starting to call parse() or parseTimed()!
 * @ingroup midifile
 * @author Phil Schatzmann
 */
class MidiFileParserMultiTrack : public Print {
public:
  /// Initializes & starts the processing
  bool begin(bool log = true, int bufferSize = MIDI_BUFFER_SIZE) {
    restart();
    return parser.begin(log, bufferSize);
  }

  /// Call this method to restart the parsing with the available data
  void restart() {
    is_iterators_available = false;
    start_time = millis();
    p_current_state = nullptr;
  }

  /// Single character write
  size_t write(uint8_t c) override { return write(&c, 1); }

  /// Feed/Provide the midi data to the parser
  virtual size_t write(const uint8_t *data, size_t len) {
    int result = parser.write(data, len);
    // Parse and process the next event
    parse(20);
    return result;
  }

  /// Max number of bytes that we can write
  int availableForWrite() { return parser.availableForWrite(); }

  /// Returns the number of recorded tracks
  int trackCount() { return data.trackCount(); }

  /// Provide the next midi element
  midi_parser_state &parse() {
    // we wait for the parser to complete the processing
    midi_time_event &event = parseTimeEvent();
    if (event == data.getNoDataEvent()) {
      switch (data.status()) {
      case Inserting:
        return not_ready;
      case EndOfData:
        return eob;
      default:
        return not_ready;
      }
    }

    // provide next element
    parser_state.status = MIDI_PARSER_TRACK_MIDI;
    parser_state.vtime_ms = event.time_ms;
    parser_state.midi.channel = event.channel;
    parser_state.midi.status = event.status;
    parser_state.midi.param1 = event.param1;
    parser_state.midi.param2 = event.param2;
    return parser_state;
  }

  /// Provide the next midi element considering the times.
  midi_parser_state &parseTimed() {

    if (p_current_state == nullptr) {
      p_current_state = &parseTimeEvent();
    }

    // check if data is relevant
    if (p_current_state == &data.getNoDataEvent()) {
      switch (data.status()) {
      case Inserting:
        p_current_state = nullptr;
        return not_ready;
      case EndOfData:
        return eob;
      default:
        break;
      }
    }

    // Wait for event to become relevant
    if ((millis() - start_time) < p_current_state->time_ms) {
      return not_ready;
    }

    // return result
    parser_state.status = MIDI_PARSER_TRACK_MIDI;
    parser_state.vtime_ms = p_current_state->time_ms;
    parser_state.midi.channel = p_current_state->channel;
    parser_state.midi.status = p_current_state->status;
    parser_state.midi.param1 = p_current_state->param1;
    parser_state.midi.param2 = p_current_state->param2;
    p_current_state = nullptr;
    return parser_state;
  }

  /// Returns true as long as we have data to be written or parsed
  operator bool() { return parser || data.status() == Parsing; }

  /// Returns true if there are no midi events
  bool isEmpty() { return data.isEmpty(); }

  /// Returns the number of available midi events (after writing)
  size_t size() { return data.size(); }

  /// Ends the processing: releases all memory
  void end() { data.end(); }

  /// Marks that we are done with writing the data to prepare for parse() and
  /// parseTimed() calls.
  void endWrite() { 
    parser.setState(MIDI_PARSER_EOB);
    // force to parse the remaining bytes
    parse(0);
  }

  /// Provides the string description for the midi_status value
  const char *midi_status_name(int status) {
    return parser.midi_status_name(status);
  }

  /// Provides the string description for the file format
  const char *midi_file_format_name(int fmt) {
    return parser.midi_file_format_name(fmt);
  }

  /// Provides the string description for the midi_meta value
  const char *midi_meta_name(int type) { return parser.midi_meta_name(type); }

protected:
  MidiFileParser parser;
  multichannel_segment_tracks data;
  midi_parser_state parser_state;
  midi_parser_state not_ready{MIDI_PARSER_DELAY};
  midi_parser_state eob{MIDI_PARSER_EOB};
  midi_time_event *p_current_state = nullptr;
  uint64_t start_time = 0l;
  bool is_iterators_available = false;

  void parse(int limit) {
    while (parser.parser_state.in.available() > limit) {
      midi_parser_state &state = parser.parse();
      addEvent(state);
    }
  }

  /// Adds an new midi event with the corresponding time
  void addEvent(midi_parser_state &state) {
    int track = state.track.number;
    // add new midi_time_event
    midi_time_event event;
    event.status = state.midi.status;
    event.channel = state.midi.channel;
    event.param1 = state.midi.param1;
    event.param2 = state.midi.param2;
    event.time_ms = state.timeInMs() + data.lastTime(track);
    data.add(track, event);
    
  }

  /// Provide the next midi element using the iterators
  midi_time_event &parseTimeEvent() {
    // create iterators to point at beginning
    if (!is_iterators_available) {
      // complete parsing
      parse(0);
      // setup iterators
      is_iterators_available = true;
      data.begin();
    }

    // provide next element
    return data.nextEvent();
  }
};

} // namespace midi
