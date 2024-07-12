/*
 Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

 See end of file for original header text and MIT license info.
 
 This sketch uses the GLCD font only.

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder. 

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */

//#include <../Setup442_S2_ST7789_240x280.h>      // ESP32-C3, ST7789, 80MHz


// !!!!!!!  for ESP32C3 Dev Module und Flash Mode auf DIO stellen !!!!!!!!!

#include "SPI.h"
#include "TFT_eSPI.h"

//#include <../Setup410_ST7789_320x170_C3.h>      // new : Setup file ST7789 ESP32 C3

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// Backlight Pin
const int pwmPin = 3;

unsigned long total = 0;
unsigned long tn = 0;
void setup() {
  Serial.begin(115200);
  //while (!Serial);
  Serial.println(""); 
  Serial.println("Bodmer's TFT_eSPI library Test!"); 

  #if defined(USE_HSPI_PORT)
  Serial.println(F("#define USE_HSPI_PORT")); 
  #elif defined(USE_VSPI_PORT)
  Serial.println(F("#define USE_VSPI_PORT")); 
  #elif defined(USE_FSPI_PORT)
  Serial.println(F("#define USE_FSPI_PORT")); 
  #else
  Serial.println(F("no #define USE_xSPI_PORT")); 
  #endif

  Serial.print("FSPI : ");  // 0
  Serial.println(FSPI); 

  Serial.print("HSPI : ");  // 1
  Serial.println(HSPI); 

  Serial.print("SPI_PORT : ");   // must be 2 ! // 1 bei 2.0.14
  Serial.println(SPI_PORT); 

  Serial.print("SPI2_HOST : ");  // 1
  Serial.println(SPI2_HOST); 

  Serial.print("DR_REG_SPI1_BASE : 0x");  //0x60002000
  Serial.println(DR_REG_SPI1_BASE,HEX); 

  Serial.print("DR_REG_SPI2_BASE : 0x");  //0x60024000
  Serial.println(DR_REG_SPI2_BASE,HEX); 
  
  Serial.print("REG_SPI_BASE(2) : 0x");   //0x60024000
  Serial.println(REG_SPI_BASE(2),HEX); 

Serial.println("start tft.init() ");
  tft.init(); 
Serial.println("done tft.init() ");

  // Backlight PWM Pin
  pinMode(pwmPin, OUTPUT);

  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, 130);
	tft.setTextSize(2);
  tft.print(F("Backlight Dimming"));
  
  delay(500);

  for (int i=255; i>0; i--) {
    analogWrite(pwmPin, i);
    delay(10);
    }
  for (int i=0; i<128; i++) {
    analogWrite(pwmPin, i);
    delay(10);
    }
  delay(500);
}

void loop(void)
{
  Serial.println("");
  Serial.print(F("Processor : ")); 
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
    Serial.println("ESP32-S3");
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    Serial.println("ESP32-S2");
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    Serial.println("ESP32-C3");  
  #elif defined (ESP32)
    Serial.println("ESP32");  
  #elif defined (ARDUINO_ARCH_ESP8266)
    Serial.println("ESP8266");  
  #elif defined (STM32)
    Serial.println("STM32");  
  #elif defined(ARDUINO_ARCH_RP2040)
    Serial.println("RP2040");  
  #elif defined(__LGT8FX8P__)
    Serial.println("LGT8FX");  
  #else
    Serial.println("Arduino");
  #endif

  Serial.print(F("ESP32     : ")); 
  Serial.print(ESP_ARDUINO_VERSION_MAJOR); 
  Serial.print("."); 
  Serial.print(ESP_ARDUINO_VERSION_MINOR); 
  Serial.print("."); 
  Serial.print(ESP_ARDUINO_VERSION_PATCH); 
  Serial.println("");

  Serial.print(F("TFT_eSPI  : ")); 
  Serial.println(TFT_ESPI_VERSION);
  
  Serial.print(F("SPI Write : ")); 
  Serial.print(SPI_FREQUENCY/1000000);
  Serial.println(F(" MHz")); 

  #if defined(USE_HSPI_PORT)
  Serial.println(F("#define USE_HSPI_PORT")); 
  #elif defined(USE_VSPI_PORT)
  Serial.println(F("#define USE_VSPI_PORT")); 
  #elif defined(USE_FSPI_PORT)
  Serial.println(F("#define USE_FSPI_PORT")); 
  #else
  Serial.println(F("no #define USE_xSPI_PORT")); 
  #endif

  Serial.print(F("MOSI ")); 
  Serial.print(TFT_MOSI);
  if (TFT_MOSI != MOSI) {
    Serial.print(F("/")); 
    Serial.print(MOSI);
  }  

  Serial.print(F(" - MISO ")); 
  Serial.print(TFT_MISO);
  if (TFT_MISO != MISO) {
    Serial.print(F("/")); 
    Serial.print(MISO);
  }

  Serial.print(F(" - SCLK ")); 
  Serial.print(TFT_SCLK);
  if (TFT_SCLK != SCK) {
    Serial.print(F("/")); 
    Serial.print(SCK);
  }
  Serial.print(F(" - CS  ")); 
  Serial.print(TFT_CS);
  if (TFT_CS != SS) {
    Serial.print(F("/")); 
    Serial.print(SS);
  }

  Serial.print(F(" - DC  ")); 
  Serial.print(TFT_DC);

  Serial.print(F(" - RST ")); 
  Serial.print(TFT_RST);

  #if defined(TOUCH_CS)
    Serial.print(F(" - TOUCH_CS ")); 
    Serial.println(TOUCH_CS);
  #endif

  Serial.println("");
	Serial.println(F("Benchmark                Time (microseconds)"));

	uint32_t usecHaD = testHaD();
	Serial.print(F("HaD pushColor            "));
	Serial.println(usecHaD);
	delay(500);

	uint32_t usecFillScreen = testFillScreen();
	Serial.print(F("Screen fill              "));
	Serial.println(usecFillScreen);
	delay(500);

	uint32_t usecText = testText();
	Serial.print(F("Text                     "));
	Serial.println(usecText);
	delay(500);

	uint32_t usecPixels = testPixels();
	Serial.print(F("Pixels                   "));
	Serial.println(usecPixels);
	delay(500);

	uint32_t usecLines = testLines(TFT_BLUE);
	Serial.print(F("Lines                    "));
	Serial.println(usecLines);
	delay(500);

	uint32_t usecFastLines = testFastLines(TFT_RED, TFT_BLUE);
	Serial.print(F("Horiz/Vert Lines         "));
	Serial.println(usecFastLines);
	delay(500);

	uint32_t usecRects = testRects(TFT_GREEN);
	Serial.print(F("Rectangles (outline)     "));
	Serial.println(usecRects);
	delay(500);

	uint32_t usecFilledRects = testFilledRects(TFT_YELLOW, TFT_MAGENTA);
	Serial.print(F("Rectangles (filled)      "));
	Serial.println(usecFilledRects);
	delay(500);

	uint32_t usecFilledCircles = testFilledCircles(10, TFT_MAGENTA);
	Serial.print(F("Circles (filled)         "));
	Serial.println(usecFilledCircles);
	delay(500);

	uint32_t usecCircles = testCircles(10, TFT_WHITE);
	Serial.print(F("Circles (outline)        "));
	Serial.println(usecCircles);
	delay(500);

	uint32_t usecTriangles = testTriangles();
	Serial.print(F("Triangles (outline)      "));
	Serial.println(usecTriangles);
	delay(500);

	uint32_t usecFilledTrangles = testFilledTriangles();
	Serial.print(F("Triangles (filled)       "));
	Serial.println(usecFilledTrangles);
	delay(500);

	uint32_t usecRoundRects = testRoundRects();
	Serial.print(F("Rounded rects (outline)  "));
	Serial.println(usecRoundRects);
	delay(500);

	uint32_t usedFilledRoundRects = testFilledRoundRects();
	Serial.print(F("Rounded rects (filled)   "));
	Serial.println(usedFilledRoundRects);
	delay(500);

	Serial.println(F("Done!"));

	uint16_t c = 4;
	int8_t d = 1;
	for (int32_t i = 0; i < tft.height(); i++)
	{
		tft.drawFastHLine(0, i, tft.width(), c);
		c += d;
		if (c <= 4 || c >= 11)
			d = -d;
	}
	
	tft.setCursor(0, 0);
	tft.setTextColor(TFT_MAGENTA);
	tft.setTextSize(2);

	tft.print(F("   "));
 	tft.println(F("TFT_eSPI test"));

	tft.setTextSize(1);
/*
	tft.setTextColor(TFT_WHITE);
	tft.println(F(""));
	tft.setTextSize(1);
	tft.println(F(""));
	tft.setTextColor(tft.color565(0x80, 0x80, 0x80));

	tft.println(F(""));
*/

	tft.setTextColor(TFT_GREEN);
	tft.print(F("      "));
	tft.println(F("Benchmark       microseconds"));
	tft.println(F(""));
	tft.setTextColor(TFT_YELLOW);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("HaD pushColor "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecHaD);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Screen fill   "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecFillScreen);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Text          "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecText);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Pixels        "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecPixels);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Lines         "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecLines);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Hor/Vert Line "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecFastLines);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Rectangles    "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecRects);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Rect.-filled  "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecFilledRects);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Circles       "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecCircles);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Circ.-filled  "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecFilledCircles);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Triangles     "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecTriangles);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Triang-filled "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecFilledTrangles);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Rounded rects "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usecRoundRects);

	tft.setTextColor(TFT_CYAN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.print(F("Round R.-fill "));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	printnice(usedFilledRoundRects);

	tft.setTextSize(1);
	//tft.println(F(""));
	tft.setTextColor(TFT_GREEN); tft.setTextSize(1);
	tft.print(F("      "));
	tft.println(F("Benchmark Complete!"));
  //tft.println(F(""));

	delay(10 * 1000L);

  tft.fillScreen(TFT_BLACK);

	tft.setCursor(0, 40);
	tft.setTextColor(TFT_WHITE);
	tft.setTextSize(2);

  tft.print(F("Processor : ")); 
  #if defined(CONFIG_IDF_TARGET_ESP32S3)
    tft.println("ESP32-S3");
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    tft.println("ESP32-S2");
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    tft.println("ESP32-C3");  
  #elif defined (ESP32)
    tft.println("ESP32");  
  #elif defined (ARDUINO_ARCH_ESP8266)
    tft.println("ESP8266");  
  #elif defined (STM32)
    tft.println("STM32");  
  #elif defined(ARDUINO_ARCH_RP2040)
    tft.println("RP2040");  
  #elif defined(__LGT8FX8P__)
    tft.println("LGT8FX");  
  #else
    tft.println("Arduino");
  #endif
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+4); 


  tft.print(F("ESP32     : ")); 
  tft.print(ESP_ARDUINO_VERSION_MAJOR); 
  tft.print("."); 
  tft.print(ESP_ARDUINO_VERSION_MINOR); 
  tft.print("."); 
  tft.print(ESP_ARDUINO_VERSION_PATCH); 
  //tft.print(" 0x"); 
  //tft.print(ESP_ARDUINO_VERSION,HEX); 
  tft.println("");
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+2); 

  tft.print(F("TFT_eSPI  : ")); 
  tft.println(TFT_ESPI_VERSION);
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+2);
  
  tft.print(F("SPI Write : ")); 
  tft.print(SPI_FREQUENCY/1000000);
  tft.println(F(" MHz")); 
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+8);

  tft.print(F("MOSI ")); 
  tft.print(TFT_MOSI);
  if (TFT_MOSI != MOSI) {
    tft.print(F("/")); 
    tft.print(MOSI);
  }  
  tft.print(F("  CS  ")); 
  tft.print(TFT_CS);
  if (TFT_CS != SS) {
    tft.print(F("/")); 
    tft.print(SS);
  }
  tft.println("");
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+2);

  tft.print(F("MISO ")); 
  tft.print(TFT_MISO);
  if (TFT_MISO != MISO) {
    tft.print(F("/")); 
    tft.print(MISO);
  }
  tft.print(F("  DC  ")); 
  tft.print(TFT_DC);
  tft.println("");
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+2);

  tft.print(F("SCLK ")); 
  tft.print(TFT_SCLK);
  if (TFT_SCLK != SCK) {
    tft.print(F("/")); 
    tft.print(SCK);
  }
  tft.print(F("  RST ")); 
  tft.print(TFT_RST);
  tft.println("");
  tft.setCursor(tft.getCursorX(), tft.getCursorY()+2);

