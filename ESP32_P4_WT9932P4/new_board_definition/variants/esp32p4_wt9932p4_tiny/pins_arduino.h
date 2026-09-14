#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>
#include "soc/soc_caps.h"

// BOOT_MODE 35
// BOOT_MODE2 36 pullup

#define PIN_RGB_LED 51

// BUILTIN_LED can be used in new Arduino API digitalWrite() like in Blink.ino
static const uint8_t LED_BUILTIN = SOC_GPIO_PIN_COUNT + PIN_RGB_LED;
#define BUILTIN_LED LED_BUILTIN  // backward compatibility
#define LED_BUILTIN LED_BUILTIN  // allow testing #ifdef LED_BUILTIN

// RGB_BUILTIN and RGB_BRIGHTNESS can be used in new Arduino API rgbLedWrite()
#define RGB_BUILTIN    LED_BUILTIN
#define RGB_BRIGHTNESS 64

static const uint8_t TX = 37;
static const uint8_t RX = 38;

static const uint8_t SDA = 7;
static const uint8_t SCL = 8;

// GPIO 36 and below: no extra on-chip LDO needed for the IO bank.
// GPIO 39-48: LDO VO4 (channel 4). GPIO 37-38 (UART) are outside that VO4 range.
static const uint8_t SS = 28;
static const uint8_t MOSI = 29;
static const uint8_t MISO = 31;
static const uint8_t SCK = 30;

static const uint8_t A0 = 16;
static const uint8_t A1 = 17;
static const uint8_t A2 = 18;
static const uint8_t A3 = 19;
static const uint8_t A4 = 20;
static const uint8_t A5 = 21;
static const uint8_t A6 = 22;
static const uint8_t A7 = 23;
static const uint8_t A8 = 49;
static const uint8_t A9 = 50;
static const uint8_t A10 = 51;
static const uint8_t A11 = 52;
static const uint8_t A12 = 53;
static const uint8_t A13 = 54;

static const uint8_t T0 = 2;
static const uint8_t T1 = 3;
static const uint8_t T2 = 4;
static const uint8_t T3 = 5;
static const uint8_t T4 = 6;
static const uint8_t T5 = 7;
static const uint8_t T6 = 8;
static const uint8_t T7 = 9;
static const uint8_t T8 = 10;
static const uint8_t T9 = 11;
static const uint8_t T10 = 12;
static const uint8_t T11 = 13;
static const uint8_t T12 = 14;
static const uint8_t T13 = 15;

// On-chip GP LDO: periman enables VO4 when a GPIO in the range is used (see esp32-hal-ldo.c).
#define BOARD_PERIMAN_IO_LDO_AUTO        1
#define BOARD_PERIMAN_IO_LDO0_CHANNEL    4   // LDO_VO4 on ESP32-P4
#define BOARD_PERIMAN_IO_LDO0_GPIO_MIN   39  // Function EV: GPIO 39-48 on VO4
#define BOARD_PERIMAN_IO_LDO0_GPIO_MAX   48
#define BOARD_PERIMAN_IO_LDO0_VOLTAGE_MV 3300

#endif /* Pins_Arduino_h */
