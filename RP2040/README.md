# Seven displays and YD RP2040.

Seven cheap Aliexpress displays, tested with a YD RP2040, Arduino IDE 2.3.2 and TFT_eSPI 2.5.43 ( u8g2 2.34.22 ).

**Board Package :** Raspberry Pi Pico/RP2040 by Earle F. Philhower, III, v3.93

**Arduino IDE Board :** "VCC-GND YD RP2040"

All three IPS displays worked stable with 125MHz (SPI 62.5MHz). Overclocking runs with 150MHz (SPI 75MHz) on all three displays.

200MHz (SPI 100MHz) and 250 MHz (SPI 125MHz) worked only with the display 240x280 and i had to connect the RESET pin of the display to pin 27 (for example), not to pin RUN. 

![3_benchmarks](pictures/3_benchmarks.png)
TFT_eSPI graphicstest

- [Arduino\RP2040_TFT_graphicstest_170x320.ino](Arduino/RP2040_TFT_graphicstest_170x320/RP2040_TFT_graphicstest_170x320.ino)
- [Arduino\RP2040_TFT_graphicstest_240x280.ino](Arduino/RP2040_TFT_graphicstest_240x280/RP2040_TFT_graphicstest_240x280.ino)
- [Arduino\RP2040_TFT_graphicstest_240x320.ino](Arduino/RP2040_TFT_graphicstest_240x320/RP2040_TFT_graphicstest_240x320.ino)

## Install Earle Philhower's RP2040 board package 

Open up the Arduino IDE and go to File->Preferences.
In the dialog that pops up, enter the following URL in the "Additional Boards Manager URLs" field:

- https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

Go to Tools->Boards->Board Manager in the IDE. Choose "Raspberry Pi Pico/RP2040" and select "Add".

## RP2040 First use 

When i got the RP2040 a RPI-RP2 drive showed up on windows but no com port. The solution i found was :

Copy the [PicoBlink.ino.uf2](documents/PicoBlink.ino.uf2) file to the RPI-RP2 drive. After uploading the drive disappears and a com port shows up. The LED on the board is flashing. 

Arduino IDE can upload programs now but while uploading the first program, the com port changes.  

Found here : https://forums.raspberrypi.com/viewtopic.php?t=368305

## Connections for YD RP2040

| GPIO      | TFT   | Interface |Description           |
| --------: | :---- | :-------- | :------------------- |
|         4 | SDA   | I2C       | SDA                  |
|         5 | CLK   | I2C       | SCL                  |
|-----------|------ |-----------|----------------------|
|        19 | SDA   | SPI       | MOSI                 |
|       (16)|       | SPI       | MISO  ( not used )   |
|        18 | SCL   | SPI       | CLK                  |
|        17 | CS    | SPI       | CS                   |
|        22 | DC    | SPI       | DC                   |
|   RUN (27)| RST   | SPI       | Reset ( or PWM-Pin ) |
|       (26)| BLK   |           | 3.3V  ( or PWM-Pin ) |
|           | VCC   |           | 3.3V                 |
|           | GND   |           | GND                  |

**RESET pin :** It's more stable to use the pin 27 (for example) than the pin RUN.

![RGB_Pinout](pictures/RGB_Pinout.png)

The serial monitor in the program [Arduino\RP2040_WS2812.ino](Arduino/RP2040_WS2812/RP2040_WS2812.ino) shows the pins of the YD RP2040 :

```java
MOSI: 19
MISO: 16
SCK: 18
SS: 17
SDA: 4
SCL: 5
LED_BUILTIN: 25
```

## Four monochrome displays

For these displays install the library u8g2 by olikraus.

![3_I2C_Displays](pictures/3_I2C_Displays.png)
Three I2C-Displays. Driver SSD1306 (left), SSD1306 (middle), SSD1309 (right).

**Pay attention** to the VCC and GND pins, which is reversed on the right display !

![GMG12864](pictures/GMG12864.png)
SPI display GMG12864-06D

