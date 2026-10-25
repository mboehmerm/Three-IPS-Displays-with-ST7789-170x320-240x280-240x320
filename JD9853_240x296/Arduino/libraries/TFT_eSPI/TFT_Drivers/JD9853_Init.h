
// This is the command sequence that initialises the JD9853 driver

// https://www.buydisplay.com/2-inch-240x296-ips-tft-lcd-display-spi-interface
// https://www.buydisplay.com/8051/ER-TFT020A1-4_8051_Tutorial.zip

{
  /* Software reset */
  writecommand(0x01);
  delay(20); 

  /* Unlock chip registers */
  writecommand(0xDF);
  writedata(0x98);
  writedata(0x53);
  writecommand(0xDE);
  writedata(0x00);

  /* Power settings */
  writecommand(0xB2);
  writedata(0x25);
  writecommand(0xB7);
  writedata(0x00);
  writedata(0x29);
  writedata(0x00);
  writedata(0x51);
  writecommand(0xBB);
  writedata(0x4F);
  writedata(0x1A);
  writedata(0x55);
  writedata(0x73);
  writedata(0x63);
  writedata(0xF0);
  
  /* Timing configuration */
  writecommand(0xC0);
  writedata(0x44);
  writedata(0x44);
  writecommand(0xC1);
  writedata(0x12);
  writecommand(0xC3);
  writedata(0x7D);
  writedata(0x07);
  writedata(0x14);
  writedata(0x06);
  writedata(0xC8);
  writedata(0x71);
  writedata(0x6C);
  writedata(0x77);
  writecommand(0xC4);
  writedata(0x00);
  writedata(0x00);
  writedata(0x94);
  writedata(0x79);
  writedata(0x25);
  writedata(0x0A);
  writedata(0x16);
  writedata(0x79);
  writedata(0x25);
  writedata(0x0A);
  writedata(0x16);
  writedata(0x82);

  /* Gamma correction */
  writecommand(0xC8);
  writedata(0x3F);
  writedata(0x32);
  writedata(0x2A);
  writedata(0x22);
  writedata(0x26);
  writedata(0x25);
  writedata(0x1E);
  writedata(0x1E);

  writedata(0x1C);
  writedata(0x1C);
  writedata(0x19);
  writedata(0x0E);
  writedata(0x09);
  writedata(0x02);
  writedata(0x06);
  writedata(0x00);

  writedata(0x3F);
  writedata(0x32);
  writedata(0x2A);
  writedata(0x22);
  writedata(0x26);
  writedata(0x25);
  writedata(0x1E);
  writedata(0x1E);

  writedata(0x1C);
  writedata(0x1C);
  writedata(0x19);
  writedata(0x0E);
  writedata(0x09);
  writedata(0x02);
  writedata(0x06);
  writedata(0x00);

  /* Power management */  
  writecommand(0xD0);
  writedata(0x04);
  writedata(0x06);
  writedata(0x6B);
  writedata(0x0F);
  writedata(0x00);
  writecommand(0xD7);
  writedata(0x00);
  writedata(0x30);
  writecommand(0xE6);
  writedata(0x10);

  /* Page 1 settings */
  writecommand(0xDE);
  writedata(0x01);
  writecommand(0xB7);
  writedata(0x03);
  writedata(0x13);
  writedata(0xEF);
  writedata(0x35);
  writedata(0x35);
  writecommand(0xC1);
  writedata(0x14);
  writedata(0x15);
  writedata(0xC0);
  writecommand(0xC2);
  writedata(0x06);
  writedata(0x3A);
  writecommand(0xC4);
  writedata(0x72);
  writedata(0x12);

  /* Back to page 0 */
  writecommand(0xDE);
  writedata(0x00);

  /* Display settings */
  writecommand(0x35);
  writedata(0x00);
  writecommand(0x36);  /* TFT_MADCTL */
  writedata(0x00);     /* TFT_MAD_MX */
  writecommand(0x3A);
  writedata(0x05);
  writecommand(0x2A);
  writedata(0x00);
  writedata(0x00);
  writedata(0x00);
  writedata(0xEF);
  writecommand(0x2B);
  writedata(0x00);
  writedata(0x00);
  writedata(0x01);     /* x: 0..239 */
  writedata(0x27);     /* y: 0..295 */

  /* Exit sleep and display on */
  writecommand(0x11);
  delay(120);
  writecommand(0x29);
  delay(10);
}

