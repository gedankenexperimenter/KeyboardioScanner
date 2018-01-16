// -*- c++ -*-

#pragma once

namespace kaleidoscope {
namespace hardware {

struct Crgb {
  byte b;
  byte g;
  byte r;

  Crgb() = default; // I think this initializes to zeros
  constexpr Crgb(byte r, byte g, byte b) : r(r), g(g), b(b) {}
};

} // namespace hardware {
} // namespace kaleidoscope {
