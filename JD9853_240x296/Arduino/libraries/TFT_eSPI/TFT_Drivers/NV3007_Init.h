// NV3007 2.79" 142x428 initialisation sequence
// Copied from project config/tft_screens/NV3007_Init.h

{
  // 复位序列
  writecommand(0xFF);
  writedata(0xA5);
  
  writecommand(0x9A);
  writedata(0x08);
  
  writecommand(0x9B);
  writedata(0x08);
  
  writecommand(0x9C);
  writedata(0xB0);
  
  writecommand(0x9D);
  writedata(0x16);
  
  writecommand(0x9E);
  writedata(0xC4);
  
  writecommand(0x8F);
  writedata(0x55);
  writedata(0x04);
  
  writecommand(0x84);
  writedata(0x90);
  
  writecommand(0x83);
  writedata(0x7B);
  
  writecommand(0x85);
  writedata(0x33);
  
  writecommand(0x60);
  writedata(0x00);
  
  writecommand(0x70);
  writedata(0x00);
  
  writecommand(0x61);
  writedata(0x02);
  
  writecommand(0x71);
  writedata(0x02);
  
  writecommand(0x62);
  writedata(0x04);
  
  writecommand(0x72);
  writedata(0x04);
  
  writecommand(0x6C);
  writedata(0x29);
  
  writecommand(0x7C);
  writedata(0x29);
  
  writecommand(0x6D);
  writedata(0x31);
  
  writecommand(0x7D);
  writedata(0x31);
  
  writecommand(0x6E);
  writedata(0x0F);
  
  writecommand(0x7E);
  writedata(0x0F);
  
  writecommand(0x66);
  writedata(0x21);
  
  writecommand(0x76);
  writedata(0x21);
  
  writecommand(0x68);
  writedata(0x3A);
  
  writecommand(0x78);
  writedata(0x3A);
  
  writecommand(0x63);
  writedata(0x07);
  
  writecommand(0x73);
  writedata(0x07);
  
  writecommand(0x64);
  writedata(0x05);
  
  writecommand(0x74);
  writedata(0x05);
  
  writecommand(0x65);
  writedata(0x02);
  
  writecommand(0x75);
  writedata(0x02);
  
  writecommand(0x67);
  writedata(0x23);
  
  writecommand(0x77);
  writedata(0x23);
  
  writecommand(0x69);
  writedata(0x08);
  
  writecommand(0x79);
  writedata(0x08);
  
  writecommand(0x6A);
  writedata(0x13);
  
  writecommand(0x7A);
  writedata(0x13);
  
  writecommand(0x6B);
  writedata(0x13);
  
  writecommand(0x7B);
  writedata(0x13);
  
  writecommand(0x6F);
  writedata(0x00);
  
  writecommand(0x7F);
  writedata(0x00);
  
  writecommand(0x50);
  writedata(0x00);
  
  writecommand(0x52);
  writedata(0xD6);
  
  writecommand(0x53);
  writedata(0x08);
  
  writecommand(0x54);
  writedata(0x08);
  
  writecommand(0x55);
  writedata(0x1E);
  
  writecommand(0x56);
  writedata(0x1C);
  
  // GOA map_sel
  writecommand(0xA0);
  writedata(0x2B);
  writedata(0x24);
  writedata(0x00);
  
  writecommand(0xA1);
  writedata(0x87);
  
  writecommand(0xA2);
  writedata(0x86);
  
  writecommand(0xA5);
  writedata(0x00);
  
  writecommand(0xA6);
  writedata(0x00);
  
  writecommand(0xA7);
  writedata(0x00);
  
  writecommand(0xA8);
  writedata(0x36);
  
  writecommand(0xA9);
  writedata(0x7E);
  
  writecommand(0xAA);
  writedata(0x7E);
  
  writecommand(0xB9);
  writedata(0x85);
  
  writecommand(0xBA);
  writedata(0x84);
  
  writecommand(0xBB);
  writedata(0x83);
  
  writecommand(0xBC);
  writedata(0x82);
  
  writecommand(0xBD);
  writedata(0x81);
  
  writecommand(0xBE);
  writedata(0x80);
  
  writecommand(0xBF);
  writedata(0x01);
  
  writecommand(0xC0);
  writedata(0x02);
  
  writecommand(0xC1);
  writedata(0x00);
  
  writecommand(0xC2);
  writedata(0x00);
  
  writecommand(0xC3);
  writedata(0x00);
  
  writecommand(0xC4);
  writedata(0x33);
  
  writecommand(0xC5);
  writedata(0x7E);
  
  writecommand(0xC6);
  writedata(0x7E);
  
  writecommand(0xC8);
  writedata(0x33);
  writedata(0x33);
  
  writecommand(0xC9);
  writedata(0x68);
  
  writecommand(0xCA);
  writedata(0x69);
  
  writecommand(0xCB);
  writedata(0x6A);
  
  writecommand(0xCC);
  writedata(0x6B);
  
  writecommand(0xCD);
  writedata(0x33);
  writedata(0x33);
  
  writecommand(0xCE);
  writedata(0x6C);
  
  writecommand(0xCF);
  writedata(0x6D);
  
  writecommand(0xD0);
  writedata(0x6E);
  
  writecommand(0xD1);
  writedata(0x6F);
  
  writecommand(0xAB);
  writedata(0x03);
  writedata(0x67);
  
  writecommand(0xAC);
  writedata(0x03);
  writedata(0x6B);
  
  writecommand(0xAD);
  writedata(0x03);
  writedata(0x68);
  
  writecommand(0xAE);
  writedata(0x03);
  writedata(0x6C);
  
  writecommand(0xB3);
  writedata(0x00);
  
  writecommand(0xB4);
  writedata(0x00);
  
  writecommand(0xB5);
  writedata(0x00);
  
  writecommand(0xB6);
  writedata(0x32);
  
  writecommand(0xB7);
  writedata(0x7E);
  
  writecommand(0xB8);
  writedata(0x7E);
  
  writecommand(0xE0);
  writedata(0x00);
  
  writecommand(0xE1);
  writedata(0x03);
  writedata(0x0F);
  
  writecommand(0xE2);
  writedata(0x04);
  
  writecommand(0xE3);
  writedata(0x01);
  
  writecommand(0xE4);
  writedata(0x0E);
  
  writecommand(0xE5);
  writedata(0x01);
  
  writecommand(0xE6);
  writedata(0x19);
  
  writecommand(0xE7);
  writedata(0x10);
  
  writecommand(0xE8);
  writedata(0x10);
  
  writecommand(0xEA);
  writedata(0x12);
  
  writecommand(0xEB);
  writedata(0xD0);
  
  writecommand(0xEC);
  writedata(0x04);
  
  writecommand(0xED);
  writedata(0x07);
  
  writecommand(0xEE);
  writedata(0x07);
  
  writecommand(0xEF);
  writedata(0x09);
  
  writecommand(0xF0);
  writedata(0xD0);
  
  writecommand(0xF1);
  writedata(0x0E);
  
  // 1 dot
  writecommand(0xE9);
  writedata(0x29);
  
  writecommand(0xEC);
  writedata(0x04);
  
  // TE
  writecommand(0x35);
  writedata(0x00);
  
  writecommand(0x44);
  writedata(0x00);
  writedata(0x10);
  
  writecommand(0x46);
  writedata(0x10);
  
  writecommand(0xFF);
  writedata(0x00);
  
  writecommand(0x3A);
  writedata(0x05);
  
  writecommand(0x11); // Sleep out
  delay(220);
  
  writecommand(0x29); // Display on
  delay(200);
  
  // 打开背光（如果定义了TFT_BL）
  #ifdef TFT_BL
    digitalWrite(TFT_BL, HIGH);
    pinMode(TFT_BL, OUTPUT);
  #endif
}
