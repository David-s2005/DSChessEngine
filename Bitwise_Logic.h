#ifndef BITWISE_LOGIC_H_INCLUDED
#define BITWISE_LOGIC_H_INCLUDED

#include <cstdint>

uint16_t retSequence(uint16_t _sequence, uint16_t _start_index,
                     uint16_t _length);

void setSequence(uint16_t &_sequence, uint16_t _start_index,
                     uint16_t _length, uint16_t _input);

#endif // BITWISE_LOGIC_H_INCLUDED
