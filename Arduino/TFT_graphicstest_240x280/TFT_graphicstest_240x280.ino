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

// Setup407_ST7789_320x170.h     // for Lolin32 ESP32  // 80MHz oder 40MHz
// Setup408_ST7789_320x170_C3.h  // for ESP32 C3 

// !!!!!!!  for ESP32C3 Dev Module und Flash Mode auf DIO stellen !!!!!!!!!

#include "SPI.h"
#include "TFT_eSPI.h"

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

unsigned long total = 0;
unsigned long tn = 0;
void setup() {
  Serial.begin(115200);
  //while (!Serial);
  Serial.println(""); 
  Serial.println("Bodmer's TFT_eSPI library Test!"); 
    /** Major version number (X.x.x) */
    // #define ESP_ARDUINO_VERSION_MAJOR 3  
  /** Minor version number (x.X.x) */
    // #define ESP_ARDUINO_VERSION_MINOR 0
  /** Patch version number (x.x.X) */
    // #define ESP_ARDUINO_VERSION_PATCH 2

	Serial.print(F("ESP32 Core ")); 
  Serial.print(ESP_ARDUINO_VERSION_MAJOR); 
  Serial.print("."); 
  Serial.print(ESP_ARDUINO_VERSION_MINOR); 
  Serial.print("."); 
  Serial.println(ESP_ARDUINO_VERSION_PATCH); 
 
  tft.init(); 
  
  //Serial.println("tft.init done"); 

}

void loop(void)
{
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

	tft.print(F("      "));
  tft.print(F("ESP32 Core ")); 
  tft.print(ESP_ARDUINO_VERSION_MAJOR); 
  tft.print("."); 
  tft.print(ESP_ARDUINO_VERSION_MINOR); 
  tft.print("."); 
  tft.print(ESP_ARDUINO_VERSION_PATCH); 

	delay(60 * 1000L);
/*
  tft.fillScreen(TFT_BLACK);
	for (int32_t i = 0; i < tft.height(); i++)
	{
		tft.drawFastHLine(0, i, tft.width(), c);
		c += d;
		if (c <= 4 || c >= 11)
			d = -d;
	}

  //tft.setAddrWindow(35, 0, 170, 280); // for Display 240x280

	tft.setCursor(0, 20);
	tft.setTextColor(TFT_MAGENTA);
	tft.setTextSize(2);

 	tft.println(F("TFT_eSPI test"));
	tft.println(F(""));
  
  tft.setTextSize(1);
  
  tft.setTextColor(TFT_WHITE);

  tft.print(F("ESP32 Core:")); 
  tft.print(ESP_ARDUINO_VERSION_MAJOR); 
  tft.print("."); 
  tft.print(ESP_ARDUINO_VERSION_MINOR); 
  tft.print("."); 
  tft.println(ESP_ARDUINO_VERSION_PATCH); 

  tft.print(F("Arduino IDE: ")); 
  tft.println(ARDUINO);

  tft.print(F("ESP IDF: ")); 
  tft.println(esp_get_idf_version());



	delay(60000);
*/  
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
  tft.print(F("       http://hackaday.io/"));
  tft.print(F("    Xark"));

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

Benchmark                Time (microseconds)
HaD pushColor             73329
Screen fill               13926
Text                      12235
Pixels                   163724
Lines                    176592
Horiz/Vert Lines           7282
Rectangles (outline)       5038
Rectangles (filled)      165600
Circles (filled)          26455
Circles (outline)         17936
Triangles (outline)       12764
Triangles (filled)        61332
Rounded rects (outline)   11764
Rounded rects (filled)   168593
Done!

170x320 :

Benchmark                Time (microseconds)

Speed                     80MHz    40MHz
HaD pushColor             73232   138856
Screen fill               11272    22093
Text                      11926    15251
Pixels                   132656   161594
Lines                    146144   195231
Horiz/Vert Lines           6079    10515
Rectangles (outline)       2667     4611
Rectangles (filled)       59861   117072
Circles (filled)          20052    31234
Circles (outline)         14215    18548
Triangles (outline)        6509     8487
Triangles (filled)        23761    42804
Rounded rects (outline)    6754     9442
Rounded rects (filled)    61590   118955

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
 
