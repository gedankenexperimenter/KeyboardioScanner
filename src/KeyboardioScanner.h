// -*- c++ -*-

#pragma once

#include <Arduino.h>
#include "twi/wire-protocol-constants.h"
#include "Color.h"

// See .cpp file for comments regarding appropriate namespaces
namespace kaleidoscope {
namespace hardware {

// I want to rewrite the whole system of LED control; it's ugly
#define LED_BANKS 4
#define LEDS_PER_HAND 32
#define LED_BYTES_PER_BANK sizeof(Color)  * LEDS_PER_HAND/LED_BANKS

// config options

// used to configure interrupts, configuration for a particular controller
class KeyboardioScanner {
 public:
  KeyboardioScanner(byte setAd01);

  int readVersion();

  byte setKeyscanInterval(byte delay);
  int readKeyscanInterval();

  byte setLedSpiFrequency(byte frequency);
  int readLedSpiFrequency();

  bool moreKeysWaiting();
  void sendLedData();
  void setOneLedTo(byte led, Color color);
  void setAllLedsTo(Color color);
  KeyData getKeyData();
  bool readKeys();
  LedData led_data;
  byte controllerAddress();

 private:
  int addr_;
  int ad01_;
  KeyData key_data_;
  bool key_ready_ = false;
  byte next_led_bank_ = 0;
  void sendLedBank(byte bank);
  int readRegister(byte cmd);
};

} // namespace hardware {
} // namespace kaleidoscope {
