// -*- c++ -*-

#pragma once

namespace kaleidoscope {
namespace hardware {

union LedStates {
  Color leds[LEDS_PER_HAND];
  byte banks[LED_BANKS][LED_BYTES_PER_BANK];
};

} // namespace hardware {
} // namespace kaleidoscope {
