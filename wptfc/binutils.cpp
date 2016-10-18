#include <iostream>
#include <fstream>

void writeU8(std::ofstream* of, uint8_t d) {
  of->put(d & 255);
}


void writeU16(std::ofstream* of, uint16_t d) {
  of->put(d >> 8);
  of->put(d & 255);
}