- [Arduino\RP2040_u8g2_Displays_Graphics_Test.ino](Arduino/RP2040_u8g2_Displays_Graphics_Test/RP2040_u8g2_Displays_Graphics_Test.ino)
- [Arduino\RP2040_u8g2_Displays_Graphics_Test_BL.ino](Arduino/RP2040_u8g2_Displays_Graphics_Test_BL/RP2040_u8g2_Displays_Graphics_Test_BL.ino) with Backlight dimming ( only CMG12864-06D ).

The backlight pin of the GMG12864 is connected with an 47 Ohm resistor, which draws about 12mA on the RP2040 pin and can cause problems.

Choose the driver in the test programs :

```java
// Enable only one of the four display drivers :

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);     // 0.96" mini or 4 button display

//U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2(U8G2_R0);    // 1.3" display, GND und VDD are reversed !!! 

//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);  // not tested

// GMG12864 uncomment both lines !!
//U8G2_ST7565_ERC12864_ALT_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 17, /* dc=*/ 22, /* reset=*/ U8X8_PIN_NONE); 
//#define CONTRAST 75  //  Don't forget this for GMG12864  !!
```

## Configure the library TFT_eSPI

Edit the file [Arduino\libraries\TFT_eSPI\User_Setup_Select.h](Arduino/libraries/TFT_eSPI/User_Setup_Select.h )

```java
// Only **ONE** line below should be uncommented to define your setup.

//#include <User_Setup.h>                // Default setup is root library folder

// new setup file in folder Arduino/libraries, so updates will not overwrite your setups.
#include <../Setup451_RP2040_ST7789_170x320.h>  // RP2040, ST7789
//#include <../Setup452_RP2040_ST7789_240x280.h>  // RP2040, ST7789
//#include <../Setup453_RP2040_ST7789_240x320.h>  // RP2040, ST7789
```
Create the new files :
- [Arduino\libraries\Setup451_RP2040_ST7789_170x320.h](Arduino/libraries/Setup451_RP2040_ST7789_170x320.h)
- [Arduino\libraries\Setup452_RP2040_ST7789_240x280.h](Arduino/libraries/Setup451_RP2040_ST7789_240x280.h)
- [Arduino\libraries\Setup453_RP2040_ST7789_240x320.h](Arduino/libraries/Setup451_RP2040_ST7789_240x320.h)

**Don't forget** the line "#define RP2040_PIO_SPI", which is necessary for Earle Philhower's RP2040 board package !


```java
#define USER_SETUP_ID 451

// Driver
#define ST7789_DRIVER            // Configure all registers
#define TFT_WIDTH  170
#define TFT_HEIGHT 320
#define TFT_INVERSION_ON
#define TFT_BACKLIGHT_ON 1

//#define TFT_RGB_ORDER TFT_BGR  // !!! Only for Display 240x320 !!!

// The PIO can only be user with Earle Philhower's RP2040 board package:
// https://github.com/earlephilhower/arduino-pico

// PIO SPI allows high SPI clock rates to be used when the processor is over-clocked.
// PIO SPI is "write only" and the TFT_eSPI touch functions are not supported.
// A touch screen could be used with a third party library on different SPI pins.

// This invokes the PIO based SPI interface for the RP2040 processor.
#define RP2040_PIO_SPI  // Black screen if you forget this line

//Pins RP2040
#define TFT_BL     -1   // LED back-light  // 26
#define TFT_MISO   -1   // Not connected
#define TFT_MOSI   19
#define TFT_SCLK   18
#define TFT_CS     17 
#define TFT_DC     22
#define TFT_RST    -1   // Set TFT_RST to -1 if display RESET is connected to RP2040 board RUN
//#define TFT_RST    27   // For overclocking, RESET was connected to pin 27  

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
//#define LOAD_FONT8N
#define LOAD_GFXFF
#define SMOOTH_FONT

// Other options
// RP2040 max frequency if f is 125MHz / 2 = 62.5MHz. Take next higher integer.

// #define SPI_FREQUENCY  25000000  // 125/5 =  25.00MHz
// #define SPI_FREQUENCY  32000000  // 125/4 =  31.25MHz
// #define SPI_FREQUENCY  42000000  // 125/3 =  41.67MHz
// #define SPI_FREQUENCY  70000000  // 125/2 =  62.50MHz
// #define SPI_FREQUENCY 100000000  // 200/2 = 125.00MHz  Overclocking
   #define SPI_FREQUENCY 125000000  // 250/2 = 125.00MHz  Overclocking

```
## RP2040 Factory Reset

