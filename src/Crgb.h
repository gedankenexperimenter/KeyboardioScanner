// -*- c++ -*-

#pragma once

namespace kaleidoscope {
// Maybe this should be in a different/addtitional namespace, to avoid conflicts with
// other hardware definitions. Alternatively, it could be in the main (uniquely named)
// KeyboardioScanner.h (or Kaleidoscope-Hardware-*.h) file.
namespace hardware {

// In the old cRGB struct, the order of the bytes (b,g,r) was important because brace
// initialization was used. for the Color struct, I'm using a real constructor, so that
// won't matter, but I'll leave the order as it was for brace initialization. What really
// matters is the order of the arguments when calling the twi function that sends the
// commands to the LED controller module, and that's handled by the functions that make
// those calls, not this struct.

struct Color {
  byte b;
  byte g;
  byte r;

  Color() = default; // I think this initializes to zeros
  // I'm not really sure why constexpr would help here. I should experiment to see if it
  // produces smaller code. I'm also not sure about the constructor above; this is obtuse
  // C++ magic.
  constexpr Color(byte red, byte green, byte blue)
      : r(red), g(green), b(blue) {}
};

} // namespace hardware {
} // namespace kaleidoscope {
