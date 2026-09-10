// C6_Zigbee_On_Off_Light , ZIGBEE_ED
// https://wiki.seeedstudio.com/xiao_esp32c5_zigbee_arduino/

// Copyright 2024 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief This example demonstrates simple Zigbee light bulb.
 *
 * The example demonstrates how to use Zigbee library to create a end device light bulb.
 * The light bulb is a Zigbee end device, which is controlled by a Zigbee coordinator. 
 *
 * Proper Zigbee mode must be selected in Tools->Zigbee mode
 * and also the correct partition scheme must be selected in Tools->Partition Scheme.
 *
 * Please check the README.md for instructions and more detailed description.
 *
 * Created by Jan Procházka (https://github.com/P-R-O-C-H-Y/)
 */

#include <Arduino.h>
#ifndef ZIGBEE_MODE_ED
#error "Zigbee end device mode is not selected in Tools->Zigbee mode"
#endif

#include "Zigbee.h"

/* Zigbee light bulb configuration */
#define ZIGBEE_LIGHT_ENDPOINT 10    // First ESP32-C6 10, Second ESP32-C6 11
uint8_t led = 15;                   //// Led Pin 15 ESP32-C6 Super Mini
//uint8_t led = RGB_BUILTIN;
uint8_t button = BOOT_PIN;

ZigbeeLight zbLight = ZigbeeLight(ZIGBEE_LIGHT_ENDPOINT);

/********************* RGB LED functions **************************/
void setLED(bool value) {
  digitalWrite(led, value);
}

/********************* Arduino functions **************************/
void setup() {
  Serial.begin(115200);

  // Init LED and turn it OFF (if LED_PIN == RGB_BUILTIN, the rgbLedWrite() will be used under the hood)
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // Init button for factory reset
  pinMode(button, INPUT_PULLUP);

    // advanced debug mode, added by AndroidCrypto 
    Zigbee.setDebugMode(true); 

  //Optional: set Zigbee device name and model
  zbLight.setManufacturerAndModel("Espressif", "ZBLightBulb");

  //Optional to allow multiple switch to bind to the light ???
  zbLight.allowMultipleBinding(true);

  // Set callback function for light change
  zbLight.onLightChange(setLED);

  //Add endpoint to Zigbee Core
  Serial.println("Adding ZigbeeLight endpoint to Zigbee Core");
  Zigbee.addEndpoint(&zbLight);

  // When all EPs are registered, start Zigbee. By default acts as ZIGBEE_END_DEVICE
  if (!Zigbee.begin()) {
    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting...");
    ESP.restart();
  }
  Serial.println("Connecting to network");
  while (!Zigbee.connected()) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
}

void loop() {
  // Checking button for factory reset
  if (digitalRead(button) == LOW) {  // Push button pressed
    // Key debounce handling
    delay(100);
    int startTime = millis();
    while (digitalRead(button) == LOW) {
      delay(50);
      if ((millis() - startTime) > 3000) {
        // If key pressed for more than 3secs, factory reset Zigbee and reboot 
        Serial.println("Resetting Zigbee to factory and rebooting in 1s.");
        delay(1000);
        Zigbee.factoryReset();
      }
    }
    // Toggle light by pressing the button
    zbLight.setLight(!zbLight.getLightState());
  }
  delay(100);
}

/*

=========== Before Setup Start ===========
Chip Info:
---------p:290] esp_zb_app_signal_handler(): Zigbee stack initialized
[   354][D][ZigbeeCore.cpp:291] esp_zb_app_signal_handler(): Zigbee channel mask: 0x02000000
[   596][W][ZigbeeCore.cpp:325] esp_zb_app_signal_handler(): Commissioning failed, trying again...
...
[  6526][W][ZigbeeCore.cpp:325] esp_zb_app_signal_handler(): Commissioning failed, trying again...
[  7287][I][ZigbeeCore.cpp:297] esp_zb_app_signal_handler(): Device started up in non factory-reset mode
[  7287][I][ZigbeeCore.cpp:310] esp_zb_app_signal_handler(): Device rebooted
[  7288][V][ZigbeeCore.cpp:793] setNVRAMChannelMask(): Channel mask set to 0x02000000
[  7289][D][ZigbeeCore.cpp:778] searchBindings(): Requesting binding table for address 0x9085
[  7290][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  7290][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 1, src_addr_mode: 0, cluster_id: 0x0033, asdu_length: 2
[  7291][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0090, src_short_addr: 0x9085, profile_id: 0x0000, security_status: 0, lqi: 0, rx_time: 0
[  7292][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  7293][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 1, src_addr_mode: 0, cluster_id: 0x8033, asdu_length: 5
[  7294][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0090, src_short_addr: 0x9085, profile_id: 0x0000, security_status: 0, lqi: 0, rx_time: 0
[  7295][D][ZigbeeCore.cpp:586] bindingTableCb(): Binding table callback for address 0x9085 with status 0
[  7296][D][ZigbeeCore.cpp:590] bindingTableCb(): Binding table info: total 0, index 0, count 0
[  7296][D][ZigbeeCore.cpp:593] bindingTableCb(): No binding table entries found
[  7297][D][ZigbeeCore.cpp:596] bindingTableCb(): Clearing bound devices for EP 10
Connecting to network

=========== After Setup Start ============
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   434332 B ( 424.2 KB)
  Free Bytes        :   367328 B ( 358.7 KB)
  Allocated Bytes   :    59068 B (  57.7 KB)
  Minimum Free Bytes:   367328 B ( 358.7 KB)
  Largest Free Block:   344052 B ( 336.0 KB)
------------------------------------------
GPIO Info:
------------------------------------------
  GPIO : BUS_TYPE[bus/unit][chan]
  --------------------------------------  
     9 : GPIO
    12 : USB_DM
    13 : USB_DP
    15 : GPIO
============ After Setup End =============
[  7405][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  7406][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 10, src_endpoint: 5, dst_addr_mode: 2, src_addr_mode: 255, cluster_id: 0x0000, asdu_length: 5
[  7407][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0xffff, src_short_addr: 0x0000, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0

[  7437][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  7438][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 11, src_endpoint: 5, dst_addr_mode: 2, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 5
[  7439][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0xffff, src_short_addr: 0x0000, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0

[  7826][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  7827][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 1, src_endpoint: 5, dst_addr_mode: 2, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 5
[  7828][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0xffff, src_short_addr: 0x0000, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0

[  9929][V][ZigbeeHandlers.cpp:103] zb_attribute_set_handler(): Received message: endpoint(10), cluster(0x6), attribute(0x0), data size(1)

[ 11583][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 11584][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 10, src_endpoint: 5, dst_addr_mode: 2, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 3
[ 11584][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x9085, src_short_addr: 0x0000, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0

[ 11586][V][ZigbeeHandlers.cpp:103] zb_attribute_set_handler(): Received message: endpoint(10), cluster(0x6), attribute(0x0), data size(1)

[ 11803][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 11803][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 10, src_endpoint: 5, dst_addr_mode: 2, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 5
[ 11804][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x9085, src_short_addr: 0x0000, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0

[ 11586][V][ZigbeeHandlers.cpp:103] zb_attribute_set_handler(): Received message: endpoint(10), cluster(0x6), attribute(0x0), data size(1)

[ 36835][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK

*/