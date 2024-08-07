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

//#include <../Setup435_H2_ST7789_240x280.h>  // ESP32-H2, ST7789, 80MHz

//  Arduino IDE Board : ESP32H2 Dev Module
//  Flash Mode        : QIO                   ( default )
//  USB CDC On Boot   : Enabled               ( so serial monitor works )

#include "SPI.h"
#include "TFT_eSPI.h"

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

// Backlight Pin
const int pwmPin = 2;

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

  Serial.print("VSPI : ");  // 1 VSPI == FSPI // 0 bei 2.0.14
  Serial.println(VSPI); 

  Serial.print("SPI_PORT : ");   // must be 2 ! // 1 bei 2.0.14
  Serial.println(SPI_PORT); 

  Serial.print("SPI2_HOST : ");  // 1
  Serial.println(SPI2_HOST); 

  Serial.print("DR_REG_SPI1_BASE : 0x");  //0x60002000
  Serial.println(DR_REG_SPI1_BASE,HEX); 

  Serial.print("DR_REG_SPI2_BASE : 0x");  //0x60024000
  Serial.println(DR_REG_SPI2_BASE,HEX); 

  Serial.print("REG_SPI_BASE(1) : 0x");   //0x60002000
  Serial.println(REG_SPI_BASE(1),HEX); 
  
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
  #elif defined(CONFIG_IDF_TARGET_ESP32C6)
    Serial.println("ESP32-C6");  
  #elif defined(CONFIG_IDF_TARGET_ESP32H2)
    Serial.println("ESP32-H2");  
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
  #elif defined(CONFIG_IDF_TARGET_ESP32C6)
    tft.println("ESP32-C6");  
  #elif defined(CONFIG_IDF_TARGET_ESP32H2)
    tft.println("ESP32-H2");  
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
  
  tft.print(F("SPI Speed : ")); 
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

Processor : ESP32-H2
ESP32     : 3.0.4
TFT_eSPI  : 2.5.43
SPI Write : 80 MHz
no #define USE_xSPI_PORT
MOSI 25 - MISO 11 - SCLK 10 - CS  0 - DC  1 - RST 3
Benchmark                Time (microseconds)
HaD pushColor            204321
Screen fill              36495
Text                     41802
Pixels                   715715
Lines                    769155
Horiz/Vert Lines         18159
Rectangles (outline)     13666
Rectangles (filled)      433670
Circles (filled)         93062
Circles (outline)        71658
Triangles (outline)      51181
Triangles (filled)       183343
Rounded rects (outline)  40344
Rounded rects (filled)   451289
Done!


about 1000 error lin

30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:302:5: note: in expansion of macro 'WRITE_PERI_REG'
  302 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.h:61:34: note: in expansion of macro 'SPI_MS_DLEN_REG'
   61 |     #define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)
      |                                  ^~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:302:20: note: in expansion of macro 'SPI_MOSI_DLEN_REG'
  302 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |                    ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:311:14: note: in expansion of macro 'READ_PERI_REG'
  311 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |              ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:311:28: note: in expansion of macro 'SPI_CMD_REG'
  311 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                            ^~~~~~~~~~~
