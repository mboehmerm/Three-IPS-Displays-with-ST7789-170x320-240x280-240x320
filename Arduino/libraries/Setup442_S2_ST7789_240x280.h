// ST7789 170 x 320 display, Lolin S2 Mini
#define USER_SETUP_ID 442

#define ST7789_DRIVER     // Configure all registers
//#define TFT_SDA_READ   // Display has a bidirectional SDA pin

#define TFT_WIDTH  240
#define TFT_HEIGHT 280

//#define CGRAM_OFFSET      // Library will add offsets required

//#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
//#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF
#define TFT_BACKLIGHT_ON 1


// Lolin S2 Mini  HSPI    FSPI
#define TFT_CS     12   // 34  
#define TFT_MOSI   11   // 35 
#define TFT_SCLK    7   // 36
#define TFT_MISO    9   // 37 // Not connected
#define TFT_DC      5
#define TFT_BL     -1   //  3 // LED back-light
#define TFT_RST    -1   // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// FSPI port will be used unless the following is defined
#define USE_HSPI_PORT

//#define SPI_FREQUENCY        27000000
//#define SPI_FREQUENCY        40000000
#define SPI_FREQUENCY          80000000

#define SPI_READ_FREQUENCY      6000000  // 6 MHz is the maximum SPI read speed for the ST7789V

// #define SPI_TOUCH_FREQUENCY  2500000

// #define SUPPORT_TRANSACTIONS