When testing the backlight and overclocking the RP2040 sometimes wasn't accessible any more. No  RPI-RP2 drive showed up and no com port appeared on windows. 

Solution : Hold down BOOT button then press RESET button when the RP2040 is connected. The RPI-RP2 drive should appear. Copy [PicoBlink.ino.uf2](documents/PicoBlink.ino.uf2) file to the RPI-RP2 drive. After upload and the drive will disappear. The LED on the board starts flashing. Arduino sketches should upload now. But while uploading the first Arduino program, the com port changes. 

Similar solutions can be found here : 

RP2040 Pico clone firmware https://forums.raspberrypi.com/viewtopic.php?t=368305

Factory reset Pico no longer registers com port on windows https://forums.raspberrypi.com/viewtopic.php?t=350680

Bricked MicroPython rescue firmware https://forums.raspberrypi.com/viewtopic.php?f=146&t=305432

Raspberry Pi Pico factory reset https://forum.micropython.org/viewtopic.php?t=10838


The folder [documents](documents) also contains the documentation for the YD RP2040 found here : http://124.222.62.86/yd-data/YD-RP2040/.

## Test programs

All files can be found above in the folder [Arduino](Arduino).

Setup :
- [Arduino\libraries\Setup407_ST7789_320x170.h](Arduino/libraries/Setup407_ST7789_320x170.h)
- [Arduino\libraries\Setup408_ST7789_280x240.h](Arduino/libraries/Setup408_ST7789_280x240.h) 
- [Arduino\libraries\Setup409_ST7789_320x240.h](Arduino/libraries/Setup409_ST7789_320x240.h) 
- [Arduino\libraries\TFT_eSPI\User_Setup_Select.h](Arduino/libraries/TFT_eSPI/User_Setup_Select.h )
- [Arduino\RP2040_WS2812.ino](Arduino/RP2040_WS2812/RP2040_WS2812.ino) 

Benchmark IPS color displays :

- [Arduino\RP2040_TFT_graphicstest_170x320.ino](Arduino/RP2040_TFT_graphicstest_170x320/RP2040_TFT_graphicstest_170x320.ino)
- [Arduino\RP2040_TFT_graphicstest_240x280.ino](Arduino/RP2040_TFT_graphicstest_240x280/RP2040_TFT_graphicstest_240x280.ino)
- [Arduino\RP2040_TFT_graphicstest_240x320.ino](Arduino/RP2040_TFT_graphicstest_240x320/RP2040_TFT_graphicstest_240x320.ino)
- [Arduino\RP2040_TFT_graphicstest_PDQ.ino](Arduino/RP2040_TFT_graphicstest_PDQ/RP2040_TFT_graphicstest_PDQ.ino)

Benchmark monochrome displays :

- [Arduino\RP2040_u8g2_Displays_Graphics_Test.ino](Arduino/RP2040_u8g2_Displays_Graphics_Test/RP2040_u8g2_Displays_Graphics_Test.ino)
- [Arduino\RP2040_u8g2_Displays_Graphics_Test_BL.ino](Arduino/RP2040_u8g2_Displays_Graphics_Test_BL/RP2040_u8g2_Displays_Graphics_Test_BL.ino)

Original TFT_eSPI examples :
- boing_ball.ino
- Bouncy_Circles.ino
- SpriteRotatingCube.ino
