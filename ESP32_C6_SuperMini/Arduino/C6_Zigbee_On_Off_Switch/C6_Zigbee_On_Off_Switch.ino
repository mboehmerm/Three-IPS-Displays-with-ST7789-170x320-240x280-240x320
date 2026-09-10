// C6_Zigbee_On_Off_Switch , ZIGBEE_COORDINATOR

// https://wiki.seeedstudio.com/xiao_esp32c5_zigbee_arduino/

// Copyright 2024 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @brief This example demonstrates simple Zigbee light switch.
 *
 * The example demonstrates how to use Zigbee library to control a light bulb.
 * The light bulb is a Zigbee end device, which is controlled by a Zigbee coordinator (Switch).
 * Button switch and Zigbee runs in separate tasks.
 *
 * Proper Zigbee mode must be selected in Tools->Zigbee mode
 * and also the correct partition scheme must be selected in Tools->Partition Scheme.
 *
 * Please check the README.md for instructions and more detailed description.
 *
 * Created by Jan Procházka (https://github.com/P-R-O-C-H-Y/)
 */

#include <Arduino.h>
#ifndef ZIGBEE_MODE_ZCZR
#error "Zigbee coordinator mode is not selected in Tools->Zigbee mode"
#endif

#include "Zigbee.h" 

/* Zigbee switch configuration */  //  Tradfri remote 704.85.95   (E1525/) E1743
#define SWITCH_ENDPOINT_NUMBER 5

#define GPIO_INPUT_IO_TOGGLE_SWITCH BOOT_PIN //// ESP32-C6 Boot Pin 9
#define PAIR_SIZE(TYPE_STR_PAIR)    (sizeof(TYPE_STR_PAIR) / sizeof(TYPE_STR_PAIR[0]))

typedef enum {
  SWITCH_ON_CONTROL,
  SWITCH_OFF_CONTROL,
  SWITCH_ONOFF_TOGGLE_CONTROL,
  SWITCH_LEVEL_UP_CONTROL,
  SWITCH_LEVEL_DOWN_CONTROL,
  SWITCH_LEVEL_CYCLE_CONTROL,
  SWITCH_COLOR_CONTROL,
} SwitchFunction;

typedef struct {
  uint8_t pin;
  SwitchFunction func;
} SwitchData;

typedef enum {
  SWITCH_IDLE,
  SWITCH_PRESS_ARMED,
  SWITCH_PRESS_DETECTED,
  SWITCH_PRESSED,
  SWITCH_RELEASE_DETECTED,
} SwitchState;

static SwitchData buttonFunctionPair[] = {{GPIO_INPUT_IO_TOGGLE_SWITCH, SWITCH_ONOFF_TOGGLE_CONTROL}};

ZigbeeSwitch zbSwitch = ZigbeeSwitch(SWITCH_ENDPOINT_NUMBER);

static bool light_state = false;

/********************* Zigbee functions **************************/
static void onZbButton(SwitchData *button_func_pair) {
  if (button_func_pair->func == SWITCH_ONOFF_TOGGLE_CONTROL) {
    // Send toggle command to the light
    Serial.println("Toggling light");
    zbSwitch.lightToggle();
  } else {
    Serial.println("Toggling light with external Switch");    // keine Änderung
    zbSwitch.lightToggle();
  }
}
  //  void lightToggle(uint8_t endpoint, uint16_t short_addr);
  //  void lightToggle(uint8_t endpoint, esp_zb_ieee_addr_t ieee_addr);
  // You can get the endpoints and addresses from the list of bound devices, which you can get by calling getBoundDevices() 
  // as its done in the example at the end of setup() after // Optional: List all bound devices and read manufacturer and model name.
  // https://github.com/espressif/arduino-esp32/issues/10867
  
static void onLightStateChange(bool state) {
  if (state != light_state) {
    light_state = state;
    Serial.printf("Light state changed to %d\r\n", state);
  }
}

/********************* Periodic task ***************************/
void periodicTask(void *arg) {
  while (true) {
    // print the bound lights every 10 seconds
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 30000) {
      lastPrint = millis();
      zbSwitch.printBoundDevices(Serial);
    }

    // Poll light state every second
    static uint32_t lastPoll = 0;
    if (millis() - lastPoll > 1000) {
      lastPoll = millis();
      zbSwitch.getLightState();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

/********************* GPIO functions **************************/
static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR onGpioInterrupt(void *arg) {
  xQueueSendFromISR(gpio_evt_queue, (SwitchData *)arg, NULL);
}

static void enableGpioInterrupt(bool enabled) {
  for (int i = 0; i < PAIR_SIZE(buttonFunctionPair); ++i) {
    if (enabled) {
      enableInterrupt((buttonFunctionPair[i]).pin);
    } else {
      disableInterrupt((buttonFunctionPair[i]).pin);
    }
  }
}

/********************* Arduino functions **************************/
void setup() {
  Serial.begin(115200);

    // By default, the smallest LQI value for network joining is 32. 
    // You can use the esp_zb_secur_network_min_join_lqi_set() function to set a lower LQI threshold.
    // ezb_nwk_set_min_join_lqi(0);  // was not declared in this scope
    
      esp_zb_secur_network_min_join_lqi_set(0);

    // advanced debug mode 
      Zigbee.setDebugMode(true); 

  //Optional: set Zigbee device name and model
  zbSwitch.setManufacturerAndModel("Espressif", "ZigbeeSwitch");

  //Optional to allow multiple light to bind to the switch
  zbSwitch.allowMultipleBinding(true);

  zbSwitch.onLightStateChange(onLightStateChange);

  //Add endpoint to Zigbee Core
  Serial.println("Adding ZigbeeSwitch endpoint to Zigbee Core");
  Zigbee.addEndpoint(&zbSwitch);

  //Open network for 180 seconds after boot
  Zigbee.setRebootOpenNetwork(180);

  // Init button switch
  for (int i = 0; i < PAIR_SIZE(buttonFunctionPair); i++) {
    pinMode(buttonFunctionPair[i].pin, INPUT_PULLUP);
    /* create a queue to handle gpio event from isr */
    gpio_evt_queue = xQueueCreate(10, sizeof(SwitchData));
    if (gpio_evt_queue == 0) {
      Serial.println("Queue creating failed, rebooting...");
      ESP.restart();
    }
    attachInterruptArg(buttonFunctionPair[i].pin, onGpioInterrupt, (void *)(buttonFunctionPair + i), FALLING);
  }

// Test 

// esp_zb_set_tx_power has a maximum value of 20dB, a minimum value of -24dB, and an adjustment step of 3dB,
// resulting in a total of 15 levels. The default setting is at the maximum level of 15 for transmit power,
// which corresponds to the highest transmission power of 100mA. 
// To use esp_zb_set_tx_power correctly, please ensure that you call it after esp_zb_init().(I have tested this)

// -24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,20 ???

//The function esp_zb_get_tx_power() has been moved and renamed to ezb_get_tx_power() 


//int8_t power;
//esp_zb_get_tx_power(&power);  // compiles
//esp_zb_set_tx_power(20);  // compiles !



  // When all EPs are registered, start Zigbee with ZIGBEE_COORDINATOR mode
  if (!Zigbee.begin(ZIGBEE_COORDINATOR)) {
    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting..."); 
    ESP.restart();
  }

  Serial.println("Waiting for Light to bound to the switch");
  //Wait for switch to bound to a light:
  while (!zbSwitch.bound()) {
    Serial.printf(".");
    delay(500);
  }

  // Optional: List all bound devices and read manufacturer and model name
  std::list<zb_device_params_t *> boundLights = zbSwitch.getBoundDevices();
  for (const auto &device : boundLights) {
    Serial.printf("Device on endpoint %u, short address: 0x%x\r\n", device->endpoint, device->short_addr);
    Serial.printf(
      "IEEE Address: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n", device->ieee_addr[7], device->ieee_addr[6], device->ieee_addr[5], device->ieee_addr[4],
      device->ieee_addr[3], device->ieee_addr[2], device->ieee_addr[1], device->ieee_addr[0]
    );
    char *manufacturer = zbSwitch.readManufacturer(device->endpoint, device->short_addr, device->ieee_addr);
    char *model = zbSwitch.readModel(device->endpoint, device->short_addr, device->ieee_addr);
    if (manufacturer != nullptr) {
      Serial.printf("Light manufacturer: %s\r\n", manufacturer);
    }
    if (model != nullptr) {
      Serial.printf("Light model: %s\r\n", model);
    }
  }

  Serial.println();

  xTaskCreate(periodicTask, "periodicTask", 1024 * 4, NULL, 10, NULL);
}

void loop() {
  // Handle button switch in loop()
  uint8_t pin = 0;
  SwitchData buttonSwitch;
  static SwitchState buttonState = SWITCH_IDLE;
  bool eventFlag = false;

  /* check if there is any queue received, if yes read out the buttonSwitch */
  if (xQueueReceive(gpio_evt_queue, &buttonSwitch, portMAX_DELAY)) {
    pin = buttonSwitch.pin;
    enableGpioInterrupt(false);
    eventFlag = true;
  }
  while (eventFlag) {
    bool value = digitalRead(pin);
    switch (buttonState) {
      case SWITCH_IDLE:           buttonState = (value == LOW) ? SWITCH_PRESS_DETECTED : SWITCH_IDLE; break;
      case SWITCH_PRESS_DETECTED: buttonState = (value == LOW) ? SWITCH_PRESS_DETECTED : SWITCH_RELEASE_DETECTED; break;
      case SWITCH_RELEASE_DETECTED:
        buttonState = SWITCH_IDLE;
        /* callback to button_handler */
        (*onZbButton)(&buttonSwitch);
        break;
      default: break;
    }
    if (buttonState == SWITCH_IDLE) {
      enableGpioInterrupt(true);
      eventFlag = false;
      break;
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

/*

C6SM_ED_Zigbee_On_Off_Switch_.ino            Endpoint 19

mit geänderter esp_zigbee_ha_standard.h und esp_zigbee_type.h

[  1345][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1346][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 19, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 17
[  1347][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x4731, profile_id: 0x0104, security_status: 0, lqi: 132, rx_time: 0
[  1348][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x4731) src endpoint(19) to dst endpoint(5) cluster(0x0)
[  1349][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(9)
[  1350][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Espressif"    // funktioniert

[  1353][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1353][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 19, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 21
[  1354][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x4731, profile_id: 0x0104, security_status: 0, lqi: 132, rx_time: 0
[  1355][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x4731) src endpoint(19) to dst endpoint(5) cluster(0x0)
[  1356][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x5), type(0x42), value(13)
[  1357][I][ZigbeeEP.cpp:494] zbReadBasicCluster(): Peer Model is "ZigbeeSwitch2"    // funktioniert

[  3267][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x5: ERROR    // funktioniert nicht, aber was ?
[  3267][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command    // funktioniert nicht, aber was ?
[  4268][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x6: ERROR
[  4268][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  5269][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x7: ERROR
[  5269][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command

...

[ 44345][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 44345][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 19, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 5
[ 44346][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x4731, profile_id: 0x0104, security_status: 0, lqi: 193, rx_time: 0
[ 44347][V][ZigbeeHandlers.cpp:495] zb_cmd_default_resp_handler(): Received default response: from address(0x4731), src_endpoint(19) to dst_endpoint(5), cluster(0x6) with status 0xc3
[ 44349][V][ZigbeeEP.cpp:830] zbDefaultResponse(): Default response received for endpoint 5
[ 44349][V][ZigbeeEP.cpp:831] zbDefaultResponse(): Status code: Cluster is not found on the target endpoint
[ 44350][V][ZigbeeEP.cpp:832] zbDefaultResponse(): Response to command: 0
[ 45309][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x2f: ERROR    // neu !!! 0x2f wird einfach hochgezählt !!!
[ 45309][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command     // neu !!!
...


andere Version

C6SM_ED_Zigbee_On_Off_Switch_old.ino            Endpoint 17

(Extended PAN ID: 58:e6:c5:ff:fe:1d:dd:b8, PAN ID: 0xb369, Channel:13, Short Address: 0x78a7)


Device on endpoint 10, short address: 0xffff, ieee address: fc:01:2c:ff:fe:f6:50:b0
Device on endpoint 17, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:24

[ 21469][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 21491][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 21492][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 17, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 5
[ 21493][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x78a7, profile_id: 0x0104, security_status: 0, lqi: 198, rx_time: 0
[ 21494][V][ZigbeeHandlers.cpp:495] zb_cmd_default_resp_handler(): Received default response: from address(0x78a7), src_endpoint(17) to dst_endpoint(5), cluster(0x6) with status 0xc3
[ 21495][V][ZigbeeEP.cpp:830] zbDefaultResponse(): Default response received for endpoint 5
[ 21496][V][ZigbeeEP.cpp:831] zbDefaultResponse(): Status code: Cluster is not found on the target endpoint
[ 21496][V][ZigbeeEP.cpp:832] zbDefaultResponse(): Response to command: 0


ESP_ZB_ZCL_STATUS_UNSUP_CLUST

C6SM_ED_Zigbee_On_Off_Switch.ino            Endpoint 18

  GPIO : BUS_TYPE[bus/unit][chan]
  --------------------------------------  
     9 : GPIO
    12 : USB_DM
    13 : USB_DP
============ After Setup End =============
[ 10864][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 10865][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 10866][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x5363, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 10867][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x5363) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 10868][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)

[ 10885][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 10885][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 18, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 5
[ 10886][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x1baa, profile_id: 0x0104, security_status: 0, lqi: 239, rx_time: 0
[ 10888][V][ZigbeeHandlers.cpp:495] zb_cmd_default_resp_handler(): Received default response: from address(0x1baa), src_endpoint(18) to dst_endpoint(5), cluster(0x6) with status 0xc3
[ 10889][V][ZigbeeEP.cpp:830] zbDefaultResponse(): Default response received for endpoint 5
[ 10889][V][ZigbeeEP.cpp:831] zbDefaultResponse(): Status code: Cluster is not found on the target endpoint
[ 10890][V][ZigbeeEP.cpp:832] zbDefaultResponse(): Response to command: 0

[ 10893][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 10893][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 19
[ 10894][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x5363, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 10896][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x5363) src endpoint(10) to dst endpoint(5) cluster(0x0)
[ 10897][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x5), type(0x42), value(11)
[ 10898][I][ZigbeeEP.cpp:494] zbReadBasicCluster(): Peer Model is "ZBLightBulb"

[ 10924][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 10925][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 18, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 20
[ 10926][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x1baa, profile_id: 0x0104, security_status: 0, lqi: 239, rx_time: 0
[ 10927][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x1baa) src endpoint(18) to dst endpoint(5) cluster(0x0)
[ 10928][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x5), type(0x42), value(12)
[ 10928][I][ZigbeeEP.cpp:494] zbReadBasicCluster(): Peer Model is "ZigbeeSwitch"

[ 11326][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 11327][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 18, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 17
[ 11328][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x1baa, profile_id: 0x0104, security_status: 0, lqi: 239, rx_time: 0
[ 11329][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x1baa) src endpoint(18) to dst endpoint(5) cluster(0x0)
[ 11330][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(9)
[ 11330][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Espressif"
[ 12845][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x5: ERROR
[ 12845][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command


Bound devices:
Device on endpoint 10, short address: 0xffff, ieee address: fc:01:2c:ff:fe:f6:50:b0
Device on endpoint 18, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:24















Adding ZigbeeSwitch endpoint to Zigbee Core
Waiting for Light to bound to the switch
Device on endpoint 10, short address: 0xffff
IEEE Address: FC:01:2C:FF:FE:F6:50:B0
Light manufacturer: Espressif
Device on endpoint 11, short address: 0xffff 
IEEE Address: 58:E6:C5:FF:FE:1C:69:18

Light state changed to 1
Toggling light
Light state changed to 0
Bound devices:
Device on endpoint 10, short address: 0xffff, ieee address: fc:01:2c:ff:fe:f6:50:b0
Device on endpoint 11, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:18
Toggling light
Light state changed to 1

Zwei Steckdosen :

Device on endpoint 1, short address: 0xe9a5, ieee address: a4:c1:38:45:5e:44:e0:51
Device on endpoint 1, short address: 0xa75f, ieee address: a4:c1:38:19:36:1e:c2:b2
Toggling light
Light state changed to 1

Bound devices:
Device on endpoint 10, short address: 0xffff, ieee address: fc:01:2c:ff:fe:f6:50:b0    Power Plug
Device on endpoint 11, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:18    Power Plug
Device on endpoint 1, short address: 0xffff, ieee address: a4:c1:38:45:5e:44:e0:51     Light 1
Device on endpoint 1, short address: 0xffff, ieee address: a4:c1:38:19:36:1e:c2:b2     Light 2
Device on endpoint 14, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:24    Button EP 14 
Device on endpoint 1, short address: 0x7dc2, ieee address: 6c:5c:b1:ff:fe:0f:e9:11     Tradfri


=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32-C6
  Package           : 1
  Revision          : 0.02
  Cores             : 1
  CPU Frequency     : 160 MHz
  XTAL Frequency    : 4eatures Bitfield : 0x00000052
  Embedded Flash    : No
  Embedded PSRAM    : No
  2.4GHz WiFi       : Yes
  Classic BT        : No
  BT Low Energy     : Yes
  IEEE 802.15.4     : Yes
------------------------------------------
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   430284 B ( 420.2 KB)
  Free Bytes        :   394552 B ( 385.3 KB)
  Allocated Bytes   :    28476 B (  27.8 KB)
  Minimum Free Bytes:   389724 B ( 380.6 KB)
  Largest Free Block:   368628 B ( 360.0 KB)
------------------------------------------
Flash Info:
------------------------------------------
  Chip Size         :  4194304B (4 MB)
  Block Size        :    65536B (  64.0 KB)
  Sector Size       :     4096B (   4.0 KB)
  Page Size         :      256B (   0.2 KB)
  Bus Speed         : 80 MHz
  Flash Frequency   : 80 MHz (source: 80 MHz, divider: 1)
  Bus Mode          : QIO
------------------------------------------
Partitions Info:
------------------------------------------
                nvs : addr: 0x00009000, size:    20.0 KB, type: DATA, subtype: NVS
            otadata : addr: 0x0000E000, size:     8.0 KB, type: DATA, subtype: OTA
               app0 : addr: 0x00010000, size:  1280.0 KB, type:  APP, subtype: OTA_0
               app1 : addr: 0x00150000, size:  1280.0 KB, type:  APP, subtype: OTA_1
             spiffs : addr: 0x00290000, size:  1384.0 KB, type: DATA, subtype: SPIFFS
         zb_storage : addr: 0x003EA000, size:    16.0 KB, type: DATA, subtype: FAT
             zb_fct : addr: 0x003EE000, size:     4.0 KB, type: DATA, subtype: FAT
             rcp_fw : addr: 0x003EF000, size:     4.0 KB, type: DATA, subtype: SPIFFS
           coredump : addr: 0x003F0000, size:    64.0 KB, type: DATA, subtype: COREDUMP
------------------------------------------
Software Info:
------------------------------------------
  Compile Date/Time : Aug 29 2026 10:23:57
  Compile Host OS   : windows
  ESP-IDF Version   : v5.5.5
  Arduino Version   : 3.3.11
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : MAKERGO_C6_SUPERMINI
  Arduino Variant   : makergo_c6_supermini
  Arduino FQBN      : esp32:esp32:makergo_c6_supermini:UploadSpeed=921600,CDCOnBoot=cdc,CPUFreq=160,FlashFreq=80,FlashMode=qio,FlashSize=4M,PartitionScheme=zigbee_zczr,DebugLevel=verbose,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=zczr
============ Before Setup End ============
[  1183][V][esp32-hal-uart.c:1085] uartSetPins(): UART0: Driver not yet installed, storing pins for later attachment (RX:17, TX:16)
[  1183][I][esp32-hal-periman.c:170] perimanSetPinBus(): Pin 12 already has type USB_DM (39) with bus 0x40814778
[  1184][I][esp32-hal-periman.c:170] perimanSetPinBus(): Pin 13 already has type USB_DP (40) with bus 0x40814778
Adding ZigbeeSwitch endpoint to Zigbee Core

[  1185][D][ZigbeeCore.cpp:127] addEndpoint(): Endpoint: 5, Device ID: 0x0000

[  1185][V][esp32-hal-periman.c:267] perimanSetBusDeinit(): Deinit function for type GPIO (1) successfully set to 0x420072e8
[  1186][V][esp32-hal-periman.c:192] perimanSetPinBus(): Pin 9 successfully set to type GPIO (1) with bus 0xa
[  1187][D][ZigbeeCore.cpp:175] zigbeeInit(): Initialize Zigbee stack
[  1252][D][ZigbeeCore.cpp:182] zigbeeInit(): Register all Zigbee EPs in list
[  1254][I][ZigbeeCore.cpp:190] zigbeeInit(): List of registered Zigbee EPs:

[  1254][I][ZigbeeCore.cpp:192] zigbeeInit(): Device type: General On/Off switch, Endpoint: 5, Device ID: 0x0000

[  1260][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Config Ready (0x17), status: ESP_FAIL
[  1261][I][ZigbeeCore.cpp:290] esp_zb_app_signal_handler(): Zigbee stack initialized
[  1262][D][ZigbeeCore.cpp:291] esp_zb_app_signal_handler(): Zigbee channel mask: 0x07fff800
[  1266][I][ZigbeeCore.cpp:462] esp_zb_app_signal_handler(): Network(0xe643) closed, devices joining not allowed.
[  1267][I][ZigbeeCore.cpp:297] esp_zb_app_signal_handler(): Device started up in non factory-reset mode
[  1268][I][ZigbeeCore.cpp:310] esp_zb_app_signal_handler(): Device rebooted
[  1268][I][ZigbeeCore.cpp:314] esp_zb_app_signal_handler(): Opening network for joining for 180 seconds
[  1269][D][ZigbeeCore.cpp:778] searchBindings(): Requesting binding table for address 0x0000
[  1270][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1270][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 255, src_addr_mode: 78, cluster_id: 0x0033, asdu_length: 2
[  1271][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x0000, profile_id: 0x0000, security_status: 0, lqi: 0, rx_time: 0
[  1273][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1273][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x8033, asdu_length: 68
[  1275][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x0000, profile_id: 0x0000, security_status: 0, lqi: 0, rx_time: 0
[  1276][D][ZigbeeCore.cpp:586] bindingTableCb(): Binding table callback for address 0x0000 with status 0
[  1276][D][ZigbeeCore.cpp:590] bindingTableCb(): Binding table info: total 6, index 0, count 3
[  1277][D][ZigbeeCore.cpp:635] bindingTableCb(): Processing record 0: src_endp 5, dst_endp 10, cluster_id 0x0006, dst_addr_mode 3
[  1277][D][ZigbeeCore.cpp:635] bindingTableCb(): Processing record 1: src_endp 5, dst_endp 11, cluster_id 0x0006, dst_addr_mode 3
[  1278][D][ZigbeeCore.cpp:635] bindingTableCb(): Processing record 2: src_endp 5, dst_endp 1, cluster_id 0x0006, dst_addr_mode 3
[  1279][D][ZigbeeCore.cpp:645] bindingTableCb(): Requesting next chunk of binding table (current index: 0, count: 3, total: 6)
[  1280][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1281][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x0000, profile_id: 0x0000, security_status: 0, lqi: 0, rx_time: 0
[  1283][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1283][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x8033, asdu_length: 68
[  1284][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x0000, profile_id: 0x0000, security_status: 0, lqi: 0, rx_time: 0
[  1285][D][ZigbeeCore.cpp:586] bindingTableCb(): Binding table callback for address 0x0000 with status 0
[  1286][D][ZigbeeCore.cpp:590] bindingTableCb(): Binding table info: total 6, index 3, count 3
[  1286][D][ZigbeeCore.cpp:635] bindingTableCb(): Processing record 0: src_endp 5, dst_endp 1, cluster_id 0x0006, dst_addr_mode 3
[  1287][D][ZigbeeCore.cpp:635] bindingTableCb(): Processing record 1: src_endp 5, dst_endp 14, cluster_id 0x0006, dst_addr_mode 3
[  1287][D][ZigbeeCore.cpp:635] bindingTableCb(): Processing record 2: src_endp 5, dst_endp 1, cluster_id 0x0006, dst_addr_mode 3
[  1288][D][ZigbeeCore.cpp:650] bindingTableCb(): Processing final chunk of binding table, total records: 6
[  1289][D][ZigbeeCore.cpp:671] bindingTableCb(): Processing endpoint 5
[  1289][D][ZigbeeCore.cpp:717] bindingTableCb(): Processing binding record for EP 5
[  1290][D][ZigbeeCore.cpp:752] bindingTableCb(): Device bound to EP 5 -> device endpoint: 10, ieee addr: FC:01:2C:FF:FE:F6:50:B0
[  1290][D][ZigbeeCore.cpp:717] bindingTableCb(): Processing binding record for EP 5
[  1291][D][ZigbeeCore.cpp:752] bindingTableCb(): Device bound to EP 5 -> device endpoint: 11, ieee addr: 58:E6:C5:FF:FE:1C:69:18
[  1291][D][ZigbeeCore.cpp:717] bindingTableCb(): Processing binding record for EP 5
[  1292][D][ZigbeeCore.cpp:752] bindingTableCb(): Device bound to EP 5 -> device endpoint: 1, ieee addr: A4:C1:38:45:5E:44:E0:51
[  1292][D][ZigbeeCore.cpp:717] bindingTableCb(): Processing binding record for EP 5
[  1293][D][ZigbeeCore.cpp:752] bindingTableCb(): Device bound to EP 5 -> device endpoint: 1, ieee addr: A4:C1:38:19:36:1E:C2:B2
[  1294][D][ZigbeeCore.cpp:717] bindingTableCb(): Processing binding record for EP 5

[  1294][D][ZigbeeCore.cpp:752] bindingTableCb(): Device bound to EP 5 -> device endpoint: 14, ieee addr: 58:E6:C5:FF:FE:1C:69:24

[  1295][D][ZigbeeCore.cpp:717] bindingTableCb(): Processing binding record for EP 5
[  1295][D][ZigbeeCore.cpp:752] bindingTableCb(): Device bound to EP 5 -> device endpoint: 1, ieee addr: 6C:5C:B1:FF:FE:0F:E9:11
[  1296][D][ZigbeeCore.cpp:765] bindingTableCb(): Filling bounded devices finished
Waiting for Light to bound to the switch
Device on endpoint 10, short address: 0xffff
IEEE Address: FC:01:2C:FF:FE:F6:50:B0
[  1318][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1319][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 17
[  1320][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[  1321][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x0)
[  1322][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(9)
[  1323][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Espressif"
  1324][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x1: ERROR
Light manufacturer: Espressif
Device on endpoint 11, short address: 0xffff
IEEE Address: 58:E6:C5:FF:FE:1C:69:18
[  1326][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x2: ERROR
[  1328][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3: ERROR
Device on endpoint 1, short address: 0xffff
IEEE Address: A4:C1:38:45:5E:44:E0:51
[  1330][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x4: ERROR
[  1331][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x5: ERROR
Device on endpoint 1, short address: 0xffff
IEEE Address: A4:C1:38:19:36:1E:C2:B2
[  1334][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x6: ERROR
[  1336][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x7: ERROR
Device on endpoint 14, short address: 0xffff
IEEE Address: 58:E6:C5:FF:FE:1C:69:24
[  1339][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x8: ERROR
[  1342][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x9: ERROR
Device on endpoint 1, short address: 0xffff
IEEE Address: 6C:5C:B1:FF:FE:0F:E9:11
[  1343][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0xa: ERROR
[  1345][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0xb: ERROR

[  1346][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0xc: ERROR
[  1347][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
=========== After Setup Start ============
INTERNAL Memory Info:
------------------------------------------
  Total Size        :   430284 B ( 420.2 KB)
  Free Bytes        :   354528 B ( 346.2 KB)
  Allocated Bytes   :    67372 B (  65.8 KB)
  Minimum Free Bytes:   354244 B ( 345.9 KB)
  Largest Free Block:   335860 B ( 328.0 KB)
------------------------------------------
GPIO Info:
------------------------------------------
  GPIO : BUS_TYPE[bus/unit][chan]
  --------------------------------------  
     9 : GPIO
    12 : USB_DM
    13 : USB_DP
============ After Setup End =============
[  1372][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  1393][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  1408][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1409][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 19
[  1410][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[  1411][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x0)
[  1412][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x5), type(0x42), value(11)
[  1413][I][ZigbeeEP.cpp:494] zbReadBasicCluster(): Peer Model is "ZBLightBulb"
[  1417][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1418][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 15
[  1419][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 122, rx_time: 0
[  1421][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x0)
[  1422][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(7)
[  1422][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Zbeacon"
[  1425][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1425][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  1426][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[  1427][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[  1428][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  1475][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1476][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  1477][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 117, rx_time: 0
[  1478][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[  1479][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  1736][I][ZigbeeCore.cpp:460] esp_zb_app_signal_handler(): Network(0xe643) is open for 180 seconds
[  1793][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1793][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 15
[  1794][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 102, rx_time: 0
[  1795][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x0)
[  1796][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(7)
[  1797][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Zbeacon"
[  1840][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0


[  1841][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 1, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 95, cluster_id: 0x000a, asdu_length: 5


[  1842][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 91, rx_time: 0
[  1895][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  1896][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  1897][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 91, rx_time: 0
[  1898][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[  1899][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  2347][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0xd: ERROR
[  2347][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  2375][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  2401][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  2401][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  2402][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 244, rx_time: 0
[  2403][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[  2404][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  2410][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  3348][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0xe: ERROR
[  3348][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  3382][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  3386][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  3386][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  3387][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 214, rx_time: 0
[  3388][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[  3389][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  3410][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  3418][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  3418][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  3419][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 137, rx_time: 0  
  3420][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[  3421][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  4349][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0xf: ERROR
[  4349][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  4371][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  4389][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  4407][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  4426][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  4448][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  5350][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x10: ERROR
[  5351][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  6169][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  6169][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 14
[  6170][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 147, rx_time: 0
[  6171][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x0)
[  6172][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x5), type(0x42), value(6)
[  6173][I][ZigbeeEP.cpp:494] zbReadBasicCluster(): Peer Model is "TS011F"
[  6351][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x11: ERROR
[  6352][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  7353][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x12: ERROR
[  7354][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  7435][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  7436][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  7437][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 153, rx_time: 0
[  7438][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[  7439][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  8354][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x13: ERROR
[  8355][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  9007][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9007][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9008][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9010][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9011][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9012][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9012][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9013][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9014][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9015][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9015][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9016][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9020][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  9021][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 255, src_addr_mode: 133, cluster_id: 0x0013, asdu_length: 12
[  9022][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0xfffd, src_short_addr: 0x9085, profile_id: 0x0000, security_status: 0, lqi: 255, rx_time: 0
[  9023][I][ZigbeeCore.cpp:375] esp_zb_app_signal_handler(): New device commissioned or rejoined (short: 0x9085)
[  9023][V][ZigbeeCore.cpp:379] esp_zb_app_signal_handler(): Device capabilities: 0x8c
[  9024][D][ZigbeeCore.cpp:393] esp_zb_app_signal_handler(): Checking endpoint 5
[  9024][D][ZigbeeCore.cpp:403] esp_zb_app_signal_handler(): Device already bound to endpoint 5
[  9031][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9053][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  9054][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 15
[  9055][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 153, rx_time: 0
[  9056][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x0)
[  9057][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(7)
[  9058][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Zbeacon"
[  9190][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  9191][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  9192][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 153, rx_time: 0
[  9193][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[  9194][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  9355][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x14: ERROR
[  9356][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[  9373][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9391][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9412][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[  9412][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[  9413][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[  9415][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[  9415][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[  9427][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[  9449][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 10032][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 10356][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x15: ERROR
[ 10356][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 10372][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 10391][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 10409][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 10428][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 10456][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 10457][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 10457][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 10459][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 10459][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 10491][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 11033][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 11357][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x16: ERROR
[ 11357][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 11370][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 11388][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 11407][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 11423][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 11424][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 11425][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 11426][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 11427][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 12000][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12034][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12161][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 12162][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 12163][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 153, rx_time: 0
[ 12164][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 12165][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 12263][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12358][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x17: ERROR
[ 12358][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 12374][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12392][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12411][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12433][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12443][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 12443][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 12444][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 142, rx_time: 0
[ 12445][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 12446][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 12469][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 12472][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 12473][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 12474][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 12475][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 12476][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 12491][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 13264][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 13353][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 13353][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 13359][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x18: ERROR
[ 13359][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 13375][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 13393][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 13412][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 13436][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 13450][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 13450][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 13451][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 13453][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 13453][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 14351][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 14354][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 14360][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x19: ERROR
[ 14360][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 14376][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 14400][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 14437][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 14438][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 14439][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 14440][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 14441][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 15001][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 15355][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 15356][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 15361][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x1a: ERROR
[ 15362][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 15380][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 15412][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 15431][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 15458][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 15471][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 15471][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 15473][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 15474][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 15475][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
Toggling light
[ 16252][V][ZigbeeSwitch.cpp:119] lightToggle(): Sending 'light toggle' command
[ 16271][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 16272][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 5
[ 16273][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 16274][V][ZigbeeHandlers.cpp:495] zb_cmd_default_resp_handler(): Received default response: from address(0x9085), src_endpoint(10) to dst_endpoint(5), cluster(0x6) with status 0x0
[ 16275][V][ZigbeeEP.cpp:830] zbDefaultResponse(): Default response received for endpoint 5
[ 16276][V][ZigbeeEP.cpp:831] zbDefaultResponse(): Status code: Success
[ 16276][V][ZigbeeEP.cpp:832] zbDefaultResponse(): Response to command: 2
[ 16353][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 16356][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 16362][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x1c: ERROR
[ 16363][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 16381][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 16399][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 16426][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 16442][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 16443][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 16444][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 16445][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 16446][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(1)
Light state changed to 1
[ 16578][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 16579][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0000, asdu_length: 15
[ 16580][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 102, rx_time: 0
[ 16581][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x0)
[ 16582][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x0), attribute(0x4), type(0x42), value(7)
[ 16583][I][ZigbeeEP.cpp:481] zbReadBasicCluster(): Peer Manufacturer is "Zbeacon"
[ 16691][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 16832][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 17087][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 17088][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 17089][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 102, rx_time: 0
[ 17090][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 17091][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
Light state changed to 0
Toggling light
 17102][V][ZigbeeSwitch.cpp:119] lightToggle(): Sending 'light toggle' command
[ 17122][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 17122][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 5
[ 17123][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 17124][V][ZigbeeHandlers.cpp:495] zb_cmd_default_resp_handler(): Received default response: from address(0x9085), src_endpoint(10) to dst_endpoint(5), cluster(0x6) with status 0x0
[ 17125][V][ZigbeeEP.cpp:830] zbDefaultResponse(): Default response received for endpoint 5
[ 17126][V][ZigbeeEP.cpp:831] zbDefaultResponse(): Status code: Success
[ 17126][V][ZigbeeEP.cpp:832] zbDefaultResponse(): Response to command: 2
[ 17355][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 17363][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x1e: ERROR
[ 17363][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 17382][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 17406][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 17414][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 17415][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 17416][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 249, rx_time: 0
[ 17417][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 17418][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 18002][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 18364][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x1f: ERROR
[ 18364][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 18381][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 18400][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 18418][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 18437][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 18453][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 18454][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 18455][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 255, rx_time: 0
[ 18456][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 18457][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 18484][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 18484][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 18485][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 158, rx_time: 0
[ 18487][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 18488][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 19032][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK

*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*
[ 47697][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 47706][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x38: ERROR
[ 47706][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 47722][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 47740][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 47756][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 47757][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 47758][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 204, rx_time: 0
[ 47759][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 47760][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 47817][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 47928][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 47929][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 47930][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 127, rx_time: 0
[ 47931][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 47932][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 48353][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 48360][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 48671][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 48702][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 48707][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x39: ERROR
[ 48707][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 48728][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 48773][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 48779][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 48780][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 48781][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 209, rx_time: 0
[ 48782][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 48783][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 49354][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 49672][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 49708][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3a: ERROR
[ 49708][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 49724][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 49746][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 49766][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 49767][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 49768][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 204, rx_time: 0
[ 49769][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 49770][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 49783][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 50673][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 50702][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 50709][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3b: ERROR
[ 50709][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 50726][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 50759][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 50767][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 50767][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 50768][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 198, rx_time: 0
[ 50769][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 50770][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 51674][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 51710][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3c: ERROR
[ 51711][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 51729][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 51751][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 51777][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 51786][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 51787][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 51788][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 204, rx_time: 0
[ 51789][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 51790][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 51799][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0


[ 51800][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 1, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 95, cluster_id: 0x0006, asdu_length: 7


[ 51801][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 132, rx_time: 0
[ 52704][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 52711][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3d: ERROR
[ 52711][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 52732][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 52750][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 52770][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 52771][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 52772][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 198, rx_time: 0
[ 52773][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 52774][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 52792][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 53712][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3e: ERROR
[ 53712][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 53728][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 53746][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 53762][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 53763][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 53764][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 204, rx_time: 0
[ 53765][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 53766][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 54673][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 54713][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x3f: ERROR
[ 54713][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 54732][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 54751][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 54769][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 54802][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 54803][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 54803][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 204, rx_time: 0
[ 54805][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 54805][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 54829][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 55714][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x40: ERROR
[ 55715][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 55733][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 55751][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 55770][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 55795][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 55795][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 55796][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 198, rx_time: 0
[ 55798][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 55798][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 55813][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 56708][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 56715][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x41: ERROR
[ 56715][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 56732][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 56764][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 56777][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 56778][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 56779][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 198, rx_time: 0
[ 56780][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 56781][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 56839][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0

[ 56839][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8

[ 56840][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 127, rx_time: 0
[ 56841][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 56842][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 57711][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 57716][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x42: ERROR
[ 57716][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 57732][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 57750][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 57769][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 57802][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 57802][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 255, cluster_id: 0x0006, asdu_length: 8
[ 57803][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 204, rx_time: 0
[ 57805][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 57805][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 57826][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 57830][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 58361][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 58710][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 58711][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 58717][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x43: ERROR
[ 58718][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 58736][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 58754][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 58773][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 58806][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 58816][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 58816][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 58817][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 198, rx_time: 0
[ 58819][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 58819][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 59718][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x44: ERROR
[ 59718][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 59734][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 59756][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 59776][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 59777][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 59778][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 219, rx_time: 0
[ 59779][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 59780][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 59828][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 60681][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 60712][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
Bound devices:
Device on endpoint 10, short address: 0xffff, ieee address: fc:01:2c:ff:fe:f6:50:b0
Device on endpoint 11, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:18
Device on endpoint 1, short address: 0xffff, ieee address: a4:c1:38:45:5e:44:e0:51
Device on endpoint 1, short address: 0xffff, ieee address: a4:c1:38:19:36:1e:c2:b2
Device on endpoint 14, short address: 0xffff, ieee address: 58:e6:c5:ff:fe:1c:69:24
Device on endpoint 1, short address: 0xffff, ieee address: 6c:5c:b1:ff:fe:0f:e9:11
[ 60720][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x45: ERROR
[ 60720][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 60736][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 60754][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 60779][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 60804][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 60805][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 60806][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 229, rx_time: 0
[ 60807][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 60808][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 60816][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 61717][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 61721][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x46: ERROR
[ 61721][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 61738][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 61757][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 61781][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 61799][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 61800][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 61801][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 234, rx_time: 0
[ 61802][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 61802][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 61822][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 61946][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0

[ 61947][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8

[ 61948][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 122, rx_time: 0
[ 61949][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 61950][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 62148][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 62148][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 0, src_endpoint: 0, dst_addr_mode: 255, src_addr_mode: 48, cluster_id: 0x0013, asdu_length: 12
[ 62149][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0xfffd, src_short_addr: 0xff30, profile_id: 0x0000, security_status: 0, lqi: 137, rx_time: 0
[ 62151][I][ZigbeeCore.cpp:375] esp_zb_app_signal_handler(): New device commissioned or rejoined (short: 0xff30)
[ 62151][V][ZigbeeCore.cpp:379] esp_zb_app_signal_handler(): Device capabilities: 0x80
[ 62151][D][ZigbeeCore.cpp:393] esp_zb_app_signal_handler(): Checking endpoint 5
[ 62152][D][ZigbeeCore.cpp:403] esp_zb_app_signal_handler(): Device already bound to endpoint 5
[ 62714][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 62722][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x47: ERROR
[ 62723][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 62743][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 62768][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 62768][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 62769][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 234, rx_time: 0
[ 62771][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 62771][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 62807][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 62832][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 62833][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 62834][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 122, rx_time: 0
[ 62835][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 62836][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 63291][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 63292][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 6
[ 63293][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xff30, profile_id: 0x0104, security_status: 0, lqi: 147, rx_time: 0
[ 63294][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xff30) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 63295][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(134), cluster(0x6), attribute(0x0), type(0x0), value(0)
[ 63353][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0

[ 63353][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8

[ 63354][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 122, rx_time: 0
[ 63355][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 63356][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 63717][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 63723][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x48: ERROR
[ 63723][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 63739][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 63757][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 63776][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
 63796][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 63797][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 63798][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 234, rx_time: 0
[ 63799][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 63800][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 63811][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 64718][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 64724][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x49: ERROR
[ 64724][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 64740][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 64758][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 64777][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 64797][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 64798][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 64799][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 234, rx_time: 0
[ 64800][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 64801][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 64816][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 64835][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0

[ 64836][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8

[ 64837][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 127, rx_time: 0
[ 64838][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 64839][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 65725][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x4a: ERROR
[ 65725][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 65743][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 65761][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 65790][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 65791][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 65792][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 229, rx_time: 0
[ 65793][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 65794][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 66136][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0

[ 66136][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 36, cluster_id: 0x0006, asdu_length: 8

[ 66137][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xa75f, profile_id: 0x0104, security_status: 0, lqi: 127, rx_time: 0
[ 66139][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xa75f) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 66140][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 66718][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 66726][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x4b: ERROR
[ 66727][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 66743][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 66765][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 66783][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 66819][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 66829][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 66830][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 66830][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 229, rx_time: 0
[ 66832][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 66832][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 67719][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 67727][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x4c: ERROR
[ 67727][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 67741][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 67759][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 67778][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 67807][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 67808][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 67809][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 229, rx_time: 0
[ 67810][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 67811][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 67817][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 68689][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 68720][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 68721][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK
[ 68728][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x4d: ERROR
[ 68729][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 68747][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 68767][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 68768][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 68769][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 224, rx_time: 0
[ 68770][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 68771][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)
[ 68785][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 68835][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 69506][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0

[ 69507][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 1, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 6

[ 69508][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0xff30, profile_id: 0x0104, security_status: 0, lqi: 132, rx_time: 0
[ 69509][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0xff30) src endpoint(1) to dst endpoint(5) cluster(0x6)
[ 69510][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(134), cluster(0x6), attribute(0x0), type(0x0), value(0)
[ 69724][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: ZDO Device Unavailable (0x3c), status: ESP_OK

[ 69729][E][ZigbeeEP.cpp:153] readClusterAttribute(): Failed to read attribute: 0x4e: ERROR

[ 69729][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command
[ 69745][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 69763][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 69782][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 69808][V][ZigbeeCore.cpp:481] esp_zb_app_signal_handler(): ZDO signal: NLME Status Indication (0x32), status: ESP_OK
[ 69850][D][ZigbeeCore.cpp:488] zb_apsde_data_indication_handler(): APSDE INDICATION - Received APSDE-DATA indication, status: 0
[ 69850][D][ZigbeeCore.cpp:489] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_endpoint: 5, src_endpoint: 10, dst_addr_mode: 255, src_addr_mode: 0, cluster_id: 0x0006, asdu_length: 8
[ 69851][D][ZigbeeCore.cpp:493] zb_apsde_data_indication_handler(): APSDE INDICATION - dst_short_addr: 0x0000, src_short_addr: 0x9085, profile_id: 0x0104, security_status: 0, lqi: 137, rx_time: 0
[ 69852][V][ZigbeeHandlers.cpp:154] zb_cmd_read_attr_resp_handler(): Read attribute response: from address(0x9085) src endpoint(10) to dst endpoint(5) cluster(0x6)
[ 69853][V][ZigbeeHandlers.cpp:163] zb_cmd_read_attr_resp_handler(): Read attribute response: status(0), cluster(0x6), attribute(0x0), type(0x10), value(0)








[ 37876][E][ZigbeeSwitch.cpp:419] getLightState(): Failed to send read light state command



*/