In file included from c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\TFT_eSPI.cpp:22:
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:311:50: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  311 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                  ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1757:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1757 | #define SPI_W0_REG (DR_REG_SPI_BASE + 0x98)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:312:7: note: in expansion of macro 'WRITE_PERI_REG'
  312 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  color[0]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:312:22: note: in expansion of macro 'SPI_W0_REG'
  312 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  color[0]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1769:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1769 | #define SPI_W1_REG (DR_REG_SPI_BASE + 0x9c)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:313:7: note: in expansion of macro 'WRITE_PERI_REG'
  313 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  color[1]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:313:22: note: in expansion of macro 'SPI_W1_REG'
  313 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  color[1]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1781:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1781 | #define SPI_W2_REG (DR_REG_SPI_BASE + 0xa0)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:314:7: note: in expansion of macro 'WRITE_PERI_REG'
  314 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  color[2]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:314:22: note: in expansion of macro 'SPI_W2_REG'
  314 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  color[2]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1793:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1793 | #define SPI_W3_REG (DR_REG_SPI_BASE + 0xa4)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:315:7: note: in expansion of macro 'WRITE_PERI_REG'
  315 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  color[3]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:315:22: note: in expansion of macro 'SPI_W3_REG'
  315 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  color[3]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1805:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1805 | #define SPI_W4_REG (DR_REG_SPI_BASE + 0xa8)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:316:7: note: in expansion of macro 'WRITE_PERI_REG'
  316 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  color[4]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:316:22: note: in expansion of macro 'SPI_W4_REG'
  316 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  color[4]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1817:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1817 | #define SPI_W5_REG (DR_REG_SPI_BASE + 0xac)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:317:7: note: in expansion of macro 'WRITE_PERI_REG'
  317 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  color[5]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:317:22: note: in expansion of macro 'SPI_W5_REG'
  317 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  color[5]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1829:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1829 | #define SPI_W6_REG (DR_REG_SPI_BASE + 0xb0)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:318:7: note: in expansion of macro 'WRITE_PERI_REG'
  318 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  color[6]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:318:22: note: in expansion of macro 'SPI_W6_REG'
  318 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  color[6]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1841:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1841 | #define SPI_W7_REG (DR_REG_SPI_BASE + 0xb4)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:319:7: note: in expansion of macro 'WRITE_PERI_REG'
  319 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  color[7]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:319:22: note: in expansion of macro 'SPI_W7_REG'
  319 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  color[7]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1853:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1853 | #define SPI_W8_REG (DR_REG_SPI_BASE + 0xb8)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:320:7: note: in expansion of macro 'WRITE_PERI_REG'
  320 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  color[8]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:320:22: note: in expansion of macro 'SPI_W8_REG'
  320 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  color[8]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1865:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1865 | #define SPI_W9_REG (DR_REG_SPI_BASE + 0xbc)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:321:7: note: in expansion of macro 'WRITE_PERI_REG'
  321 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  color[9]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:321:22: note: in expansion of macro 'SPI_W9_REG'
  321 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  color[9]);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1877:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1877 | #define SPI_W10_REG (DR_REG_SPI_BASE + 0xc0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:322:7: note: in expansion of macro 'WRITE_PERI_REG'
  322 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), color[10]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:322:22: note: in expansion of macro 'SPI_W10_REG'
  322 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), color[10]);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1889:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1889 | #define SPI_W11_REG (DR_REG_SPI_BASE + 0xc4)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:323:7: note: in expansion of macro 'WRITE_PERI_REG'
  323 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), color[11]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:323:22: note: in expansion of macro 'SPI_W11_REG'
  323 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), color[11]);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1901:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1901 | #define SPI_W12_REG (DR_REG_SPI_BASE + 0xc8)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:324:7: note: in expansion of macro 'WRITE_PERI_REG'
  324 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), color[12]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:324:22: note: in expansion of macro 'SPI_W12_REG'
  324 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), color[12]);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1913:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1913 | #define SPI_W13_REG (DR_REG_SPI_BASE + 0xcc)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:325:7: note: in expansion of macro 'WRITE_PERI_REG'
  325 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), color[13]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:325:22: note: in expansion of macro 'SPI_W13_REG'
  325 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), color[13]);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1925:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1925 | #define SPI_W14_REG (DR_REG_SPI_BASE + 0xd0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:326:7: note: in expansion of macro 'WRITE_PERI_REG'
  326 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), color[14]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:326:22: note: in expansion of macro 'SPI_W14_REG'
  326 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), color[14]);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1937:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1937 | #define SPI_W15_REG (DR_REG_SPI_BASE + 0xd4)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:327:7: note: in expansion of macro 'WRITE_PERI_REG'
  327 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), color[15]);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:327:22: note: in expansion of macro 'SPI_W15_REG'
  327 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), color[15]);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:329:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  329 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |       ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:329:25: note: in expansion of macro 'SPI_CMD_REG'
  329 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |                         ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:329:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  329 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |       ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:330:14: note: in expansion of macro 'READ_PERI_REG'
  330 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |              ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:330:28: note: in expansion of macro 'SPI_CMD_REG'
  330 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                            ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:330:50: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  330 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                                                  ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:332:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  332 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |       ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:332:25: note: in expansion of macro 'SPI_CMD_REG'
  332 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                         ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:332:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  332 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |       ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:345:12: note: in expansion of macro 'READ_PERI_REG'
  345 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |            ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:345:26: note: in expansion of macro 'SPI_CMD_REG'
  345 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                          ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:345:48: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  345 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:481:26: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
  481 | #define SPI_MS_DLEN_REG (DR_REG_SPI_BASE + 0x1c)
      |                          ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:346:5: note: in expansion of macro 'WRITE_PERI_REG'
  346 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 255);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.h:61:34: note: in expansion of macro 'SPI_MS_DLEN_REG'
   61 |     #define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)
      |                                  ^~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:346:20: note: in expansion of macro 'SPI_MOSI_DLEN_REG'
  346 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 255);
      |                    ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1757:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1757 | #define SPI_W0_REG (DR_REG_SPI_BASE + 0x98)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:347:5: note: in expansion of macro 'WRITE_PERI_REG'
  347 |     WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  color[0]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:347:20: note: in expansion of macro 'SPI_W0_REG'
  347 |     WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  color[0]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1769:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1769 | #define SPI_W1_REG (DR_REG_SPI_BASE + 0x9c)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:348:5: note: in expansion of macro 'WRITE_PERI_REG'
  348 |     WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  color[1]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:348:20: note: in expansion of macro 'SPI_W1_REG'
  348 |     WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  color[1]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1781:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1781 | #define SPI_W2_REG (DR_REG_SPI_BASE + 0xa0)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:349:5: note: in expansion of macro 'WRITE_PERI_REG'
  349 |     WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  color[2]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:349:20: note: in expansion of macro 'SPI_W2_REG'
  349 |     WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  color[2]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1793:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1793 | #define SPI_W3_REG (DR_REG_SPI_BASE + 0xa4)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:350:5: note: in expansion of macro 'WRITE_PERI_REG'
  350 |     WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  color[3]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:350:20: note: in expansion of macro 'SPI_W3_REG'
  350 |     WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  color[3]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1805:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1805 | #define SPI_W4_REG (DR_REG_SPI_BASE + 0xa8)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:351:5: note: in expansion of macro 'WRITE_PERI_REG'
  351 |     WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  color[4]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:351:20: note: in expansion of macro 'SPI_W4_REG'
  351 |     WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  color[4]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1817:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1817 | #define SPI_W5_REG (DR_REG_SPI_BASE + 0xac)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:352:5: note: in expansion of macro 'WRITE_PERI_REG'
  352 |     WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  color[5]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:352:20: note: in expansion of macro 'SPI_W5_REG'
  352 |     WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  color[5]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1829:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1829 | #define SPI_W6_REG (DR_REG_SPI_BASE + 0xb0)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:353:5: note: in expansion of macro 'WRITE_PERI_REG'
  353 |     WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  color[6]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:353:20: note: in expansion of macro 'SPI_W6_REG'
  353 |     WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  color[6]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1841:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1841 | #define SPI_W7_REG (DR_REG_SPI_BASE + 0xb4)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:354:5: note: in expansion of macro 'WRITE_PERI_REG'
  354 |     WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  color[7]);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:354:20: note: in expansion of macro 'SPI_W7_REG'
  354 |     WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  color[7]);
      |                    ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:356:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  356 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |     ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:356:23: note: in expansion of macro 'SPI_CMD_REG'
  356 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |                       ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:356:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  356 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |     ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:357:12: note: in expansion of macro 'READ_PERI_REG'
  357 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |            ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:357:26: note: in expansion of macro 'SPI_CMD_REG'
  357 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                          ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:357:48: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  357 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                                                ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:359:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  359 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:359:23: note: in expansion of macro 'SPI_CMD_REG'
  359 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                       ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:359:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  359 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:365:12: note: in expansion of macro 'READ_PERI_REG'
  365 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |            ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:365:26: note: in expansion of macro 'SPI_CMD_REG'
  365 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                          ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:365:48: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  365 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:481:26: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
  481 | #define SPI_MS_DLEN_REG (DR_REG_SPI_BASE + 0x1c)
      |                          ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:366:5: note: in expansion of macro 'WRITE_PERI_REG'
  366 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.h:61:34: note: in expansion of macro 'SPI_MS_DLEN_REG'
   61 |     #define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)
      |                                  ^~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:366:20: note: in expansion of macro 'SPI_MOSI_DLEN_REG'
  366 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |                    ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1757:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1757 | #define SPI_W0_REG (DR_REG_SPI_BASE + 0x98)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:368:7: note: in expansion of macro 'WRITE_PERI_REG'
  368 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT)+i, DAT8TO32(data)); data+=4;
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:368:22: note: in expansion of macro 'SPI_W0_REG'
  368 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT)+i, DAT8TO32(data)); data+=4;
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:371:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  371 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |     ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:371:23: note: in expansion of macro 'SPI_CMD_REG'
  371 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |                       ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:371:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  371 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |     ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:372:12: note: in expansion of macro 'READ_PERI_REG'
  372 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |            ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:372:26: note: in expansion of macro 'SPI_CMD_REG'
  372 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                          ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:372:48: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  372 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                                                ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:374:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  374 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:374:23: note: in expansion of macro 'SPI_CMD_REG'
  374 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                       ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:374:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  374 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:376:10: note: in expansion of macro 'READ_PERI_REG'
  376 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |          ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:376:24: note: in expansion of macro 'SPI_CMD_REG'
  376 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                        ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:376:46: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  376 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                              ^
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c: In member function 'void TFT_eSPI::pushPixels(const void*, uint32_t)':
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:481:26: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
  481 | #define SPI_MS_DLEN_REG (DR_REG_SPI_BASE + 0x1c)
      |                          ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:395:5: note: in expansion of macro 'WRITE_PERI_REG'
  395 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.h:61:34: note: in expansion of macro 'SPI_MS_DLEN_REG'
   61 |     #define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)
      |                                  ^~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:395:20: note: in expansion of macro 'SPI_MOSI_DLEN_REG'
  395 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), 511);
      |                    ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:398:14: note: in expansion of macro 'READ_PERI_REG'
  398 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |              ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:398:28: note: in expansion of macro 'SPI_CMD_REG'
  398 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                            ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:398:50: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  398 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                  ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1757:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1757 | #define SPI_W0_REG (DR_REG_SPI_BASE + 0x98)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:399:7: note: in expansion of macro 'WRITE_PERI_REG'
  399 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:399:22: note: in expansion of macro 'SPI_W0_REG'
  399 |       WRITE_PERI_REG(SPI_W0_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1769:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1769 | #define SPI_W1_REG (DR_REG_SPI_BASE + 0x9c)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:400:7: note: in expansion of macro 'WRITE_PERI_REG'
  400 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:400:22: note: in expansion of macro 'SPI_W1_REG'
  400 |       WRITE_PERI_REG(SPI_W1_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1781:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1781 | #define SPI_W2_REG (DR_REG_SPI_BASE + 0xa0)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:401:7: note: in expansion of macro 'WRITE_PERI_REG'
  401 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:401:22: note: in expansion of macro 'SPI_W2_REG'
  401 |       WRITE_PERI_REG(SPI_W2_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1793:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1793 | #define SPI_W3_REG (DR_REG_SPI_BASE + 0xa4)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:402:7: note: in expansion of macro 'WRITE_PERI_REG'
  402 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:402:22: note: in expansion of macro 'SPI_W3_REG'
  402 |       WRITE_PERI_REG(SPI_W3_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1805:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1805 | #define SPI_W4_REG (DR_REG_SPI_BASE + 0xa8)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:403:7: note: in expansion of macro 'WRITE_PERI_REG'
  403 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:403:22: note: in expansion of macro 'SPI_W4_REG'
  403 |       WRITE_PERI_REG(SPI_W4_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1817:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1817 | #define SPI_W5_REG (DR_REG_SPI_BASE + 0xac)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:404:7: note: in expansion of macro 'WRITE_PERI_REG'
  404 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:404:22: note: in expansion of macro 'SPI_W5_REG'
  404 |       WRITE_PERI_REG(SPI_W5_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1829:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1829 | #define SPI_W6_REG (DR_REG_SPI_BASE + 0xb0)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:405:7: note: in expansion of macro 'WRITE_PERI_REG'
  405 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:405:22: note: in expansion of macro 'SPI_W6_REG'
  405 |       WRITE_PERI_REG(SPI_W6_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1841:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1841 | #define SPI_W7_REG (DR_REG_SPI_BASE + 0xb4)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:406:7: note: in expansion of macro 'WRITE_PERI_REG'
  406 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:406:22: note: in expansion of macro 'SPI_W7_REG'
  406 |       WRITE_PERI_REG(SPI_W7_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1853:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1853 | #define SPI_W8_REG (DR_REG_SPI_BASE + 0xb8)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:407:7: note: in expansion of macro 'WRITE_PERI_REG'
  407 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:407:22: note: in expansion of macro 'SPI_W8_REG'
  407 |       WRITE_PERI_REG(SPI_W8_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1865:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1865 | #define SPI_W9_REG (DR_REG_SPI_BASE + 0xbc)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:408:7: note: in expansion of macro 'WRITE_PERI_REG'
  408 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:408:22: note: in expansion of macro 'SPI_W9_REG'
  408 |       WRITE_PERI_REG(SPI_W9_REG(SPI_PORT),  *data++);
      |                      ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1877:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1877 | #define SPI_W10_REG (DR_REG_SPI_BASE + 0xc0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:409:7: note: in expansion of macro 'WRITE_PERI_REG'
  409 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:409:22: note: in expansion of macro 'SPI_W10_REG'
  409 |       WRITE_PERI_REG(SPI_W10_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1889:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1889 | #define SPI_W11_REG (DR_REG_SPI_BASE + 0xc4)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:410:7: note: in expansion of macro 'WRITE_PERI_REG'
  410 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:410:22: note: in expansion of macro 'SPI_W11_REG'
  410 |       WRITE_PERI_REG(SPI_W11_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1901:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1901 | #define SPI_W12_REG (DR_REG_SPI_BASE + 0xc8)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:411:7: note: in expansion of macro 'WRITE_PERI_REG'
  411 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:411:22: note: in expansion of macro 'SPI_W12_REG'
  411 |       WRITE_PERI_REG(SPI_W12_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1913:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1913 | #define SPI_W13_REG (DR_REG_SPI_BASE + 0xcc)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:412:7: note: in expansion of macro 'WRITE_PERI_REG'
  412 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:412:22: note: in expansion of macro 'SPI_W13_REG'
  412 |       WRITE_PERI_REG(SPI_W13_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1925:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1925 | #define SPI_W14_REG (DR_REG_SPI_BASE + 0xd0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:413:7: note: in expansion of macro 'WRITE_PERI_REG'
  413 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:413:22: note: in expansion of macro 'SPI_W14_REG'
  413 |       WRITE_PERI_REG(SPI_W14_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1937:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1937 | #define SPI_W15_REG (DR_REG_SPI_BASE + 0xd4)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:414:7: note: in expansion of macro 'WRITE_PERI_REG'
  414 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), *data++);
      |       ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:414:22: note: in expansion of macro 'SPI_W15_REG'
  414 |       WRITE_PERI_REG(SPI_W15_REG(SPI_PORT), *data++);
      |                      ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:416:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  416 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |       ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:416:25: note: in expansion of macro 'SPI_CMD_REG'
  416 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |                         ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:416:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  416 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |       ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:417:14: note: in expansion of macro 'READ_PERI_REG'
  417 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |              ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:417:28: note: in expansion of macro 'SPI_CMD_REG'
  417 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                            ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:417:50: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  417 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                                                  ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:419:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  419 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |       ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:419:25: note: in expansion of macro 'SPI_CMD_REG'
  419 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                         ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:419:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  419 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |       ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:426:12: note: in expansion of macro 'READ_PERI_REG'
  426 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |            ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:426:26: note: in expansion of macro 'SPI_CMD_REG'
  426 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                          ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:426:48: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  426 |     while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                                ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:481:26: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
  481 | #define SPI_MS_DLEN_REG (DR_REG_SPI_BASE + 0x1c)
      |                          ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:427:5: note: in expansion of macro 'WRITE_PERI_REG'
  427 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |     ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.h:61:34: note: in expansion of macro 'SPI_MS_DLEN_REG'
   61 |     #define SPI_MOSI_DLEN_REG(x) SPI_MS_DLEN_REG(x)
      |                                  ^~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:427:20: note: in expansion of macro 'SPI_MOSI_DLEN_REG'
  427 |     WRITE_PERI_REG(SPI_MOSI_DLEN_REG(SPI_PORT), (len << 4) - 1);
      |                    ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:1757:21: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
 1757 | #define SPI_W0_REG (DR_REG_SPI_BASE + 0x98)
      |                     ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:428:45: note: in expansion of macro 'WRITE_PERI_REG'
  428 |     for (uint32_t i=0; i <= (len<<1); i+=4) WRITE_PERI_REG((SPI_W0_REG(SPI_PORT) + i), *data++);
      |                                             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:428:61: note: in expansion of macro 'SPI_W0_REG'
  428 |     for (uint32_t i=0; i <= (len<<1); i+=4) WRITE_PERI_REG((SPI_W0_REG(SPI_PORT) + i), *data++);
      |                                                             ^~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:430:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  430 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |       ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:430:25: note: in expansion of macro 'SPI_CMD_REG'
  430 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |                         ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:430:7: note: in expansion of macro 'SET_PERI_REG_MASK'
  430 |       SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_UPDATE);
      |       ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:431:14: note: in expansion of macro 'READ_PERI_REG'
  431 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |              ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:431:28: note: in expansion of macro 'SPI_CMD_REG'
  431 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                            ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:431:50: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  431 |       while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_UPDATE);
      |                                                  ^
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:13: note: in expansion of macro 'WRITE_PERI_REG'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |             ^~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:433:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  433 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:433:23: note: in expansion of macro 'SPI_CMD_REG'
  433 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |                       ^~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:110:54: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator|'
  110 |             WRITE_PERI_REG((reg), (READ_PERI_REG(reg)|(mask)));                                                        \
      |                                                      ^~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:100:76: note: in definition of macro 'WRITE_PERI_REG'
  100 |             (*((volatile uint32_t *)ETS_UNCACHED_ADDR(addr))) = (uint32_t)(val);                                       \
      |                                                                            ^~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:433:5: note: in expansion of macro 'SET_PERI_REG_MASK'
  433 |     SET_PERI_REG_MASK(SPI_CMD_REG(SPI_PORT), SPI_USR);
      |     ^~~~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/spi_reg.h:17:22: error: 'DR_REG_SPI_BASE' was not declared in this scope; did you mean 'DR_REG_SPI0_BASE'?
   17 | #define SPI_CMD_REG (DR_REG_SPI_BASE + 0x0)
      |                      ^~~~~~~~~~~~~~~
C:\Users\mb\AppData\Local\Arduino15\packages\esp32\tools\esp32-arduino-libs\idf-release_v5.1-dc859c1e67\esp32h2/include/soc/esp32h2/include/soc/soc.h:30:34: note: in definition of macro 'ETS_UNCACHED_ADDR'
   30 | #define ETS_UNCACHED_ADDR(addr) (addr)
      |                                  ^~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:435:10: note: in expansion of macro 'READ_PERI_REG'
  435 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |          ^~~~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:435:24: note: in expansion of macro 'SPI_CMD_REG'
  435 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                        ^~~~~~~~~~~
c:\Users\mb\Documents\Arduino\libraries\TFT_eSPI\Processors/TFT_eSPI_ESP32_C3.c:435:46: error: invalid operands of types 'void' and 'long unsigned int' to binary 'operator&'
  435 |   while (READ_PERI_REG(SPI_CMD_REG(SPI_PORT))&SPI_USR);
      |                                              ^

exit status 1

Compilation error: exit status 1
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
 
