#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>

/// Create a class that does your own settings, derived from LGFX_Device.
class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ST7789    _panel_instance;
  lgfx::Bus_SPI         _bus_instance;
  //lgfx::Light_PWM     _light_instance;

public:

  LGFX(void)
  {
    { // Configure bus control settings.

      auto cfg = _bus_instance.config(); // Get a structure for bus settings.

      // SPI bus settings
      cfg.spi_host    = SPI3_HOST;       // Select SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // * Due to the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated,
      // so if an error occurs, use SPI2_HOST and SPI3_HOST instead.
      cfg.spi_mode    = 0;               // Set SPI communication mode (0 ~ 3)
      cfg.freq_write  = 80000000;        // SPI clock when sending (up to 80MHz, rounded to 80MHz divided by an integer)
      cfg.freq_read   = 16000000;        // SPI clock when receiving
      cfg.spi_3wire   = true;            // Set true if receiving on the MOSI 
      cfg.use_lock    = true;            // Set true to use transaction lock
      cfg.dma_channel = SPI_DMA_CH_AUTO; // Set the DMA channel to use (0=not use DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=auto setting)
      // * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is recommended for the DMA channel.  
      // Specifying 1ch and 2ch is deprecated.
      cfg.pin_sclk    = 36;              // SPI SCLK
      cfg.pin_mosi    = 32;              // SPI MOSI
      cfg.pin_miso    = -1;  //33;       // SPI MISO    (-1 = disable)
      cfg.pin_dc      = 27;              // SPI D/C A0  (-1 = disable)
      // When using the same SPI bus as the SD card, be sure to set MISO without omitting it.
//
      _bus_instance.config(cfg);               // Reflect the setting value to the bus.
      _panel_instance.setBus(&_bus_instance);  // Sets the bus to the panel.

    }
    { // Set display panel controls.

      auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

      cfg.pin_cs           =    26;  // CS    (-1 = disable)
      cfg.pin_rst          =    28;  // RST   (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSY  (-1 = disable)

      // * The following setting values ​​are general initial values ​​for each panel, so please comment out any unknown items and try them.


      cfg.panel_width      =   172;  // actual visible width
      cfg.panel_height     =   320;  // actual visible height
      cfg.offset_x         =    34;  // Panel offset amount in X direction
      cfg.offset_y         =     0;  // Panel offset amount in Y direction
      cfg.offset_rotation  =     0;  // Rotation direction value offset 0~7 (4~7 is upside down)
      cfg.dummy_read_pixel =     8;  // Number of dummy read bits before pixel read
      cfg.dummy_read_bits  =     1;  // Number of dummy read bits before non-pixel data read
      cfg.readable         = false;  // Data can be read set to true
      cfg.invert           =  true;  // if panel light and dark are inverted set to true
      cfg.rgb_order        = false;  // if panel red and blue are reversed set to true
      cfg.dlen_16bit       = false;  // Set to true for panels that transmit data length in 16-bit units with 16-bit parallel or SPI
      cfg.bus_shared       = false;  // If the bus is shared with the SD card, set to true (bus control with drawJpgFile etc.)

      // Please set below only with drivers that can change the number of pixels such as ST7735 and ILI9163.
      // Please set the following only when the display shifts with a driver with a variable number of pixels such as ST7735 or ILI9163.
      //cfg.memory_width     =   320;  // Maximum width supported by the driver IC
      //cfg.memory_height    =   172;  // Maximum height supported by the driver IC

      _panel_instance.config(cfg);
    }

/*
    { // Set backlight control.  (delete if not necessary)

      auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

      cfg.pin_bl      = 29;                // pin number to which the backlight is connected 
      cfg.invert      = false;             // true to invert the brightness of the backlight
      cfg.freq        = 44100;             // Backlight PWM frequency
      cfg.pwm_channel = 7;                 // PWM channel number to use

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
    }

*/
    setPanel(&_panel_instance); // Sets the panel to use.
  }
};

