#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>

class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB        _bus_instance;
  //lgfx::Panel_RGB    _panel_instance;  // black screen
  //lgfx::Panel_ST7701 _panel_instance;  // wrong colors, IPS display
  //lgfx::Panel_ST7701_guition_esp32_4848S040  _panel_instance;  // only 480x480
  lgfx::Panel_ST7701_360x640  _panel_instance;  // colors are better
  //lgfx::Light_PWM    _light_instance;

  LGFX(void)
  {
    {
      auto cfg = _panel_instance.config();

      cfg.memory_width  = 480;
      cfg.memory_height = 640;
      cfg.panel_width   = 360;  // 480;  // Workaround
      cfg.panel_height  = 640;

      cfg.offset_x = 120;  // Does not help !
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _panel_instance.config_detail();

      cfg.pin_cs   = 10;     // CS
      cfg.pin_sclk = 12;     // SCL
      cfg.pin_mosi = 11;     // SDA

      _panel_instance.config_detail(cfg);
    }

    {
      auto cfg  = _bus_instance.config();
      cfg.panel = &_panel_instance;

      cfg.pin_d0  = 4;       // B0
      cfg.pin_d1  = 1;       // B1
      cfg.pin_d2  = 5;       // B2
      cfg.pin_d3  = 2;       // B3
      cfg.pin_d4  = 6;       // B4

      cfg.pin_d5  = 42;      // G0
      cfg.pin_d6  = 7;       // G1
      cfg.pin_d7  = 41;      // G2
      cfg.pin_d8  = 15;      // G3
      cfg.pin_d9  = 40;      // G4
      cfg.pin_d10 = 16;      // G5

      cfg.pin_d11 = 39;      // R0
      cfg.pin_d12 =  8;      // R1
      cfg.pin_d13 =  0;      // R2
      cfg.pin_d14 =  3;      // R3
      cfg.pin_d15 = 45;      // R4

      cfg.pin_henable = 9;   // DEN
      cfg.pin_vsync   = 48;  // VS
      cfg.pin_hsync   = 13;  // HS
      cfg.pin_pclk    = 47;  // CLK
 
      cfg.freq_write  = 16000000;  // (10,12,14,16) 20MHz is too much

      // Many numbers work. I don't which numbers are the best. 
      cfg.hsync_polarity    =  0;
      cfg.hsync_front_porch =  4;  // 20;  // 8-80
      cfg.hsync_pulse_width =  8;  // 10;  // 4-20 (48)
      cfg.hsync_back_porch  =  4;  // 40;  // 8-80

      cfg.vsync_polarity    =  0;
      cfg.vsync_front_porch = 10;  // 10;  // 4-30
      cfg.vsync_pulse_width =  4;  //  4;  // 2-10 (31)
      cfg.vsync_back_porch  = 14;  // 14;  // 4-30

      cfg.pclk_idle_high    =  1;  // 0
      cfg.de_idle_high      =  0;  // must be 0
      cfg.pclk_active_neg   =  0;  // 1

      _bus_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);
    
    /*
    {
      auto cfg   = _light_instance.config();
      cfg.pin_bl = 14;       // BL
      _light_instance.config(cfg);
    }
    _panel_instance.light(&_light_instance);
    */
    
    setPanel(&_panel_instance);
  }
};

