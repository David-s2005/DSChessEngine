#include "Bitwise_Logic.h"

// Returns a specific bit sequence from a 16 bit sequence given the
// start position (from the right) and its length.
uint16_t retSequence(uint16_t _sequence, uint16_t _start_index,
                     uint16_t _length) {
    uint16_t mask = static_cast<uint16_t>((0b1 << _length) - 0b1);
    return (_sequence >> _start_index) & mask;
}

// Inserts _input into a specific bit sequence within _sequence,
// given the start position (from the right) and its length.
void setSequence(uint16_t &_sequence, uint16_t _start_index,
                     uint16_t _length, uint16_t _input) {
    uint16_t mask = static_cast<uint16_t>(((0b1 << _length) - 0b1) << _start_index);
    uint16_t value = static_cast<uint16_t>((_input << _start_index) & mask);
    _sequence = static_cast<uint16_t>((_sequence & ~mask) | value);
}
