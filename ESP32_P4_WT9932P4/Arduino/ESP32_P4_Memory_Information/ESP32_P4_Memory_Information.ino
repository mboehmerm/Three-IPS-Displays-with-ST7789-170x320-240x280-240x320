/* Lonely Binary ESP32-P4 Memory Information Test

USB CDC On Boot  : "Enabled"
Flash Size       : 16MB(128Mb)
Partition Scheme : 16MB Flash (3MB APP/9.9MB FATFS)
PSRAM            : Enabled
Upload Mode      : UART0 / Hardware CDC
USB Mode         : Hardware CDC and JTAG 

----------------------------------------------------

=== ESP32-P4 Memory Information ===
Lonely Binary ESP32-P4 N16R32

--- PSRAM Test ---
✅ PSRAM is available!
PSRAM Size: 33554432 bytes (32.00 MB)
✅ PSRAM is initialized and working!

--- Flash Memory Test ---
Flash Size: 16777216 bytes (16.00 MB)
Flash Speed: 80 MHz
Flash Mode: 0
✅ Flash memory information retrieved!

--- SRAM Test ---
Free Heap: 444468 bytes (434.05 KB)
Minimum Free Heap: 439200 bytes (428.91 KB)
Maximum Allocatable Heap: 253940 bytes (247.99 KB)
✅ SRAM information retrieved!

=== Memory Test Complete ===

*/

#include <esp_heap_caps.h>

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for serial to initialize
  
  Serial.println("=== ESP32-P4 Memory Information ===");
  Serial.println("Lonely Binary ESP32-P4 N16R32");
  Serial.println();
  
  // Test PSRAM
  testPSRAM();
  
  // Test Flash Memory
  testFlashMemory();
  
  // Test SRAM
  testSRAM();
  
  Serial.println("=== Memory Test Complete ===");
}

void loop() {
  // Nothing to do in loop
  delay(1000);
}

void testPSRAM() {
  Serial.println("--- PSRAM Test ---");

    // Get PSRAM size
  size_t psramSize = ESP.getPsramSize();
  if (psramSize > 0 ) {
    Serial.println("✅ PSRAM is available!");
    Serial.printf("PSRAM Size: %d bytes (%.2f MB)\n", psramSize, psramSize / 1024.0 / 1024.0);
    
    // Check if PSRAM is initialized
    if (psramInit()) {
      Serial.println("✅ PSRAM is initialized and working!");
    } else {
      Serial.println("❌ PSRAM initialization failed!");
    }
      } else {
    Serial.println("❌ PSRAM not found!");
    Serial.println("❌ PSRAM mode should be OPI PSRAM in Arduino Settings");
  }

  Serial.println();
}

void testFlashMemory() {
  Serial.println("--- Flash Memory Test ---");
  
  // Get Flash memory size
  size_t flashSize = ESP.getFlashChipSize();
  Serial.printf("Flash Size: %d bytes (%.2f MB)\n", flashSize, flashSize / 1024.0 / 1024.0);
  
  // Get Flash chip speed
  uint32_t flashSpeed = ESP.getFlashChipSpeed();
  Serial.printf("Flash Speed: %d MHz\n", flashSpeed / 1000000);
  
  // Get Flash chip mode
  uint8_t flashMode = ESP.getFlashChipMode();
  Serial.printf("Flash Mode: %d\n", flashMode);
  
  Serial.println("✅ Flash memory information retrieved!");
  Serial.println();
}

void testSRAM() {
  Serial.println("--- SRAM Test ---");
  
  // Get free heap size
  size_t freeHeap = ESP.getFreeHeap();
  Serial.printf("Free Heap: %d bytes (%.2f KB)\n", freeHeap, freeHeap / 1024.0);
  
  // Get minimum free heap size
  size_t minFreeHeap = ESP.getMinFreeHeap();
  Serial.printf("Minimum Free Heap: %d bytes (%.2f KB)\n", minFreeHeap, minFreeHeap / 1024.0);
  
  // Get maximum allocatable heap size
  size_t maxAllocHeap = ESP.getMaxAllocHeap();
  Serial.printf("Maximum Allocatable Heap: %d bytes (%.2f KB)\n", maxAllocHeap, maxAllocHeap / 1024.0);
  
  Serial.println("✅ SRAM information retrieved!");
  Serial.println();
}
