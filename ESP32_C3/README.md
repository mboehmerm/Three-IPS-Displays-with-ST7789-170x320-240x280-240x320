# !!!! UNDER CONSTRUCTION !!!!
# ESP32-C3 and esp32 3.0.2 board package and ST7789 SPI display

Cheap Aliexpress displays, tested with a Tasmota Core ESP32-C3, Arduino IDE 2.3.2 and TFT_eSPI 2.5.43

**Board Package :** esp32 3.0.2 ( or esp32 2.0.14 )

**Arduino IDE Board :** ESP32-C3 Dev Module

All three IPS displays worked stable at 80MHz.

![3_benchmarks](pictures/3_benchmarks.png)
TFT_eSPI graphicstest

## Also tested ESP32-S3 and esp32 board package 3.0.2

Easier to hanle is the ESP32-S3.
Here is only one to add in the TFT_eSPI configuration file 

> #define USE_HSPI_PORT

## esp32 board package, TFT_eSPI 2.5.43 and ESP32-C3

There are two options tested :
1. Downgrade the esp32 board package to version 2.0.14 
2. For esp32 board package version 3.0.2, there are two files that must be changed,  **TFT_eSPI_ESP32_C3.h** and the esp32 package file **soc.h** for ESP32-C3.

Downgrading is easy, so let's have a look at the second option.

Edit the file
> C:\Users\<username>\Documents\Arduino\libraries\TFT_eSPI\Processors\TFT_eSPI_ESP32_C3.h

and change in line 71
```
 #define SPI_PORT SPI2_HOST 
```
to
```
#if ESP_ARDUINO_VERSION_MAJOR < 3
  #define SPI_PORT SPI2_HOST
#else
  #define SPI_PORT 2
#endif
```

The second file is
>  C:\Users\<username>\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-bd2b9390ef\esp32c3\include\soc\esp32c3\include\soc\soc.h

Replace
```
#define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (0))   // only one GPSPI 
```
with
```
#define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (DR_REG_SPI0_BASE - ((i) * 0x1000)))
```
That's all.

Solution for ESP32 C3 found here :
- https://github.com/Bodmer/TFT_eSPI/issues/3384#issuecomment-2200970244
- [https://github.com/espressif/arduino-esp32/issues/9618](https://github.com/espressif/arduino-esp32/issues/9618) 

This is the important text:

```
The change done in IDF to #define REG_SPI_BASE(i) (((i)>=2) ? (DR_REG_SPI2_BASE + (i-2) * 0x1000) : (0)) // GPSPI2 and GPSPI3 is wrong.  Correct is #define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (DR_REG_SPI0_BASE - ((i) * 0x1000))) // GPSPI2 and GPSPI3 Changed this in our IDF fork for Tasmota, which is the base for compiling the Tasmota Arduino libs.
Edit: The changes done for the other SOCs are imho wrong too. Only ESP32 looks correct.
```
## Connections for Tasmota Core ESP32-C3 and ST7789 IPS displays

| GPIO      | TFT   | Description          |
| --------: | :---- | :------------------- |
|         7 | CS    | CS                   |
|         6 | SDA   | MOSI                 |
|         5 | ---   | MISO  ( not used )   |
|         4 | SCL   | SCLK                 |
|        19 | DC    | DC                   |
|        EN | RST   | Reset or PWM-Pin     |
|        18 | BLK   | 3.3V  or PWM-Pin     |
|           | VCC   | 3.3V                 |
|           | GND   | GND                  |


## Configuring the TFT_eSPI

```java
#define USER_SETUP_ID 413

// Driver
#define ST7789_DRIVER   // Configure all registers

#define TFT_WIDTH  170  // 240  // 240 
#define TFT_HEIGHT 320  // 280  // 320

//#define TFT_RGB_ORDER TFT_BGR // only for display 240x320 // Colour order Blue-Green-Red
//#define TFT_INVERSION_ON      // only for display 240x320

// Pins ESP32 S3
#define TFT_CS    10    // 10 or 34 (FSPI CS0) 
#define TFT_MOSI  11    // 11 or 35 (FSPI D) 
#define TFT_SCLK  12    // 12 or 36 (FSPI CLK)
#define TFT_MISO  13    // 13 or 37 (FSPI Q)
#define TFT_DC     7    // Data Command control pin
#define TFT_RST   -1    // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN

#define TOUCH_CS   6    // Chip select pin (T_CS) of touch screen

// Fonts
#define LOAD_GLCD       // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2      // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4      // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6      // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7      // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8      // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N   // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF      // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// FSPI / VSPI port (SPI2) used unless following defined. HSPI port is (SPI3) on S3.
#define USE_HSPI_PORT   // Necessary for ESP32 S3 and Pins 10-12

// Other options
//#define SPI_FREQUENCY     27000000    // 80/3 MHz
#define SPI_FREQUENCY     40000000      // 80/2 MHz  // works fine. 
//#define SPI_FREQUENCY     80000000    // 80/1 MHz  // a bit too fast

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  16000000
#define SPI_TOUCH_FREQUENCY  2500000
```

Now the ST7789 display works with my Tasmota ESP32-C3 as long as I don't forget to set the Flash mode to "DIO".

