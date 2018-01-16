// -*- c++ -*-

#pragma once

#include <Arduino.h>
#include "twi/wire-protocol-constants.h"
#include "Crgb.h"

// See .cpp file for comments regarding appropriate namespaces
namespace kaleidoscope {
namespace hardware {

#define LED_BANKS 4

#define LEDS_PER_HAND 32
#define LED_BYTES_PER_BANK sizeof(Crgb)  * LEDS_PER_HAND/LED_BANKS

// config options

// used to configure interrupts, configuration for a particular controller
class KeyboardioScanner {
 public:
  KeyboardioScanner(byte setAd01);

  int readVersion();

  byte setKeyscanInterval(byte delay);
  int readKeyscanInterval();

  byte setLEDSPIFrequency(byte frequency);
  int readLEDSPIFrequency();

  bool moreKeysWaiting();
  void sendLEDData();
  void setOneLEDTo(byte led, Crgb color);
  void setAllLEDsTo(Crgb color);
  KeyData getKeyData();
  bool readKeys();
  LedData led_data;
  uint8_t controllerAddress();

 private:
  int addr_;
  int ad01_;
  KeyData key_data_;
  bool key_ready_ = false;
  byte next_led_bank_ = 0;
  void sendLEDBank(byte bank);
  int readRegister(uint8_t cmd);
};

} // namespace hardware {
} // namespace kaleidoscope {
