// ST7789 170 x 320 display with no chip select line
#define USER_SETUP_ID 410

#define ST7789_DRIVER     // Configure all registers
//#define TFT_SDA_READ   // Display has a bidirectional SDA pin

#define TFT_WIDTH  170
#define TFT_HEIGHT 320

//#define CGRAM_OFFSET      // Library will add offsets required

//#define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
//#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

//#define TFT_INVERSION_ON
//#define TFT_INVERSION_OFF
//#define TFT_BACKLIGHT_ON 1

//#define TFT_BL   18   // 18 // LED back-light  // TFT_BL -1 gibt Fehlermeldung bei verbose
#define TFT_MISO    5   //  5 // Not used
#define TFT_MOSI    6   //  6 // 17 HSPI ?
#define TFT_SCLK    4   //  4 // 18 HSPI ?
#define TFT_CS      7 
#define TFT_DC     19
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

// Solution for Tasmota Luatos Core ESP32 C3 
// Found :  https://github.com/espressif/arduino-esp32/issues/9618
// 
// The change done in IDF to #define REG_SPI_BASE(i) (((i)>=2) ? (DR_REG_SPI2_BASE + (i-2) * 0x1000) : (0)) // GPSPI2 and GPSPI3 is wrong.
// Correct is #define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (DR_REG_SPI0_BASE - ((i) * 0x1000))) // GPSPI2 and GPSPI3
// Changed this in our IDF fork for Tasmota, which is the base for compiling the Tasmota Arduino libs.
// Edit: The changes done for the other SOCs are imho wrong too. Only ESP32 looks correct.

// Edit C:\Users\<username>\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-bd2b9390ef\esp32c3\include\soc\esp32c3\include\soc\soc.h

// Replace "#define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (0))   // only one GPSPI"
// With    "#define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (DR_REG_SPI0_BASE - ((i) * 0x1000)))"

// Still necessary for ESP32 S3 
// FSPI / VSPI port (SPI2) used unless following defined. HSPI port is (SPI3) on S3.
//#define USE_HSPI_PORT                   // Necessary for ESP32 S3 

//#define SPI_FREQUENCY  20000000 
//#define SPI_FREQUENCY  27000000
//#define SPI_FREQUENCY  40000000
#define SPI_FREQUENCY  80000000

//#define SPI_READ_FREQUENCY  20000000

// #define SPI_TOUCH_FREQUENCY  2500000

// #define SUPPORT_TRANSACTIONS