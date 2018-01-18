#include <Arduino.h>
#include "KeyboardioScanner.h"

// why extern "C"? Because twi.c is not C++!
extern "C" {
#include "twi/twi.h"
}

// I'm not sure "kaleidoscope" is the best namespace here. Maybe it should be "keyboardio"?
namespace kaleidoscope {
// Likewise, maybe this should be "scanner"?
namespace hardware {

// Magic constant with no documentation...
#define SCANNER_I2C_ADDR_BASE 0x58
#define ELEMENTS(arr)  (sizeof(arr) / sizeof((arr)[0]))

uint8_t twi_uninitialized = 1;

// What's this array for?
const byte PROGMEM gamma8[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};


// Constructor
KeyboardioScanner::KeyboardioScanner(byte ad01) {
  ad01_ = ad01;
  addr_ = SCANNER_I2C_ADDR_BASE | ad01_;
  if (twi_uninitialized--) {
    twi_init();
  }
}


// Returns the relative controller addresss. The expected range is 0-3
byte KeyboardioScanner::controllerAddress() {
  return ad01;
}


// Sets the keyscan interval. We currently do three reads.
// before declaring a key event debounced.
//
// Takes an integer value representing a counter.
//
// 0 - 0.1-0.25ms
// 1 - 0.125ms
// 10 - 0.35ms
// 25 - 0.8ms
// 50 - 1.6ms
// 100 - 3.15ms
//
// You should think of this as the _minimum_ keyscan interval.
// LED updates can cause a bit of jitter.
//
// returns the Wire.endTransmission code (0 = success)
// https://www.arduino.cc/en/Reference/WireEndTransmission
byte KeyboardioScanner::setKeyscanInterval(byte delay) {
  byte data[] = {TWI_CMD_KEYSCAN_INTERVAL, delay};
  byte result = twi_writeTo(addr_, data, ELEMENTS(data), 1, 0);

  return result;
}


// These functions seem to be here only for debugging purposes, and can probably be removed

// // returns -1 on error, otherwise returns the scanner version integer
// int KeyboardioScanner::readVersion() {
//   return readRegister(TWI_CMD_VERSION);
// }

// // returns -1 on error, otherwise returns the scanner keyscan interval
// int KeyboardioScanner::readKeyscanInterval() {
//   return readRegister(TWI_CMD_KEYSCAN_INTERVAL);
// }

// // returns -1 on error, otherwise returns the LED SPI Frequncy
// int KeyboardioScanner::readLedSpiFrequency() {
//   return readRegister(TWI_CMD_LED_SPI_FREQUENCY);
// }

// The only thing that uses this right now is the RainbowWave plugin

// // Set the LED SPI Frequency. See wire-protocol-constants.h for
// // values.
// //
// // returns the Wire.endTransmission code (0 = success)
// // https://www.arduino.cc/en/Reference/WireEndTransmission
// byte KeyboardioScanner::setLedSpiFrequency(byte frequency) {
//   byte data[] = {TWI_CMD_LED_SPI_FREQUENCY, frequency};
//   byte result = twi_writeTo(addr_, data, ELEMENTS(data), 1, 0);

//   return result;
// }


// I think this is getting data from keyswitches?
int KeyboardioScanner::readRegister(byte cmd) {
  byte return_value = 0;

  byte data[] = {cmd};
  byte result = twi_writeTo(addr_, data, ELEMENTS(data), 1, 0);

  delayMicroseconds(15); // We may be able to drop this in the future
  // but will need to verify with correctly
  // sized pull-ups on both the left and right
  // hands' i2c SDA and SCL lines

  byte rx_buffer[1];

  // perform blocking read into buffer
  byte read = twi_readFrom(addr_, rx_buffer, ELEMENTS(rx_buffer), true);
  if (read > 0) {
    return rx_buffer[0];
  } else {
    return -1;
  }
}


// gives information on the key that was just pressed or released.
bool KeyboardioScanner::readKeys() {
  byte rx_buffer[5];

  // perform blocking read into buffer
  byte read = twi_readFrom(addr_, rx_buffer, ELEMENTS(rx_buffer), true);
  if (rx_buffer[0] == TWI_REPLY_KEYDATA) {
    key_data_.rows[0] = rx_buffer[1];
    key_data_.rows[1] = rx_buffer[2];
    key_data_.rows[2] = rx_buffer[3];
    key_data_.rows[3] = rx_buffer[4];
    return true;
  } else {
    return false;
  }
}


KeyData KeyboardioScanner::getKeyData() {
  return key_data_;
}


// This function gets called to set led status on one bank (eight LEDs) at a time. Each
// time it's called, it updates the next bank. I'm renaming it to be more clear.
void KeyboardioScanner::updateNextLedBank() {
  sendLedBank(next_led_bank_++);
  if (next_led_bank_ == LED_BANKS) {
    next_led_bank_ = 0;
  }
}


// This function is private, and only gets called by updateNextLedBank() (see above)
void KeyboardioScanner::updateLedBank(byte bank) {
  byte data[LED_BYTES_PER_BANK + 1];
  data[0] = TWI_CMD_LED_BASE + bank;
  for (byte i = 0 ; i < LED_BYTES_PER_BANK; i++) {
    data[i + 1] = pgm_read_byte(&gamma8[led_data_.bytes[bank][i]]);
  }
  byte result = twi_writeTo(addr_, data, ELEMENTS(data), 1, 0);
}


// An efficient way to set the value of just one LED, without having to update everything
void KeyboardioScanner::updateLed(byte led, Color color) {
  byte data[] = {TWI_CMD_LED_SET_ONE_TO,
                 led,
                 pgm_read_byte(&gamma8[color.b]),
                 pgm_read_byte(&gamma8[color.g]),
                 pgm_read_byte(&gamma8[color.r])
                };
  byte result = twi_writeTo(addr_, data, ELEMENTS(data), 1, 0);
  led_data_.leds[led] = color;
}


// An efficient way to set all LEDs to the same color at once
void KeyboardioScanner::updateAllLeds(Color color) {
  byte data[] = {TWI_CMD_LED_SET_ALL_TO,
                 pgm_read_byte(&gamma8[color.b]),
                 pgm_read_byte(&gamma8[color.g]),
                 pgm_read_byte(&gamma8[color.r])
                };
  byte result = twi_writeTo(addr_, data, ELEMENTS(data), 1, 0);
  // we should set all the values of led_data_ here
  for (byte led = 0; led < TOTAL_LEDS; ++led) {
    led_data_.leds[led] = color;
  }
}

} // namespace hardware {
} // namespace kaleidoscope {