/*
  #if defined(TOUCH_CS)
    tft.print(F("TOUCH_CS ")); 
    tft.println(TOUCH_CS);
  #endif
*/
  #if defined(USE_HSPI_PORT)
  tft.println(F("#define USE_HSPI_PORT")); 
  #elif defined(USE_VSPI_PORT)
  tft.println(F("#define USE_VSPI_PORT")); 
  #elif defined(USE_FSPI_PORT)
  tft.println(F("#define USE_FSPI_PORT")); 
  #else
  tft.println(F("no USE_xSPI_PORT")); 
  #endif

  //tft.print(F("Arduino IDE: ")); 
  //tft.println(ARDUINO);

	//tft.setTextSize(1);
  //tft.print(F("ESP IDF: ")); 
  //tft.println(esp_get_idf_version());

	delay(30000);
}

void printnice(int32_t v)
{
	char	str[32] = { 0 };
	sprintf(str, "%d", v);
	for (char *p = (str+strlen(str))-3; p > str; p -= 3)
	{
		memmove(p+1, p, strlen(p)+1);
		*p = ',';
		
	}
	while (strlen(str) < 7)
	{
		memmove(str+1, str, strlen(str)+1);
		*str = ' ';
	}
	tft.println(str);
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
static inline uint32_t micros_start()
{
	uint8_t oms = millis();
	while ((uint8_t)millis() == oms)
		;
	return micros();
}

uint32_t testHaD()
{
  // pseudo-code for cheesy RLE
  // start with color1
  // while more input data remaining
  //  count =  0nnnnnnn = 1 byte or 1nnnnnnn nnnnnnnn 2 bytes (0 - 32767)
  //  repeat color count times
  //  toggle color1/color2
  static const uint8_t HaD_128x160[] PROGMEM =
  {
    0x85, 0x91, 0x09, 0x4b, 0x09, 0x24, 0x0a, 0x47, 0x09, 0x27, 0x0a, 0x44, 0x0a, 0x29, 0x0a, 0x42, 
    0x0a, 0x2b, 0x0a, 0x41, 0x0a, 0x2c, 0x0a, 0x3e, 0x0b, 0x2f, 0x09, 0x3d, 0x09, 0x32, 0x08, 0x3c, 
    0x09, 0x33, 0x09, 0x3b, 0x08, 0x33, 0x0a, 0x3a, 0x0a, 0x31, 0x0b, 0x3a, 0x0c, 0x1d, 0x01, 0x10, 
    0x0d, 0x39, 0x0c, 0x1d, 0x01, 0x10, 0x0d, 0x39, 0x0d, 0x0f, 0x01, 0x0c, 0x03, 0x0d, 0x0e, 0x39, 
    0x0e, 0x0c, 0x03, 0x0c, 0x04, 0x0b, 0x0f, 0x39, 0x0f, 0x0a, 0x04, 0x0c, 0x05, 0x09, 0x10, 0x39, 
    0x10, 0x08, 0x05, 0x0c, 0x06, 0x07, 0x11, 0x39, 0x11, 0x06, 0x06, 0x0d, 0x07, 0x04, 0x13, 0x37, 
    0x12, 0x05, 0x07, 0x0d, 0x08, 0x02, 0x15, 0x34, 0x15, 0x03, 0x08, 0x0d, 0x20, 0x32, 0x20, 0x0e, 
    0x21, 0x31, 0x20, 0x0f, 0x21, 0x2e, 0x22, 0x10, 0x22, 0x2b, 0x22, 0x12, 0x22, 0x12, 0x05, 0x12, 
    0x22, 0x14, 0x22, 0x0c, 0x0f, 0x0c, 0x22, 0x16, 0x22, 0x08, 0x15, 0x08, 0x22, 0x18, 0x22, 0x05, 
    0x19, 0x05, 0x21, 0x1c, 0x1f, 0x04, 0x1c, 0x05, 0x1f, 0x1f, 0x1c, 0x04, 0x1e, 0x04, 0x1d, 0x2b, 
    0x11, 0x04, 0x21, 0x03, 0x12, 0x36, 0x0f, 0x03, 0x24, 0x03, 0x10, 0x38, 0x0d, 0x03, 0x26, 0x03, 
    0x0d, 0x3b, 0x0b, 0x03, 0x28, 0x03, 0x0b, 0x3d, 0x0a, 0x03, 0x29, 0x03, 0x09, 0x40, 0x07, 0x03, 
    0x2b, 0x03, 0x07, 0x42, 0x05, 0x03, 0x2c, 0x04, 0x04, 0x45, 0x04, 0x03, 0x2d, 0x03, 0x04, 0x46, 
    0x02, 0x03, 0x2e, 0x03, 0x03, 0x48, 0x01, 0x03, 0x2f, 0x03, 0x01, 0x4c, 0x31, 0x4e, 0x32, 0x4e, 
    0x33, 0x4c, 0x34, 0x4c, 0x34, 0x4c, 0x35, 0x4b, 0x35, 0x4a, 0x0e, 0x03, 0x14, 0x04, 0x0d, 0x4a, 
    0x0b, 0x09, 0x0e, 0x0a, 0x0a, 0x4a, 0x0a, 0x0b, 0x0c, 0x0c, 0x09, 0x4a, 0x09, 0x0d, 0x0a, 0x0e, 
    0x09, 0x49, 0x08, 0x0f, 0x09, 0x0e, 0x09, 0x49, 0x08, 0x0f, 0x09, 0x0f, 0x08, 0x49, 0x08, 0x0f, 
    0x09, 0x0f, 0x08, 0x49, 0x07, 0x0f, 0x0a, 0x0f, 0x08, 0x49, 0x07, 0x0f, 0x0b, 0x0e, 0x08, 0x49, 
    0x07, 0x0d, 0x0e, 0x0d, 0x08, 0x49, 0x07, 0x0b, 0x13, 0x0a, 0x08, 0x49, 0x08, 0x07, 0x18, 0x08, 
    0x08, 0x49, 0x08, 0x06, 0x1b, 0x06, 0x08, 0x49, 0x09, 0x04, 0x1c, 0x05, 0x08, 0x4a, 0x09, 0x04, 
    0x1d, 0x04, 0x08, 0x4a, 0x0a, 0x03, 0x1d, 0x03, 0x09, 0x4b, 0x19, 0x02, 0x1a, 0x4b, 0x19, 0x03, 
    0x19, 0x4b, 0x18, 0x04, 0x18, 0x4d, 0x17, 0x05, 0x17, 0x4a, 0x01, 0x02, 0x17, 0x05, 0x16, 0x4a, 
    0x02, 0x02, 0x17, 0x05, 0x16, 0x02, 0x03, 0x44, 0x03, 0x03, 0x16, 0x02, 0x01, 0x02, 0x16, 0x02, 
    0x03, 0x43, 0x05, 0x03, 0x15, 0x01, 0x03, 0x01, 0x15, 0x03, 0x04, 0x41, 0x06, 0x03, 0x15, 0x01, 
    0x03, 0x01, 0x14, 0x03, 0x07, 0x3d, 0x09, 0x03, 0x2d, 0x03, 0x08, 0x3b, 0x0a, 0x04, 0x2b, 0x03, 
    0x0a, 0x39, 0x0c, 0x03, 0x2a, 0x04, 0x0b, 0x37, 0x0e, 0x03, 0x28, 0x03, 0x0e, 0x2e, 0x04, 0x03, 
    0x10, 0x03, 0x27, 0x03, 0x10, 0x03, 0x03, 0x24, 0x19, 0x03, 0x26, 0x03, 0x1a, 0x1e, 0x1d, 0x03, 
    0x24, 0x03, 0x1e, 0x19, 0x20, 0x04, 0x21, 0x03, 0x20, 0x17, 0x22, 0x04, 0x1f, 0x03, 0x22, 0x15, 
    0x22, 0x04, 0x21, 0x04, 0x21, 0x13, 0x22, 0x05, 0x15, 0x01, 0x0b, 0x05, 0x21, 0x12, 0x21, 0x06, 
    0x15, 0x01, 0x0b, 0x06, 0x21, 0x10, 0x21, 0x07, 0x0a, 0x01, 0x0a, 0x01, 0x0b, 0x07, 0x21, 0x0e, 
    0x20, 0x0a, 0x09, 0x02, 0x09, 0x02, 0x09, 0x09, 0x20, 0x0e, 0x08, 0x02, 0x15, 0x0b, 0x08, 0x03, 
    0x08, 0x03, 0x08, 0x0b, 0x15, 0x03, 0x08, 0x0d, 0x07, 0x04, 0x13, 0x0d, 0x06, 0x05, 0x06, 0x06, 
    0x05, 0x0d, 0x14, 0x04, 0x07, 0x0c, 0x07, 0x06, 0x11, 0x38, 0x12, 0x06, 0x06, 0x0c, 0x06, 0x08, 
    0x10, 0x39, 0x10, 0x08, 0x05, 0x0c, 0x04, 0x0b, 0x0f, 0x39, 0x0f, 0x0a, 0x04, 0x0c, 0x03, 0x0d, 
    0x0e, 0x39, 0x0e, 0x0c, 0x03, 0x0c, 0x02, 0x0e, 0x0e, 0x39, 0x0d, 0x0f, 0x01, 0x0c, 0x01, 0x10, 
    0x0d, 0x39, 0x0d, 0x0f, 0x01, 0x1e, 0x0c, 0x39, 0x0c, 0x30, 0x0a, 0x3a, 0x0a, 0x33, 0x09, 0x3b, 
    0x08, 0x34, 0x09, 0x3b, 0x09, 0x32, 0x09, 0x3c, 0x0a, 0x2f, 0x0a, 0x3e, 0x0a, 0x2d, 0x0b, 0x3f, 
    0x0a, 0x2b, 0x0b, 0x41, 0x0a, 0x29, 0x0b, 0x43, 0x0a, 0x27, 0x0a, 0x46, 0x0a, 0x25, 0x0a, 0x49, 
    0x09, 0x23, 0x08, 0x4e, 0x08, 0x96, 0x12 
  };

  tft.fillScreen(TFT_BLACK);

  uint32_t start = micros_start();

  tft.startWrite();

  for (int i = 0; i < 0x10; i++)
  {
    //tft.setAddrWindow(0, 0, tft.width(), tft.height());
     tft.setAddrWindow(55, 59, 128, 160); // for Display 240x280

    uint16_t cnt = 0;
    uint16_t color = tft.color565((i << 4) | i, (i << 4) | i, (i << 4) | i);
    uint16_t curcolor = 0;

    const uint8_t *cmp = &HaD_128x160[0];
    tft.startWrite();
    while (cmp < &HaD_128x160[sizeof(HaD_128x160)])
    {
      cnt = pgm_read_byte(cmp++);
      if (cnt & 0x80)
        cnt = ((cnt & 0x7f) << 8) | pgm_read_byte(cmp++);

      tft.pushColor(curcolor, cnt); // PDQ_GFX has count

      curcolor ^= color;
    }
    tft.endWrite();
  }

  tft.endWrite();

  uint32_t t = micros() - start;

  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(0, 225);
  tft.println(F("http://hackaday.io/     Xark"));

  delay(3 * 1000L);
  
  return t;
}

/*
uint32_t testHaD()
{
	// pseudo-code for cheesy RLE
	// start with color1
	// while more input data remaining
	// 	count =  0nnnnnnn = 1 byte or 1nnnnnnn nnnnnnnn 2 bytes (0 - 32767)
	// 	repeat color count times
	// 	toggle color1/color2
	static const uint8_t HaD_240x320[] PROGMEM =
	{
		0xb9, 0x50, 0x0e, 0x80, 0x93, 0x0e, 0x41, 0x11, 0x80, 0x8d, 0x11, 0x42, 0x12, 0x80, 0x89, 0x12, 
		0x45, 0x12, 0x80, 0x85, 0x12, 0x48, 0x12, 0x80, 0x83, 0x12, 0x4a, 0x13, 0x7f, 0x13, 0x4c, 0x13, 
		0x7d, 0x13, 0x4e, 0x13, 0x7b, 0x13, 0x50, 0x13, 0x79, 0x13, 0x52, 0x13, 0x77, 0x13, 0x54, 0x13, 
		0x75, 0x13, 0x57, 0x11, 0x75, 0x11, 0x5a, 0x11, 0x73, 0x11, 0x5c, 0x11, 0x71, 0x11, 0x5e, 0x10, 
		0x71, 0x10, 0x60, 0x10, 0x6f, 0x10, 0x61, 0x10, 0x6f, 0x10, 0x60, 0x11, 0x6f, 0x11, 0x5e, 0x13, 
		0x6d, 0x13, 0x5c, 0x14, 0x6d, 0x14, 0x5a, 0x15, 0x6d, 0x15, 0x58, 0x17, 0x6b, 0x17, 0x37, 0x01, 
		0x1f, 0x17, 0x6b, 0x17, 0x1f, 0x01, 0x17, 0x02, 0x1d, 0x18, 0x6b, 0x18, 0x1d, 0x02, 0x17, 0x03, 
		0x1b, 0x19, 0x6b, 0x19, 0x1b, 0x03, 0x17, 0x05, 0x18, 0x1a, 0x6b, 0x1a, 0x18, 0x05, 0x17, 0x06, 
		0x16, 0x1b, 0x6b, 0x1b, 0x16, 0x06, 0x17, 0x07, 0x14, 0x1c, 0x6b, 0x1c, 0x14, 0x07, 0x17, 0x08, 
		0x12, 0x1d, 0x6b, 0x1d, 0x12, 0x08, 0x17, 0x09, 0x10, 0x1e, 0x6b, 0x1e, 0x10, 0x09, 0x17, 0x0a, 
		0x0e, 0x1f, 0x6b, 0x1f, 0x0e, 0x0a, 0x17, 0x0b, 0x0c, 0x20, 0x6b, 0x20, 0x0c, 0x0b, 0x17, 0x0c, 
		0x0b, 0x21, 0x69, 0x21, 0x0b, 0x0c, 0x18, 0x0d, 0x08, 0x23, 0x67, 0x23, 0x08, 0x0d, 0x19, 0x0e, 
		0x06, 0x26, 0x63, 0x26, 0x06, 0x0e, 0x19, 0x0f, 0x04, 0x28, 0x61, 0x28, 0x04, 0x0f, 0x19, 0x10, 
		0x02, 0x2a, 0x5f, 0x2a, 0x02, 0x10, 0x1a, 0x3c, 0x5d, 0x3c, 0x1b, 0x3d, 0x5b, 0x3d, 0x1c, 0x3d, 
		0x59, 0x3d, 0x1d, 0x3e, 0x57, 0x3e, 0x1e, 0x3e, 0x55, 0x3e, 0x1f, 0x40, 0x51, 0x40, 0x20, 0x40, 
		0x4f, 0x40, 0x22, 0x40, 0x22, 0x09, 0x22, 0x40, 0x24, 0x40, 0x1a, 0x17, 0x1a, 0x40, 0x26, 0x40, 
		0x16, 0x1d, 0x16, 0x40, 0x28, 0x40, 0x12, 0x23, 0x12, 0x40, 0x2a, 0x40, 0x0f, 0x27, 0x0f, 0x40, 
		0x2c, 0x41, 0x0b, 0x2b, 0x0b, 0x41, 0x2f, 0x3f, 0x09, 0x2f, 0x09, 0x3f, 0x32, 0x3d, 0x08, 0x33, 
		0x08, 0x3d, 0x35, 0x3a, 0x08, 0x35, 0x08, 0x3a, 0x3a, 0x36, 0x07, 0x39, 0x07, 0x36, 0x41, 0x09, 
		0x05, 0x23, 0x07, 0x3b, 0x07, 0x23, 0x05, 0x09, 0x54, 0x21, 0x07, 0x3d, 0x07, 0x21, 0x64, 0x1f, 
		0x06, 0x41, 0x06, 0x1f, 0x66, 0x1d, 0x06, 0x43, 0x06, 0x1d, 0x68, 0x1b, 0x06, 0x45, 0x06, 0x1b, 
		0x6b, 0x18, 0x06, 0x47, 0x06, 0x18, 0x6e, 0x16, 0x06, 0x49, 0x06, 0x16, 0x70, 0x14, 0x06, 0x4b, 
		0x06, 0x14, 0x72, 0x13, 0x06, 0x4b, 0x06, 0x13, 0x74, 0x11, 0x06, 0x4d, 0x06, 0x11, 0x76, 0x0f, 
		0x06, 0x4f, 0x06, 0x0f, 0x78, 0x0e, 0x05, 0x51, 0x05, 0x0e, 0x7a, 0x0c, 0x06, 0x51, 0x06, 0x0c, 
		0x7d, 0x09, 0x06, 0x53, 0x06, 0x09, 0x80, 0x80, 0x08, 0x05, 0x55, 0x05, 0x08, 0x80, 0x82, 0x06, 
		0x05, 0x57, 0x05, 0x06, 0x80, 0x84, 0x05, 0x05, 0x57, 0x05, 0x05, 0x80, 0x86, 0x03, 0x05, 0x59, 
		0x05, 0x03, 0x80, 0x88, 0x02, 0x05, 0x59, 0x05, 0x02, 0x80, 0x8f, 0x5b, 0x80, 0x95, 0x5b, 0x80, 
		0x94, 0x5d, 0x80, 0x93, 0x5d, 0x80, 0x92, 0x5e, 0x80, 0x92, 0x5f, 0x80, 0x91, 0x5f, 0x80, 0x90, 
		0x61, 0x80, 0x8f, 0x61, 0x80, 0x8f, 0x61, 0x80, 0x8e, 0x63, 0x80, 0x8d, 0x63, 0x80, 0x8d, 0x63, 
		0x80, 0x8d, 0x63, 0x80, 0x8c, 0x19, 0x07, 0x25, 0x07, 0x19, 0x80, 0x8b, 0x16, 0x0d, 0x1f, 0x0d, 
		0x16, 0x80, 0x8b, 0x14, 0x11, 0x1b, 0x11, 0x14, 0x80, 0x8b, 0x13, 0x13, 0x19, 0x13, 0x13, 0x80, 
		0x8b, 0x12, 0x15, 0x17, 0x15, 0x12, 0x80, 0x8a, 0x12, 0x17, 0x15, 0x17, 0x12, 0x80, 0x89, 0x11, 
		0x19, 0x13, 0x19, 0x11, 0x80, 0x89, 0x11, 0x19, 0x13, 0x19, 0x11, 0x80, 0x89, 0x10, 0x1b, 0x11, 
		0x1b, 0x10, 0x80, 0x89, 0x0f, 0x1c, 0x11, 0x1c, 0x0f, 0x80, 0x89, 0x0f, 0x1c, 0x11, 0x1c, 0x0f, 
		0x80, 0x89, 0x0f, 0x1c, 0x11, 0x1c, 0x0f, 0x80, 0x89, 0x0e, 0x1d, 0x11, 0x1d, 0x0e, 0x80, 0x89, 
		0x0e, 0x1c, 0x13, 0x1c, 0x0e, 0x80, 0x89, 0x0e, 0x1b, 0x15, 0x1b, 0x0e, 0x80, 0x89, 0x0e, 0x1b, 
		0x15, 0x1b, 0x0e, 0x80, 0x89, 0x0e, 0x1a, 0x17, 0x1a, 0x0e, 0x80, 0x89, 0x0e, 0x18, 0x1b, 0x18, 
		0x0e, 0x80, 0x89, 0x0e, 0x16, 0x1f, 0x16, 0x0e, 0x80, 0x89, 0x0e, 0x14, 0x23, 0x14, 0x0e, 0x80, 
		0x89, 0x0f, 0x11, 0x27, 0x11, 0x0f, 0x80, 0x89, 0x0f, 0x0e, 0x2d, 0x0e, 0x0f, 0x80, 0x89, 0x0f, 
		0x0c, 0x31, 0x0c, 0x0f, 0x80, 0x89, 0x0f, 0x0b, 0x33, 0x0b, 0x0f, 0x80, 0x8a, 0x0f, 0x09, 0x35, 
		0x09, 0x0f, 0x80, 0x8b, 0x10, 0x08, 0x35, 0x08, 0x10, 0x80, 0x8b, 0x10, 0x07, 0x37, 0x07, 0x10, 
		0x80, 0x8b, 0x11, 0x06, 0x37, 0x06, 0x11, 0x80, 0x8b, 0x12, 0x05, 0x37, 0x05, 0x12, 0x80, 0x8c, 
		0x13, 0x03, 0x1b, 0x01, 0x1b, 0x03, 0x13, 0x80, 0x8d, 0x30, 0x03, 0x30, 0x80, 0x8d, 0x30, 0x04, 
		0x2f, 0x80, 0x8d, 0x2f, 0x05, 0x2f, 0x80, 0x8e, 0x2e, 0x06, 0x2d, 0x80, 0x8f, 0x2d, 0x07, 0x2d, 
		0x80, 0x8f, 0x2d, 0x07, 0x2d, 0x80, 0x90, 0x2c, 0x08, 0x2b, 0x80, 0x91, 0x2b, 0x09, 0x2b, 0x80, 
		0x8c, 0x01, 0x05, 0x2a, 0x09, 0x2a, 0x05, 0x01, 0x80, 0x85, 0x03, 0x05, 0x2a, 0x09, 0x2a, 0x05, 
		0x03, 0x80, 0x82, 0x04, 0x05, 0x2a, 0x09, 0x2a, 0x04, 0x05, 0x80, 0x80, 0x06, 0x05, 0x29, 0x04, 
		0x02, 0x03, 0x29, 0x05, 0x06, 0x7e, 0x07, 0x05, 0x29, 0x03, 0x03, 0x03, 0x29, 0x05, 0x07, 0x7c, 
		0x09, 0x05, 0x28, 0x02, 0x05, 0x02, 0x28, 0x05, 0x09, 0x7a, 0x0a, 0x05, 0x28, 0x02, 0x05, 0x02, 
		0x28, 0x05, 0x0a, 0x78, 0x0c, 0x05, 0x27, 0x02, 0x05, 0x02, 0x27, 0x05, 0x0c, 0x76, 0x0d, 0x06, 
		0x26, 0x01, 0x07, 0x01, 0x26, 0x06, 0x0d, 0x73, 0x10, 0x05, 0x55, 0x05, 0x10, 0x70, 0x12, 0x05, 
		0x53, 0x05, 0x12, 0x6e, 0x13, 0x06, 0x51, 0x06, 0x13, 0x6c, 0x15, 0x05, 0x51, 0x05, 0x15, 0x6a, 
		0x16, 0x06, 0x4f, 0x06, 0x16, 0x68, 0x18, 0x06, 0x4d, 0x06, 0x18, 0x66, 0x1a, 0x06, 0x4b, 0x06, 
		0x1a, 0x64, 0x1c, 0x06, 0x49, 0x06, 0x1c, 0x55, 0x07, 0x05, 0x1e, 0x06, 0x49, 0x06, 0x1e, 0x05, 
		0x07, 0x42, 0x30, 0x06, 0x47, 0x06, 0x30, 0x3a, 0x34, 0x06, 0x45, 0x06, 0x34, 0x35, 0x37, 0x06, 
		0x43, 0x06, 0x37, 0x32, 0x39, 0x07, 0x3f, 0x07, 0x39, 0x2f, 0x3c, 0x07, 0x3d, 0x07, 0x3c, 0x2c, 
		0x3e, 0x07, 0x3b, 0x07, 0x3e, 0x2a, 0x40, 0x06, 0x3b, 0x06, 0x40, 0x28, 0x40, 0x06, 0x3c, 0x07, 
		0x40, 0x26, 0x3f, 0x08, 0x3d, 0x08, 0x3f, 0x24, 0x3f, 0x09, 0x3d, 0x09, 0x3f, 0x22, 0x3f, 0x0a, 
		0x14, 0x01, 0x13, 0x02, 0x13, 0x0a, 0x3f, 0x20, 0x3f, 0x0b, 0x14, 0x01, 0x13, 0x02, 0x13, 0x0b, 
		0x3f, 0x1f, 0x3e, 0x0c, 0x14, 0x01, 0x13, 0x02, 0x13, 0x0c, 0x3e, 0x1e, 0x3e, 0x0d, 0x13, 0x02, 
		0x13, 0x02, 0x13, 0x0d, 0x3e, 0x1d, 0x3d, 0x0e, 0x13, 0x02, 0x13, 0x02, 0x13, 0x0e, 0x3d, 0x1c, 
		0x3c, 0x11, 0x11, 0x04, 0x11, 0x04, 0x11, 0x11, 0x3c, 0x1b, 0x10, 0x01, 0x2a, 0x12, 0x11, 0x04, 
		0x11, 0x04, 0x11, 0x12, 0x2a, 0x01, 0x10, 0x1a, 0x0f, 0x04, 0x28, 0x14, 0x0f, 0x06, 0x0f, 0x06, 
		0x0f, 0x14, 0x28, 0x04, 0x0f, 0x19, 0x0e, 0x06, 0x26, 0x16, 0x0d, 0x08, 0x0d, 0x08, 0x0d, 0x16, 
		0x26, 0x06, 0x0e, 0x19, 0x0d, 0x07, 0x25, 0x18, 0x0b, 0x0a, 0x0b, 0x0a, 0x0b, 0x18, 0x25, 0x07, 
		0x0d, 0x19, 0x0c, 0x09, 0x23, 0x1c, 0x06, 0x0f, 0x05, 0x10, 0x05, 0x1c, 0x23, 0x09, 0x0c, 0x18, 
		0x0c, 0x0b, 0x21, 0x69, 0x21, 0x0b, 0x0c, 0x17, 0x0b, 0x0d, 0x1f, 0x6b, 0x1f, 0x0d, 0x0b, 0x17, 
		0x0a, 0x0f, 0x1e, 0x6b, 0x1e, 0x0f, 0x0a, 0x17, 0x09, 0x11, 0x1d, 0x6b, 0x1d, 0x11, 0x09, 0x17, 
		0x07, 0x14, 0x1c, 0x6b, 0x1c, 0x14, 0x07, 0x17, 0x06, 0x16, 0x1b, 0x6b, 0x1b, 0x16, 0x06, 0x17, 
		0x05, 0x18, 0x1a, 0x6b, 0x1a, 0x18, 0x05, 0x17, 0x04, 0x1a, 0x19, 0x6b, 0x19, 0x1a, 0x04, 0x17, 
		0x03, 0x1b, 0x19, 0x6b, 0x19, 0x1b, 0x03, 0x17, 0x02, 0x1d, 0x18, 0x6b, 0x18, 0x1d, 0x02, 0x37, 
		0x17, 0x6b, 0x17, 0x58, 0x16, 0x6b, 0x16, 0x5a, 0x14, 0x6d, 0x14, 0x5c, 0x13, 0x6d, 0x13, 0x5e, 
		0x12, 0x6d, 0x12, 0x60, 0x10, 0x6f, 0x10, 0x61, 0x10, 0x6f, 0x10, 0x60, 0x11, 0x6f, 0x11, 0x5e, 
		0x11, 0x71, 0x11, 0x5c, 0x12, 0x71, 0x12, 0x5a, 0x12, 0x73, 0x12, 0x58, 0x12, 0x75, 0x12, 0x56, 
		0x13, 0x75, 0x13, 0x54, 0x13, 0x77, 0x13, 0x51, 0x14, 0x79, 0x14, 0x4e, 0x14, 0x7b, 0x14, 0x4c, 
		0x14, 0x7d, 0x14, 0x4a, 0x14, 0x7f, 0x14, 0x48, 0x13, 0x80, 0x83, 0x13, 0x46, 0x13, 0x80, 0x85, 
		0x13, 0x44, 0x12, 0x80, 0x89, 0x12, 0x42, 0x11, 0x80, 0x8d, 0x11, 0x40, 0x0f, 0x80, 0x93, 0x0f, 
		0x45, 0x04, 0x80, 0x9d, 0x04, 0xb9, 0x56, 
	};
	
	tft.fillScreen(TFT_BLACK);

	uint32_t start = micros_start();

	for (int i = 0; i < 0x10; i++)
	{
		tft.setAddrWindow(0, 0, 170, 320); ////

		uint16_t cnt = 0;
		uint16_t color = tft.color565((i << 4) | i, (i << 4) | i, (i << 4) | i);
		uint16_t curcolor = 0;

		const uint8_t *cmp = &HaD_240x320[0];

		tft.startWrite();
		while (cmp < &HaD_240x320[sizeof(HaD_240x320)])
		{
			cnt = pgm_read_byte(cmp++);
			if (cnt & 0x80) cnt = ((cnt & 0x7f) << 8) | pgm_read_byte(cmp++);
			tft.pushColor(curcolor, cnt);	// PDQ_GFX has count
			curcolor ^= color;
		}
		tft.endWrite();
	}

	uint32_t t = micros() - start;

	tft.setTextColor(TFT_YELLOW);
	tft.setTextSize(1);
	tft.setCursor(8, 285);
	tft.print(F("http://hackaday.io/"));
	tft.setCursor(96, 302);
	tft.print(F("Xark"));

	delay(3 * 1000L);
	
	return t;
}

*/

uint32_t testFillScreen()
{
	uint32_t start = micros_start();
    // Shortened this tedious test!
		tft.fillScreen(TFT_WHITE);
		tft.fillScreen(TFT_RED);
		tft.fillScreen(TFT_GREEN);
		tft.fillScreen(TFT_BLUE);
		tft.fillScreen(TFT_BLACK);

	return (micros() - start)/5;
}

uint32_t testText()
{
	tft.fillScreen(TFT_BLACK);
	uint32_t start = micros_start();
	tft.setCursor(0, 0);
	tft.setTextColor(TFT_WHITE,TFT_BLACK);	tft.setTextSize(1);
	tft.println(F("Hello World!"));
	tft.setTextSize(2);
	tft.setTextColor(tft.color565(0xff, 0x00, 0x00));
	tft.print(F("RED "));
	tft.setTextColor(tft.color565(0x00, 0xff, 0x00));
	tft.print(F("GREEN "));
	tft.setTextColor(tft.color565(0x00, 0x00, 0xff));
	tft.println(F("BLUE"));
	tft.setTextColor(TFT_YELLOW); tft.setTextSize(2);
	tft.println(1234.56);
	tft.setTextColor(TFT_RED);		tft.setTextSize(3);
	tft.println(0xDEADBEEF, HEX);
	tft.println();
	tft.setTextColor(TFT_GREEN);
	tft.setTextSize(5);
	tft.println(F("Groop"));
	tft.setTextSize(2);
	tft.println(F("I implore thee"));
  tft.setTextColor(TFT_GREEN);
	tft.setTextSize(1);
	tft.println(F("my foonting turlingdromes."));
	tft.println(F("And hooptiously drangle me"));
	tft.println(F("with crinkly bindlewurdles,"));
	tft.println(F("Or I will rend thee"));
	tft.println(F("in the gobberwarts"));
	tft.println(F("with my blurglecruncheon,"));
	tft.println(F("see if I don't!"));
	tft.println(F(""));
	tft.println(F(""));
	tft.setTextColor(TFT_MAGENTA);
	tft.setTextSize(5);
	tft.println(F("Woot!"));
	uint32_t t = micros() - start;
	delay(500);
	return t;
}

uint32_t testPixels()
{
	int32_t	w = tft.width();
	int32_t	h = tft.height();

	uint32_t start = micros_start();
	tft.startWrite();
	for (uint16_t y = 0; y < h; y++)
	{
		for (uint16_t x = 0; x < w; x++)
		{
			tft.drawPixel(x, y, tft.color565(x<<3, y<<3, x*y));
		}
	}
	tft.endWrite();
	return micros() - start;
}


uint32_t testLines(uint16_t color)
{
	uint32_t start, t;
	int32_t	x1, y1, x2, y2;
	int32_t	w = tft.width();
	int32_t	h = tft.height();

	tft.fillScreen(TFT_BLACK);

	x1 = y1 = 0;
	y2 = h - 1;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = w - 1;

	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t = micros() - start; // fillScreen doesn't count against timing

	tft.fillScreen(TFT_BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t += micros() - start;

	tft.fillScreen(TFT_BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}
	t += micros() - start;

	tft.fillScreen(TFT_BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;

	start = micros_start();

	for (x2 = 0; x2 < w; x2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
	{
		tft.drawLine(x1, y1, x2, y2, color);
	}

	t += micros() - start;

	return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
	uint32_t start;
	int32_t x, y;
	int32_t w = tft.width();
	int32_t h = tft.height();

	tft.fillScreen(TFT_BLACK);

	start = micros_start();

	for (y = 0; y < h; y += 5)
		tft.drawFastHLine(0, y, w, color1);
	for (x = 0; x < w; x += 5)
		tft.drawFastVLine(x, 0, h, color2);

	return micros() - start;
}

uint32_t testRects(uint16_t color)
{
	uint32_t start;
	int32_t n, i, i2;
	int32_t cx = tft.width() / 2;
	int32_t cy = tft.height() / 2;

	tft.fillScreen(TFT_BLACK);
	n = min(tft.width(), tft.height());
	start = micros_start();
	for (i = 2; i < n; i += 6)
	{
		i2 = i / 2;
		tft.drawRect(cx-i2, cy-i2, i, i, color);
	}

	return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
	uint32_t start, t = 0;
	int32_t n, i, i2;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(TFT_BLACK);
	n = min(tft.width(), tft.height());
	for (i = n; i > 0; i -= 6)
	{
		i2 = i / 2;

		start = micros_start();

		tft.fillRect(cx-i2, cy-i2, i, i, color1);

		t += micros() - start;

		// Outlines are not included in timing results
		tft.drawRect(cx-i2, cy-i2, i, i, color2);
	}

	return t;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
	uint32_t start;
	int32_t x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

	tft.fillScreen(TFT_BLACK);

	start = micros_start();

	for (x = radius; x < w; x += r2)
	{
		for (y = radius; y < h; y += r2)
		{
			tft.fillCircle(x, y, radius, color);
		}
	}

	return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
	uint32_t start;
	int32_t x, y, r2 = radius * 2;
	int32_t w = tft.width() + radius;
	int32_t h = tft.height() + radius;

	// Screen is not cleared for this one -- this is
	// intentional and does not affect the reported time.
	start = micros_start();

	for (x = 0; x < w; x += r2)
	{
		for (y = 0; y < h; y += r2)
		{
			tft.drawCircle(x, y, radius, color);
		}
	}

	return micros() - start;
}

uint32_t testTriangles()
{
	uint32_t start;
	int32_t n, i;
	int32_t cx = tft.width()/ 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(TFT_BLACK);
	n = min(cx, cy);

	start = micros_start();

	for (i = 0; i < n; i += 5)
	{
		tft.drawTriangle(
			cx		, cy - i, // peak
			cx - i, cy + i, // bottom left
			cx + i, cy + i, // bottom right
			tft.color565(0, 0, i));
	}

	return micros() - start;
}

uint32_t testFilledTriangles()
{
	uint32_t start, t = 0;
	int32_t i;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(TFT_BLACK);

	start = micros_start();

	for (i = min(cx,cy); i > 10; i -= 5) {
		start = micros_start();
		tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
			tft.color565(0, i, i));
		t += micros() - start;
		tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
			tft.color565(i, i, 0));
	}

	return t;
}

uint32_t testRoundRects()
 {
	uint32_t start;
	int32_t w, i, i2;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(TFT_BLACK);
	
	w = min(tft.width(), tft.height());
	
	start = micros_start();

	for (i = 0; i < w; i += 6)
	{
		i2 = i / 2;
		tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
	}

	return micros() - start;
}

uint32_t testFilledRoundRects()
{
	uint32_t start;
	int32_t i, i2;
	int32_t cx = tft.width() / 2 - 1;
	int32_t cy = tft.height() / 2 - 1;

	tft.fillScreen(TFT_BLACK);

	start = micros_start();

	for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
	{
		i2 = i / 2;
		tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
	}

	return micros() - start;
}
/*

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Solution for Tasmota Luatos Core ESP32 C3 
// Found :  https://github.com/espressif/arduino-esp32/issues/9618
// 
// The change done in IDF to #define REG_SPI_BASE(i) (((i)>=2) ? (DR_REG_SPI2_BASE + (i-2) * 0x1000) : (0)) // GPSPI2 and GPSPI3 is wrong.
// Correct is #define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (DR_REG_SPI0_BASE - ((i) * 0x1000))) // GPSPI2 and GPSPI3
// Changed this in our IDF fork for Tasmota, which is the base for compiling the Tasmota Arduino libs.
// Edit: The changes done for the other SOCs are imho wrong too. Only ESP32 looks correct.
//
// Edit C:\Users\<username>\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-bd2b9390ef\esp32c3\include\soc\esp32c3\include\soc\soc.h
//
// Replace "#define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (0))   // only one GPSPI"
// With    "#define REG_SPI_BASE(i) (((i)==2) ? (DR_REG_SPI2_BASE) : (DR_REG_SPI0_BASE - ((i) * 0x1000)))"
//
// Replace in C:\Users\<username>\Documents\Arduino\libraries\TFT_eSPI\Processors\TFT_eSPI_ESP32_C3.h the line
//   #define SPI_PORT SPI2_HOST
// with
// 
//   #if ESP_ARDUINO_VERSION_MAJOR < 3
//     #define SPI_PORT SPI2_HOST
//   #else
//     #define SPI_PORT 2
//   #endif
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Still necessary for ESP32 S3 
// FSPI / VSPI port (SPI2) used unless following defined. HSPI port is (SPI3) on S3.
//#define USE_HSPI_PORT                   // Necessary for ESP32 S3 

--------------------------------------------------------------------------------------------------------------------------
Benchmark                Time (microseconds)
HaD pushColor            80668
Screen fill              14497
Text                     15464
Pixels                   241325
Lines                    261431
Horiz/Vert Lines         7282
Rectangles (outline)     5363
Rectangles (filled)      172212
Circles (filled)         33622
Circles (outline)        24798
Triangles (outline)      17867
Triangles (filled)       69588
Rounded rects (outline)  14599
Rounded rects (filled)   177844
Done!
--------------------------------------------------------------------------------------------------------------------------
ESP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5820,len:0x3ac
load:0x403cc710,len:0x6c4
load:0x403ce710,len:0x2788
entry 0x403cc710
[   101][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x4200c676
[   113][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x4200c63a
[   124][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x4200c5fe
[   135][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x4200c5c2
[   147][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x4200c676
[   158][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x4200c63a
[   169][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x4200c5fe
[   181][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x4200c5c2
[   198][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 20 successfully set to type UART_RX (2) with bus 0x3fc8ca90
[   209][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 21 successfully set to type UART_TX (3) with bus 0x3fc8ca90
=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32-C3
  Package           : 0
  Revision          : 3
  Cores             : 1
  CPU Frequency     : 160 MHz
  XTAL Frequency    : 40 MHz
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : No
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   333064 B ( 325.3 KB)
  Free Bytes        :   301596 B ( 294.5 KB)
  Allocated Bytes   :    27692 B (  27.0 KB)
  Minimum Free Bytes:   301456 B ( 294.4 KB)
  Largest Free Block:   163828 B ( 160.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304 B (4 MB)
  Block Size        :    65536 B (  64.0 KB)
  Sector Size       :     4096 B (   4.0 KB)
  Page Size         :      256 B (   0.2 KB)
  Bus Speed         : 80 MHz
  Bus Mode          : DIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Jul  8 2024 19:42:29
  Compile Host OS   : windows
  ESP-IDF Version   : v5.1.4-358-gbd2b9390ef-dirty
  Arduino Version   : 3.0.2
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : ESP32C3_DEV
  Arduino Variant   : esp32c3
  Arduino FQBN      : esp32:esp32:esp32c3:UploadSpeed=921600,CDCOnBoot=default,CPUFreq=160,FlashFreq=80,FlashMode=dio,FlashSize=4M,PartitionScheme=default,DebugLevel=verbose,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default
============ Before Setup End ============
[   533][V][esp32-hal-uart.c:408] uartBegin(): UART0 baud(115200) Mode(800001c) rxPin(20) txPin(21)
[   542][V][esp32-hal-uart.c:497] uartBegin(): UART0 not installed. Starting installation
[   550][V][esp32-hal-uart.c:544] uartBegin(): UART0 initialization done.

Bodmer's TFT_eSPI library Test!
no #define USE_xSPI_PORT
FSPI : 0
HSPI : 1
VSPI : 0
SPI_PORT : 2
SPI2_HOST : 1
DR_REG_SPI1_BASE : 0x60002000
DR_REG_SPI2_BASE : 0x60024000
REG_SPI_BASE(2) : 0x60024000
REG_SPI_BASE(1) : 0x60002000
start tft.init() 
[   578][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008cae
[   591][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 7 successfully set to type GPIO (1) with bus 0x8
[   601][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008cae
[   612][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 19 successfully set to type GPIO (1) with bus 0x14
[   622][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_SCK (34) successfully set to 0x4200b5bc
[   634][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_MISO (35) successfully set to 0x4200b4b6
[   646][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_MOSI (36) successfully set to 0x4200b3ae
[   658][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_SS (37) successfully set to 0x4200b376
[   670][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008cae
[   681][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 4 successfully set to type GPIO (1) with bus 0x5
[   691][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 4 successfully set to type SPI_MASTER_SCK (34) with bus 0x1
[   701][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008cae
[   712][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 5 successfully set to type GPIO (1) with bus 0x6
[   722][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 5 successfully set to type SPI_MASTER_MISO (35) with bus 0x1
[   733][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008cae
   744][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type GPIO (1) with bus 0x7
[   754][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type SPI_MASTER_MOSI (36) with bus 0x1
done tft.init() 
[  1299][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008cae
[  1311][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 18 successfully set to type GPIO (1) with bus 0x13
[  1837][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 18 successfully set to type INIT (0) with bus 0x0
[  1847][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type LEDC (9) successfully set to 0x42008f08
[  1858][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 18 successfully set to type LEDC (9) with bus 0x3fc92a38
[  1868][I][esp32-hal-ledc.c:117] ledcAttachChannel(): LEDC attached to pin 18 (channel 0, resolution 8)
=========== After Setup Start ============
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   333064 B ( 325.3 KB)
  Free Bytes        :   304232 B ( 297.1 KB)
  Allocated Bytes   :    24704 B (  24.1 KB)
  Minimum Free Bytes:   300996 B ( 293.9 KB)
  Largest Free Block:   163828 B ( 160.0 KB)
------------------------------------------
GPIO Info:
------------------------------------------
  GPIO : BUS_TYPE[bus/unit][chan]
  --------------------------------------  
     4 : SPI_MASTER_SCK[0]
     5 : SPI_MASTER_MISO[0]
     6 : SPI_MASTER_MOSI[0]
     7 : GPIO
    18 : LEDC[0][0]
    19 : GPIO
    20 : UART_RX[0]
    21 : UART_TX[0]
============ After Setup End =============
Processor : ESP32-C3
ESP32     : 3.0.2
TFT_eSPI  : 2.5.43
SPI Write : 80 MHz
no #define USE_xSPI_PORT
MOSI 6 - MISO 5 - SCLK 4 - CS  7 - DC  19 - RST -1
Benchmark                Time (microseconds)
HaD pushColor            81182
Screen fill              14497
Text                     15424
Pixels                   241329
Lines                    261918
Horiz/Vert Lines         7317
Rectangles (outline)     5380
Rectangles (filled)      172310
Circles (filled)         33952
Circles (outline)        24865
Triangles (outline)      17986
Triangles (filled)       69931
Rounded rects (outline)  14623
Rounded rects (filled)   177987
Done!

Processor : ESP32-C3
ESP32     : 3.0.2
TFT_eSPI  : 2.5.43
SPI Write : 80 MHz
no #define USE_xSPI_PORT
MOSI 6 - MISO 5 - SCLK 4 - CS  7 - DC  19 - RST -1
Benchmark                Time (microseconds)
HaD pushColor            81159
Screen fill              14498
Text                     15418
Pixels                   241329
Lines                    261915
Horiz/Vert Lines         7318
Rectangles (outline)     5380
Rectangles (filled)      172303
Circles (filled)         33952
Circles (outline)        24866
Triangles (outline)      17983
Triangles (filled)       69931
Rounded rects (outline)  14623
Rounded rects (filled)   177983
Done!


Alt :
=========================================================================================================================

ESP32 2.0.14 DIO ! :

Funktioniert auch mit modifizierter TFT_eSPI_ESP32_C3.h :
  ESP32 2.0.14 + TFT_eSPI 2.5.43, DIO, !!! nur ohne "#define USE_HSPI_PORT" !!!
  ESP32 2.0.14 + TFT_eSPI 2.5.43, DIO, mit #define USE_FSPI_PORT oder #define USE_VSPI_PORT
Geht nicht :  
  mit "#define USE_HSPI_PORT" Display schwarz, Serial Monitor Benchmark :  Werte zu klein, SDA und SCK beide konstant bei 3.3V


ESP32 2.0.17 DIO !, mit modifizierter TFT_eSPI_ESP32_C3.h :

Kernel panic 
  ESP32 2.0.17 + TFT_eSPI 2.5.43, DIO, ohne "#define USE_xSPI_PORT"
  ESP32 2.0.17 + TFT_eSPI 2.5.43, DIO, "#define USE_HSPI_PORT"
  ESP32 2.0.17 + TFT_eSPI 2.5.43, DIO, "#define USE_FSPI_PORT"
  ESP32 2.0.17 + TFT_eSPI 2.5.43, DIO, "#define USE_VSPI_PORT"


ESP32 3.0.2 DIO !, mit modifizierter TFT_eSPI_ESP32_C3.h :

Kernel panic 
  ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, "#define USE_VSPI_PORT"

crasht in tft.init();
  ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, ohne "#define USE_xSPI_PORT"
  ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, "#define USE_FSPI_PORT"


  ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, "#define USE_HSPI_PORT" mit(ohne) modifizierter TFT_eSPI_ESP32_C3.h
Funktioniert etwas, crasht nicht in tft_init, aber Display bleibt schwarz und
Serial Monitor Benchmark-Werte zu klein, 
SCK konstant bei 0V und SDA konstant bei 3.3V = Pins werden nicht angesteuert
  ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, "#define USE_HSPI_PORT"

-------------------------------------------------------------------------------------------------------------------------
TFT_BL -1 verbose  ( ->   //#define TFT_BL ...

Guru Meditation Error: Core  0 panic'ed (Store access fault). Exception was unhandled.
-------------------------------------------------------------------------------------------------------------------------
Debug Verbose : ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, "#define USE_HSPI_PORT" mit(ohne) modifizierter TFT_eSPI_ESP32_C3.h

ESP-ROM:esp32c3-api1-20210207 ESP32 3.0.2 + TFT_eSPI 2.5.43, DIO, "#define USE_HSPI_PORT" 
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5820,len:0x3ac
load:0x403cc710,len:0x6c4
load:0x403ce710,len:0x2788
entry 0x403cc710
[   101][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x4200c0fe
[   112][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x4200c0c2
[   124][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x4200c086
[   135][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x4200c04a
[   147][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x4200c0fe
[   158][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x4200c0c2
[   169][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x4200c086
[   181][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x4200c04a
E (195) esp_core_dump_flash: Core dump data check failed:                                         jetzt nicht mehr ? BL -1            ???? 
Calculated checksum='08099a69'                                                                                                        ????
Image checksum='ffffffff'                                                                                                             ????
[   200][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 20 successfully set to type UART_RX (2) with bus 0x3fc8ca90
[   216][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 21 successfully set to type UART_TX (3) with bus 0x3fc8ca90
=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32-C3
  Package           : 0
  Revision          : 3
  Cores             : 1
  CPU Frequency     : 160 MHz
  XTAL Frequency    : 40 MHz
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : No
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   333032 B ( 325.2 KB)
  Free Bytes        :   301464 B ( 294.4 KB)
  Allocated Bytes   :    27776 B (  27.1 KB)
  Minimum Free Bytes:   301324 B ( 294.3 KB)
  Largest Free Block:   163828 B ( 160.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304 B (4 MB)
  Block Size        :    65536 B (  64.0 KB)
  Sector Size       :     4096 B (   4.0 KB)
  Page Size         :      256 B (   0.2 KB)
  Bus Speed         : 80 MHz
  Bus Mode          : DIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Jul  6 2024 21:09:42
  Compile Host OS   : windows
  ESP-IDF Version   : v5.1.4-358-gbd2b9390ef-dirty
  Arduino Version   : 3.0.2
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : ESP32C3_DEV
  Arduino Variant   : esp32c3
  Arduino FQBN      : esp32:esp32:esp32c3:UploadSpeed=921600,CDCOnBoot=default,CPUFreq=160,FlashFreq=80,FlashMode=dio,FlashSize=4M,PartitionScheme=default,DebugLevel=verbose,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default
============ Before Setup End ============
[   541][V][esp32-hal-uart.c:408] uartBegin(): UART0 baud(115200) Mode(800001c) rxPin(20) txPin(21)
[   550][V][esp32-hal-uart.c:497] uartBegin(): UART0 not installed. Starting installation
[   559][V][esp32-hal-uart.c:544] uartBegin(): UART0 initialization done.

Bodmer's TFT_eSPI library Test!
1: start tft.init() 
[   566][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008736
[   581][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 7 successfully set to type GPIO (1) with bus 0x8
[   591][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008736
[   602][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 19 successfully set to type GPIO (1) with bus 0x14
????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
[  1140][E][esp32-hal-periman.c:180] perimanGetPinBus(): Invalid pin: 255
[  1147][E][esp32-hal-gpio.c:166] __digitalWrite(): IO 255 is not set as GPIO.
[  1154][E][esp32-hal-gpio.c:103] __pinMode(): Invalid IO 255 selected

oder :

  Guru Meditation Error: Core  0 panic'ed (Store access fault). Exception was unhandled.
????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????

9: done tft.init() 
[  1160][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008736
[  1173][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 18 successfully set to type GPIO (1) with bus 0x13
[  1686][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 18 successfully set to type INIT (0) with bus 0x0
[  1696][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type LEDC (9) successfully set to 0x42008990
[  1707][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 18 successfully set to type LEDC (9) with bus 0x3fc929d4
[  1718][I][esp32-hal-ledc.c:117] ledcAttachChannel(): LEDC attached to pin 18 (channel 0, resolution 8)
=========== After Setup Start ============
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   333032 B ( 325.2 KB)
  Free Bytes        :   304332 B ( 297.2 KB)
  Allocated Bytes   :    24604 B (  24.0 KB)
  Minimum Free Bytes:   300864 B ( 293.8 KB)
  Largest Free Block:   163828 B ( 160.0 KB)
------------------------------------------
GPIO Info:
------------------------------------------
  GPIO : BUS_TYPE[bus/unit][chan]
  --------------------------------------  
     7 : GPIO
    18 : LEDC[0][0]
    19 : GPIO
    20 : UART_RX[0]
    21 : UART_TX[0]
============ After Setup End =============

Benchmark                Time (microseconds)
HaD pushColor            20773
Screen fill              671
Text                     31014
Pixels                   160081
Lines                    199347
Horiz/Vert Lines         12398
Rectangles (outline)     5439
Rectangles (filled)      11251
Circles (filled)         34027
Circles (outline)        36272
Triangles (outline)      18906
Triangles (filled)       20260
Rounded rects (outline)  23623
Rounded rects (filled)   17236
Done!

Benchmark                Time (microseconds)
HaD pushColor            20775
...
-------------------------------------------------------------------------------------------------------------------------
Debug Verbose : ESP32 2.0.14 + TFT_eSPI 2.5.43, DIO, !! ohne !! "#define USE_HSPI_PORT" ohne modifizierter TFT_eSPI_ESP32_C3.h

ESP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5810,len:0x38c
load:0x403cc710,len:0x6a0
load:0x403ce710,len:0x25f4
entry 0x403cc710
E (100) esp_core_dump_flash: Core dump data check failed��                                   ??????
Bodmer's TFT_eSPI library Test!
1: start tft.init() 
E (718) gpio: gpio_set_level(227): GPIO output gpio_num error                                  ????  TFT_BL -1  mag er nicht !!
[   667][E][esp32-hal-gpio.c:102] __pinMode(): Invalid pin selected                            ????  dito
9: done tft.init() 
[  1222][V][esp32-hal-ledc.c:231] analogWrite(): GPIO 18 - Using Channel 5, Value = 255
[  1232][V][esp32-hal-ledc.c:231] analogWrite(): GPIO 18 - Using Channel 5, Value = 254
...
[  5032][V][esp32-hal-ledc.c:231] analogWrite(): GPIO 18 - Using Channel 5, Value = 126
[  5042][V][esp32-hal-ledc.c:231] analogWrite(): GPIO 18 - Using Channel 5, Value = 127

Benchmark                Time (microseconds)
HaD pushColor            212027
Screen fill              41385
Text                     23026
Pixels                   347133
Lines                    395849
Horiz/Vert Lines         18464
Rectangles (outline)     13319
Rectangles (filled)      491309
Circles (filled)         64166
Circles (outline)        37315
Triangles (outline)      27540
Triangles (filled)       172906
Rounded rects (outline)  25548
Rounded rects (filled)   496731
Done!

----------------------------------------------------------------------------------------------------------

ESP32 3.0.2 + TFT_eSPI 2.5.43 DIO Verbose

SP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5820,len:0x3ac
load:0x403cc710,len:0x6c4
load:0x403ce710,len:0x2788
entry 0x403cc710
[   101][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x4200c10e
[   112][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x4200c0d2
[   124][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x4200c096
[   135][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x4200c05a
[   147][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RX (2) successfully set to 0x4200c10e
[   158][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_TX (3) successfully set to 0x4200c0d2
[   169][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_CTS (4) successfully set to 0x4200c096
[   181][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type UART_RTS (5) successfully set to 0x4200c05a
[   201][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 20 successfully set to type UART_RX (2) with bus 0x3fc8ca90
[   212][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 21 successfully set to type UART_TX (3) with bus 0x3fc8ca90
=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32-C3
  Package           : 0
  Revision          : 3
  Cores             : 1
  CPU Frequency     : 160 MHz
  XTAL Frequency    : 40 MHz
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : No
  BT Low Energy     : Yes
  IEEE 802.15.4     : No
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   333064 B ( 325.3 KB)
  Free Bytes        :   301596 B ( 294.5 KB)
  Allocated Bytes   :    27692 B (  27.0 KB)
  Minimum Free Bytes:   301456 B ( 294.4 KB)
  Largest Free Block:   163828 B ( 160.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304 B (4 MB)
  Block Size        :    65536 B (  64.0 KB)
  Sector Size       :     4096 B (   4.0 KB)
  Page Size         :      256 B (   0.2 KB)
  Bus Speed         : 80 MHz
  Bus Mode          : DIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1408.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Jul  6 2024 16:16:26
  Compile Host OS   : windows
  ESP-IDF Version   : v5.1.4-358-gbd2b9390ef-dirty
  Arduino Version   : 3.0.2
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : ESP32C3_DEV
  Arduino Variant   : esp32c3
  Arduino FQBN      : esp32:esp32:esp32c3:UploadSpeed=921600,CDCOnBoot=default,CPUFreq=160,FlashFreq=80,FlashMode=dio,FlashSize=4M,PartitionScheme=default,DebugLevel=verbose,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default
============ Before Setup End ============
[   536][V][esp32-hal-uart.c:408] uartBegin(): UART0 baud(115200) Mode(800001c) rxPin(20) txPin(21)
[   545][V][esp32-hal-uart.c:497] uartBegin(): UART0 not installed. Starting installation
[   553][V][esp32-hal-uart.c:544] uartBegin(): UART0 initialization done.

Bodmer's TFT_eSPI library Test!
before tft.init() 
[   560][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008746
[   576][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 7 successfully set to type GPIO (1) with bus 0x8
[   586][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008746
[   597][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 19 successfully set to type GPIO (1) with bus 0x14
[   607][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_SCK (34) successfully set to 0x4200b054
[   619][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_MISO (35) successfully set to 0x4200af4e
[   631][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_MOSI (36) successfully set to 0x4200ae46
[   643][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type SPI_MASTER_SS (37) successfully set to 0x4200ae0e
[   655][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008746
[   666][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 4 successfully set to type GPIO (1) with bus 0x5
[   676][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 4 successfully set to type SPI_MASTER_SCK (34) with bus 0x1
[   686][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008746
[   697][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type GPIO (1) with bus 0x7
[   707][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type SPI_MASTER_MISO (35) with bus 0x1
[   718][V][esp32-hal-periman.c:235] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x42008746
[   729][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type INIT (0) with bus 0x0
[   739][D][esp32-hal-spi.c:185] spiDetachBus(): Stopping SPI bus
[   745][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 4 successfully set to type INIT (0) with bus 0x0
[   754][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type INIT (0) with bus 0x0
[   764][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type GPIO (1) with bus 0x7
[   774][V][esp32-hal-periman.c:160] perimanSetPinBus(): Pin 6 successfully set to type SPI_MASTER_MOSI (36) with bus 0x1


ESP32 3.0.2 + TFT_eSPI 2.5.43 DIO

ELF file SHA256: de3bd0b868f3319b

Rebooting...
ESP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x3 (RTC_SW_SYS_RST),boot:0xc (SPI_FAST_FLASH_BOOT)
Saved PC:0x40048b82
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5820,len:0x3ac
load:0x403cc710,len:0x6c4
load:0x403ce710,len:0x2788
entry 0x403cc710

Bodmer's TFT_eSPI library Test!
1
Guru Meditation Error: Core  0 panic'ed (Store access fault). Exception was unhandled.




ESP32 3.0.2 + TFT_eSPI 2.5.43 ohne QIO  QIO  QIO  QIO  QIO  QIO  QIO  QIO  QIO  

ELF file SHA256: a8b1736d8d2a86df

E (502) esp_core_dump_flash: Core dump flash config is corrupted! CRC=0x7bd5c66f instead of 0x0
E (510) esp_core_dump_elf: Elf write init failed!
E (514) esp_core_dump_common: Core dump write failed with error=-1
Rebooting...
ESP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x3 (RTC_SW_SYS_RST),boot:0xc (SPI_FAST_FLASH_BOOT)
Saved PC:0x40048b82
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5820,len:0x458
load:0x403cc710,len:0x814
load:0x403ce710,len:0x2880
entry 0x403cc710

assert failed: do_core_init startup.c:350 (flash_ret == ESP_OK)
Core  0 register dump:



ESP32 2.0.14 + TFT_eSPI 2.5.43 QIO  QIO  QIO  QIO  QIO  QIO  QIO  QIO  QIO  

LF file SHA256: e7e915be239fd730

E (499) esp_core_dump_flash: Core dump flash config is corrupted! CRC=0x7bd5c66f instead of 0x0
Rebooting...
ESP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x3 (RTC_SW_SYS_RST),boot:0xc (SPI_FAST_FLASH_BOOT)
Saved PC:0x4038205c
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5810,len:0x438
load:0x403cc710,len:0x90c
load:0x403ce710,len:0x25f4
entry 0x403cc710

assert failed: do_core_init startup.c:328 (flash_ret == ESP_OK)
Core  0 register dump:
MEPC    : 0x40382480  RA      : 0x4038554e  SP      : 0x3fcde190  GP      : 0x3fc8c200  
TP      : 0x00000000  T0      : 0x37363534  T1      : 0x7271706f  T2      : 0x33323130  
S0/FP   : 0x3fcde2ff  S1      : 0x00000088  A0      : 0x3fcde1f4  A1      : 0x3fc8d095  
A2      : 0x00000001  A3      : 0x00000029  A4      : 0x00000001  A5      : 0x3fc8f000  
A6      : 0x7a797877  A7      : 0x76757473  S2      : 0x3fcde1e8  S3      : 0x00000001  
S4      : 0x3fcde1e8  S5      : 0x42015d0c  S6      : 0x42000020  S7      : 0x3c040020  
S8      : 0x00010020  S9      : 0x00000000  S10     : 0x00000000  S11     : 0x00000000  
T3      : 0x6e6d6c6b  T4      : 0x6a696867  T5      : 0x66656463  T6      : 0x62613938  
MSTATUS : 0x00001801  MTVEC   : 0x40380001  MCAUSE  : 0x00000007  MTVAL   : 0x00000000  
MHARTID : 0x00000000  

Stack memory:
3fcde190: 0x42000020 0x

*/


/***************************************************
  Original sketch text:

  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
