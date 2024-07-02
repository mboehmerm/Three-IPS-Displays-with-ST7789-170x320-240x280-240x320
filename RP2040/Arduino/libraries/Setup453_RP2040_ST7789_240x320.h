
#define USER_SETUP_ID 453

#define ST7789_DRIVER            // Configure all registers
//#define TFT_SDA_READ           // Display has a bidirectional SDA pin

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

//#define CGRAM_OFFSET           // Library will add offsets required

//#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF
#define TFT_BACKLIGHT_ON 1


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

//#define TOUCH_CS   -1   // Chip select pin (T_CS) of touch screen

//Fonts
#define LOAD_GLCD       // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2      // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4      // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6      // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7      // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8      // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N   // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF      // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// RP2040 max frequency f is 125MHz / 2 = 62.5MHz. Take next higher integer.

// #define SPI_FREQUENCY  25000000  // 125/5 =  25.00MHz
// #define SPI_FREQUENCY  32000000  // 125/4 =  31.25MHz
// #define SPI_FREQUENCY  42000000  // 125/3 =  41.67MHz
// #define SPI_FREQUENCY  70000000  // 125/2 =  62.50MHz
// #define SPI_FREQUENCY 100000000  // 200/2 = 125.00MHz  Overclocking
   #define SPI_FREQUENCY 125000000  // 250/2 = 125.00MHz  Overclocking

// #define SPI_READ_FREQUENCY  20000000

// #define SPI_TOUCH_FREQUENCY  2500000

// #define SUPPORT_TRANSACTIONS