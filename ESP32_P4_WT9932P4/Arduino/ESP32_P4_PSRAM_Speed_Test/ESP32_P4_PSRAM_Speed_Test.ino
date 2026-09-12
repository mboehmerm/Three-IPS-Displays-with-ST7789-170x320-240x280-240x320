/*
PSRAM Found! Total size: 33554432 bytes
Free PSRAM before alloc: 33551856 bytes
Starting PSRAM Write Test (1 MB)...
Write Time: 0.0150 s
Write Speed: 66.52 MB/s
Starting PSRAM Read Test (1 MB)...
Read Time: 0.0242 s
Read Speed: 41.26 MB/s
Checksum (to prevent optimization): 133693440
*/

#include <Arduino.h>

#define BUFFER_SIZE (1024 * 1024) // 1 MB test buffer

void setup() {
  Serial.begin(115200);
  delay(2000);

  // 1. Verify PSRAM availability
  if (!psramFound()) {
    Serial.println("PSRAM not found or not initialized!");
    return;
  }
  
  Serial.printf("PSRAM Found! Total size: %d bytes\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM before alloc: %d bytes\n", ESP.getFreePsram());

  // 2. Allocate memory in PSRAM
  uint8_t* testBuffer = (uint8_t*) ps_malloc(BUFFER_SIZE);
  if (testBuffer == NULL) {
    Serial.println("Failed to allocate memory in PSRAM!");
    return;
  }

  // 3. Write Speed Test
  Serial.println("Starting PSRAM Write Test (1 MB)...");
  uint32_t startWrite = micros();
  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    testBuffer[i] = (uint8_t)(i & 0xFF);
  }
  uint32_t endWrite = micros();
  float writeTimeSec = (endWrite - startWrite) / 1000000.0;
  float writeSpeedMB = (BUFFER_SIZE / (1024.0 * 1024.0)) / writeTimeSec;
  
  Serial.printf("Write Time: %.4f s\n", writeTimeSec);
  Serial.printf("Write Speed: %.2f MB/s\n", writeSpeedMB);

  // 4. Read Speed Test
  Serial.println("Starting PSRAM Read Test (1 MB)...");
  volatile uint32_t checksum = 0;
  uint32_t startRead = micros();
  for (size_t i = 0; i < BUFFER_SIZE; i++) {
    checksum += testBuffer[i];
  }
  uint32_t endRead = micros();
  float readTimeSec = (endRead - startRead) / 1000000.0;
  float readSpeedMB = (BUFFER_SIZE / (1024.0 * 1024.0)) / readTimeSec;

  Serial.printf("Read Time: %.4f s\n", readTimeSec);
  Serial.printf("Read Speed: %.2f MB/s\n", readSpeedMB);
  Serial.printf("Checksum (to prevent optimization): %u\n", checksum);

  // Cleanup
  free(testBuffer);
}

void loop() {
  // Nothing to do here
}