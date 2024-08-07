#define USER_SETUP_ID 435

#define ST7789_DRIVER     // Configure all registers
//#define TFT_SDA_READ   // Display has a bidirectional SDA pin

#define TFT_WIDTH  240
#define TFT_HEIGHT 240

//#define CGRAM_OFFSET      // Library will add offsets required

//#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
//#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

//#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF
//#define TFT_BACKLIGHT_ON 1

//Pins ESP32-H2
#define TFT_CS      0   // 16 = TX  // 18 // 7
#define TFT_MOSI   25   // 19  // 7
#define TFT_MISO   11   // 20  // 2
#define TFT_SCLK   10   // 21  // 6
#define TFT_DC      1
#define TFT_RST     3   // Set TFT_RST to -1 if display RESET is connected to ESP32 board EN
//#define TFT_BL    2   // LED back-light  // TFT_BL -1 gibt Fehlermeldung bei verbose

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT 

// Still necessary for ESP32 S3 
// FSPI / VSPI port (SPI2) used unless following defined. HSPI port is (SPI3) on S3.
//#define USE_HSPI_PORT

//#define SPI_FREQUENCY  10000000     // 80/8 // 40/4 // 547185 (HaD pushColor)
//#define SPI_FREQUENCY  14000000     // 80/6 // 40/3 // 415943
//#define SPI_FREQUENCY  20000000     // 80/4 // 40/2 // 284528
//#define SPI_FREQUENCY  27000000     // 80/3 // 40/2 // 284527  
//#define SPI_FREQUENCY  40000000     // 80/2 // 40/1 // 152672
#define SPI_FREQUENCY  80000000       // 80/1 // 40/1 // 152671 should be about 80000 at 80MHz

//#define SPI_READ_FREQUENCY  20000000

// #define SPI_TOUCH_FREQUENCY  2500000

// #define SUPPORT_TRANSACTIONS