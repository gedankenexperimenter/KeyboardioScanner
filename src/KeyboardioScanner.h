// -*- c++ -*-

#pragma once

#include <Arduino.h>
#include "twi/wire-protocol-constants.h"
#include "Color.h"

// See .cpp file for comments regarding appropriate namespaces
namespace kaleidoscope {
namespace model01 {


// used to configure interrupts, configuration for a particular controller
class KeyboardioScanner {
 public:
  KeyboardioScanner(byte setAd01);

  // only here for debugging, I think; it's unused in firmware
  //int readVersion();

  byte setKeyscanInterval(byte delay);
  //int readKeyscanInterval();

  //byte setLedSpiFrequency(byte frequency);
  //int readLedSpiFrequency();

  // interface to LED color array
  Color getLedColor(byte i);
  void  setLedColor(byte i, Color color);

  // send message to controller to change physical LEDs
  void updateNextLedBank();

  void updateLed(byte led, Color color);
  void updateAllLeds(Color color);

  bool scanKeys();

 private:
  int addr_;
  int ad01_;

  //int readRegister(byte cmd);

  // These constants might be wasting some space vs #define
  static constexpr byte total_leds_         = 32;  // per controller
  static constexpr byte leds_per_bank_      = 8;   // CHAR_BIT
  static constexpr byte led_banks_          = TOTAL_LEDS / LEDS_PER_BANK;
  static constexpr byte led_bytes_per_bank_ = LEDS_PER_BANK * sizeof(Color);

  // This union stores the (pending) color data for all the LEDs controlled by this
  // scanner/controller
  union {
    Color leds[leds_per_hand_];
    byte banks[led_banks_][led_bytes_per_bank_];
  } led_states_;

  // the next LED bank that will be updated by updateLedBank()
  byte next_led_bank_ = 0;

  // bitfield storing which LED banks need an update
  byte led_banks_changed_ = 0;

  void updateLedBank(byte bank);

}; // class KeyboardioScanner {

} // namespace model01 {
} // namespace kaleidoscope {
