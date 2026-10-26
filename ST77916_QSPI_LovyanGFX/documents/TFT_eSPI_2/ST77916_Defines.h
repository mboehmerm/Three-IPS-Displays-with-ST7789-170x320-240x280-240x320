// Change the width and height if required (defined in portrait mode)
// or use the constructor to over-ride defaults
// 2024.09.09 by Timucin KANATLI. No warranty. Use your own responsibility as it is

#ifndef TFT_WIDTH
#define TFT_WIDTH  360
#endif

#ifndef TFT_HEIGHT
#define TFT_HEIGHT 360
#endif

// Generic commands used by TFT_eSPI.cpp
#define TFT_NOP     0x00
#define TFT_SWRST   0x01

#define TFT_INVOFF  0x20
#define TFT_INVON   0x21

#define TFT_DISPOFF 0x28
#define TFT_DISPON  0x29

#define TFT_CASET   0x2A
#define TFT_PASET   0x2B
#define TFT_RAMWR   0x2C
#define TFT_RAMRD   0x2E

#define TFT_MADCTL  0x36
#define TFT_MAD_MY  0x80
#define TFT_MAD_MX  0x40
#define TFT_MAD_MV  0x20
#define TFT_MAD_ML  0x10
#define TFT_MAD_BGR 0x08
#define TFT_MAD_MH  0x04
#define TFT_MAD_RGB 0x00

#ifdef TFT_RGB_ORDER
  #if (TFT_RGB_ORDER == 1)
    #define TFT_MAD_COLOR_ORDER TFT_MAD_RGB
  #else
    #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR
  #endif
#else
  #define TFT_MAD_COLOR_ORDER TFT_MAD_BGR
#endif

  #define TFT_MAD_COLOR_ORDER TFT_MAD_RGB



// ST77916 specific commands
#define ST77916_NOP     0x00
#define ST77916_SWRESET 0x01
#define ST77916_RDDID   0x04
#define ST77916_RDDST   0x09

#define ST77916_SLPIN   0x10
#define ST77916_SLPOUT  0x11
#define ST77916_PTLON   0x12
#define ST77916_NORON   0x13

#define ST77916_RDMODE  0x0A
#define ST77916_RDMADCTL  0x0B
#define ST77916_RDPIXFMT  0x0C
#define ST77916_RDIMGFMT  0x0A
#define ST77916_RDSELFDIAG  0x0F

#define ST77916_INVOFF  0x20
#define ST77916_INVON   0x21

#define ST77916_DISPOFF 0x28
#define ST77916_DISPON  0x29

#define ST77916_CASET   0x2A
#define ST77916_PASET   0x2B
#define ST77916_RAMWR   0x2C
#define ST77916_RAMRD   0x2E

#define ST77916_PTLAR   0x30
#define ST77916_VSCRDEF 0x33
#define ST77916_MADCTL  0x36
#define ST77916_VSCRSADD 0x37
#define ST77916_PIXFMT  0x3A

#define ST77916_WRDISBV  0x51
#define ST77916_RDDISBV  0x52
#define ST77916_WRCTRLD  0x53

#define ST77916_FRMCTR1 0xB1
#define ST77916_FRMCTR2 0xB2
#define ST77916_FRMCTR3 0xB3
#define ST77916_INVCTR  0xB4
#define ST77916_DFUNCTR 0xB6

#define ST77916_PWCTR1  0xC0
#define ST77916_PWCTR2  0xC1
#define ST77916_PWCTR3  0xC2

#define ST77916_VMCTR1  0xC5
#define ST77916_VMCOFF  0xC6

#define ST77916_RDID4   0xD3

#define ST77916_GMCTRP1 0xE0
#define ST77916_GMCTRN1 0xE1

#define ST77916_MADCTL_MY  0x80
#define ST77916_MADCTL_MX  0x40
#define ST77916_MADCTL_MV  0x20
#define ST77916_MADCTL_ML  0x10
#define ST77916_MADCTL_RGB 0x00
#define ST77916_MADCTL_BGR 0x08
#define ST77916_MADCTL_MH  0x04
