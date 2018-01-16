// -*- c++ -*-

#pragma once

#include <Arduino.h>
#include "twi/wire-protocol-constants.h"

// This ought to be defined elsewhere?
struct cRGB {
  uint8_t b;
  uint8_t g;
  uint8_t r;
};

#define LED_BANKS 4

#define LEDS_PER_HAND 32
#define LED_BYTES_PER_BANK sizeof(cRGB)  * LEDS_PER_HAND/LED_BANKS

// Should be defined in separate file, unless it's only used here
union LEDData_t {
  cRGB leds[LEDS_PER_HAND];
  byte bytes[LED_BANKS][LED_BYTES_PER_BANK];
};


// Same datastructure as on the other side
union key_t {
  struct {
    uint8_t row: 2,
            col: 3,
            keyState: 1,
            keyEventsWaiting: 1,
            eventReported: 1;
  };
  uint8_t val;
};


// again, should be defined elsewhere?
union keydata_t {
  uint8_t rows[4];
  uint32_t all;
};

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
  void setOneLEDTo(byte led, cRGB color);
  void setAllLEDsTo(cRGB color);
  keydata_t getKeyData();
  bool readKeys();
  LEDData_t ledData;
  uint8_t controllerAddress();

 private:
  int addr_;
  int ad01_;
  keydata_t key_data_;
  bool key_ready_ = false;
  byte next_led_bank_ = 0;
  void sendLEDBank(byte bank);
  int readRegister(uint8_t cmd);
};

