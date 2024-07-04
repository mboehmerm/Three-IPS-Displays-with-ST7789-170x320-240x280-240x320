// See SetupX_Template.h for all options available
#define USER_SETUP_ID 454

// Driver
#define ILI9488_DRIVER  // WARNING: Do not connect ILI9488 display SDO to MISO if other devices share the SPI bus (TFT SDO does NOT tristate when CS is high)

//#define TFT_INVERSION_OFF

// The PIO can only be user with Earle Philhower's RP2040 board package:
// https://github.com/earlephilhower/arduino-pico

// PIO SPI allows high SPI clock rates to be used when the processor is over-clocked.
// PIO SPI is "write only" and the TFT_eSPI touch functions are not supported.
// A touch screen could be used with a third party library on different SPI pins.

// This invokes the PIO based SPI interface for the RP2040 processor.

#define RP2040_PIO_SPI  // Faster with all frequencies except 125MHz, but no touch functions.

// Pins RP2040
#define TFT_BL     -1   // LED back-light  // 26
#define TFT_MISO   -1   // Not connected
#define TFT_MOSI   19
#define TFT_SCLK   18
#define TFT_CS     17 
#define TFT_DC     22
#define TFT_RST    -1   // Set TFT_RST to -1 if display RESET is connected to RP2040 board RUN
//#define TFT_RST    27 // For overclocking, RESET was connected to pin 27. Or capacitor 2,2nF between RUN and GND. 

//#define TOUCH_CS   6    // Chip select pin (T_CS) of touch screen. Not supported for RP2040 ?!

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
//#define USE_HSPI_PORT // only ESP32 S3

// RP2040 max frequency if f is 125MHz / 2 = 62.5MHz. Take next higher integer.

// #define SPI_FREQUENCY  25000000  // 125/5 =  25.00MHz
// #define SPI_FREQUENCY  32000000  // 125/4 =  31.25MHz
// #define SPI_FREQUENCY  42000000  // 125/3 =  41.67MHz
// #define SPI_FREQUENCY  75000000  // 125/2 =  62.50MHz
   #define SPI_FREQUENCY 100000000  // 200/2 = 100.00MHz  Overclocking
// #define SPI_FREQUENCY 125000000  // 250/2 = 125.00MHz  Overclocking

 
//RP2040         MHz      |     125  |   125  |   133  |   200  |   225  |   240  |   250  | 
//SPI            MHz      |    41,67 |  62,50 |  66,50 |  66,67 |  75,00 |  80,00 |  83,34 |
//Benchmark Time µs
//HaD pushColor           |   991385 | 499217 | 468883+| 619425 | 550510 | 516086 | 495483 | 
//Screen fill             |   122907 |  61474 |  57763+|  76812 |  68277 |  64011 |  61450 | 
//Text                    |    18848 |  12250 |  11602 |  12002 |  10628 |   9862 |   9548+| 
//Pixels                  |   631967 | 335160 | 314888 | 393710 | 350052 | 327912 | 314820+| 
//Lines                   |   539284 | 306844 | 288233 | 336136 | 298578 | 279823 | 268578+| 
//Horiz/Vert Lines        |    49965 |  25147 |  23613+|  31234 |  27763 |  26026 |  24992 | 
//Rectangles (outline)    |    27594 |  14001 |  13133+|  17253 |  15365 |  14403 |  13818 | 
//Rectangles (filled)     |  1497646 | 748896 | 703862+| 936047 | 832032 | 780038 | 748814 | 
//Circles (filled)        |   133280 |  69367 |  64902+|  83082 |  73793 |  69194 |  66403 | 
//Circles (outline)       |    58235 |  33090 |  30996 |  36297 |  32243 |  30206 |  28994+| 
//Triangles (outline)     |    31982 |  20649 |  19399 |  19976 |  17738 |  16622 |  15952+| 
//Triangles (filled)      |   473104 | 238287 | 223949+| 295703 | 262837 | 246391 | 236490 | 
//Rounded rects (outline) |    38961 |  21749 |  19741 |  23827 |  21189 |  19826 |  19029+| 
//Rounded rects (filled)  |  1493357 | 748021 | 702346+| 932549 | 828886 | 777132 | 746038 | 

//RP2040 175MHz SPI 87,50MHz RP2040 "bricked"
//RP2040 150MHz SPI 75,00MHz many distortions


// Optional reduced SPI frequency for reading TFT
//#define SPI_READ_FREQUENCY  16000000

//#define SPI_TOUCH_FREQUENCY  2500000

// #define SUPPORT_TRANSACTIONS
