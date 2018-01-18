// -*- c++ -*-

#pragma once

namespace kaleidoscope {
namespace hardware {

union LedData {
  Color leds[LEDS_PER_HAND];
  byte bytes[LED_BANKS][LED_BYTES_PER_BANK];
};

} // namespace hardware {
} // namespace kaleidoscope {