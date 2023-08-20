#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define PEEK_MAX 80

namespace midi {

/**
 * @brief RingBuffer that supports extensive peek operations to 
 * access and compare the next values
 * @author Phil Schatzmann
 */
class RingBuffer {
public:
  RingBuffer() = default;

  RingBuffer(RingBuffer &t){
    _aucBuffer = nullptr;
    reset();
  }

  RingBuffer(int size) {
    resize(size);
    reset();
  }

  ~RingBuffer() { if (_aucBuffer!=nullptr)delete[] _aucBuffer; }

  uint8_t read() {
    if (isEmpty())
      return -1;

    uint8_t value = _aucBuffer[_iTail];
    _iTail = nextIndex(_iTail);
    _numElems--;

    return value;
  }

  // checks if the buffer is full
  bool isFull() { return available() == max_size; }

  bool isEmpty() { return available() == 0; }

  // write add an entry to the buffer
  bool write(uint8_t data) {
    bool result = false;
    if (!isFull()) {
      _aucBuffer[_iHead] = data;
      _iHead = nextIndex(_iHead);
      _numElems++;
      result = true;
    }
    return result;
  }

  int write(const uint8_t *data, size_t len) {
    for (int j = 0; j < len; j++) {
      if (!write(data[j])) {
        return j;
      }
    }
    return len;
  }

  // clears the buffer
  void reset() {
    _iHead = 0;
    _iTail = 0;
    _numElems = 0;
  }

  // provides the number of entries that are available to read
  int available() { return _numElems; }

  // provides the number of entries that are available to write
  int availableForWrite() { return (max_size - _numElems); }

  void resize(int len) {
    // if no change: we can leave
    if (this->max_size==len && _aucBuffer!=nullptr){
      return;
    }
    // (re)allocate buffer
    if (_aucBuffer != nullptr) {
      delete[] _aucBuffer;
    }
    this->max_size = len;
    if (len > 0) {
      _aucBuffer = new uint8_t[max_size];
      assert(_aucBuffer != nullptr);
    }
    reset();
  }

  /// Returns the maximum capacity of the buffer
  int size() { return max_size; }

  // peeks the actual entry from the buffer
  int peek() {
    if (isEmpty())
      return -1;

    return _aucBuffer[_iTail];
  }

  /// @brief peeks the idx next entry from the buffer
  int peek(int idx) {
    if (idx > available())
      return -1;
    int tmp_tail = (_iTail + idx) % max_size;
    return _aucBuffer[tmp_tail];
  }

  /// @brief returns a temporary copy of the requested bytes
  uint8_t *peekStr(int idx, int len) {
    if (len==0) return 0;
    static uint8_t tmp_copy[PEEK_MAX];
    assert(len < PEEK_MAX);
    assert(idx+len<available());
    memset(&tmp_copy, 0, PEEK_MAX);
    for (int j = 0; j < len; j++) {
      tmp_copy[j] = peek(idx + j);
    }
    return tmp_copy;
  }

  /// @brief alternaive syntax to peek(idx)
  uint8_t operator[](int idx) {
    int result = peek(idx);
    assert(result!=-1);
    return result;
  }

  /// @brief Compares the string with the current peek values
  bool equals(const char *str) {
    int len = strlen(str);
    return strncmp(str, (char*)peekStr(0, len), len) == 0;
  }

  /// @brief Removes the next n characters from the ringbuffer
  void consume(int offset) {
    assert(available() >= offset);
    for (int j = 0; j < offset; j++) {
      read();
    }
  }

protected:
  uint8_t *_aucBuffer = nullptr;
  int _iHead;
  int _iTail;
  int _numElems;
  int max_size = 0;

  int nextIndex(int index) { return (uint32_t)(index + 1) % max_size; }
};

} // namespace