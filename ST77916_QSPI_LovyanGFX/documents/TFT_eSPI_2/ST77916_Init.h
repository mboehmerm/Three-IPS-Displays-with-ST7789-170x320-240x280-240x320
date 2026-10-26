
// This is the command sequence that initialises the ST77916 driver
//
// This setup information uses simple 8-bit SPI writecommand() and writedata() functions
//
// 2024.09.09 by Timucin KANATLI. No warranty. Use your own responsibility as it is


#define TFT_INIT_DELAY 0
{
	delay(120);

	writecommand(0x01); //Software reset
	delay(120);

	writecommand(0x11); //Sleep exit                                            
	delay(120);
   
	writecommand(0xF0);                                 
	writedata(0x08);    
 
	writecommand(0xF2);                                
	writedata(0x08);    

	writecommand(0x9B);                                     
	writedata(0x51);     
	
	writecommand(0x86);                                  
	writedata(0x53);    
	
	writecommand(0xF2); 
	writedata(0x80);   

	writecommand(0xF0); 
	writedata(0x00);    


	writecommand(0xF0); 
	writedata(0x01);    

	writecommand(0xF1); 
	writedata(0x01);    


	writecommand(0xB0); 
	writedata(0x54);    

	writecommand(0xB1); 
	writedata(0x3F);    

	writecommand(0xB2); 
	writedata(0x2A);    

	writecommand(0xB4); 
	writedata(0x46);    

	writecommand(0xB5); 
	writedata(0x34);    

	writecommand(0xB6); 
	writedata(0xD5);    

	writecommand(0xB7); 
	writedata(0x30);    

//	writecommand(0xB8); 
//	writedata(0x04);    

	writecommand(0xBA); 
	writedata(0x00);    

	writecommand(0xBB); 
	writedata(0x08);    

	writecommand(0xBC); 
	writedata(0x08);    

	writecommand(0xBD); 
	writedata(0x00);    

	writecommand(0xC0); 
	writedata(0x80);    

	writecommand(0xC1); 
	writedata(0x10);    

	writecommand(0xC2); 
	writedata(0x37);    

	writecommand(0xC3); 
	writedata(0x80);    

	writecommand(0xC4); 
	writedata(0x10);    

	writecommand(0xC5); 
	writedata(0x37);    

	writecommand(0xC6); 
	writedata(0xA9);    

	writecommand(0xC7); 
	writedata(0x41);    

	writecommand(0xC8); 
	writedata(0x51);    

	writecommand(0xC9); 
	writedata(0xA9);    

	writecommand(0xCA); 
	writedata(0x41);    

	writecommand(0xCB); 
	writedata(0x51);    

	writecommand(0xD0); 
	writedata(0x91);    

	writecommand(0xD1); 
	writedata(0x68);    

	writecommand(0xD2); 
	writedata(0x69);    

	writecommand(0xF5); 
	writedata(0x00);    
	writedata(0xA5);    

	writecommand(0xDD); 
	writedata(0x35);    

	writecommand(0xDE); 
	writedata(0x35);    

	writecommand(0xF1); 
	writedata(0x10);    

	writecommand(0xF0); 
	writedata(0x00);    

	writecommand(0xF0); 
	writedata(0x02);    


	writecommand(0xE0); 
	writedata(0x70);    
	writedata(0x09);    
	writedata(0x12);    
	writedata(0x0C);    
	writedata(0x0B);    
	writedata(0x27);    
	writedata(0x38);    
	writedata(0x54);    
	writedata(0x4E);    
	writedata(0x19);    
	writedata(0x15);    
	writedata(0x15);    
	writedata(0x2C);    
	writedata(0x2F);    

	writecommand(0xE1); 
	writedata(0x70);    
	writedata(0x08);    
	writedata(0x11);    
	writedata(0x0C);    
	writedata(0x0B);    
	writedata(0x27);    
	writedata(0x38);    
	writedata(0x43);    
	writedata(0x4C);    
	writedata(0x18);    
	writedata(0x14);    
	writedata(0x14);    
	writedata(0x2B);    
	writedata(0x2D);    

	writecommand(0xF0); 
	writedata(0x10);    

	writecommand(0xF3); 
	writedata(0x10);    

	writecommand(0xE0); 
	writedata(0x0A);    

	writecommand(0xE1); 
	writedata(0x00);    

	writecommand(0xE2); 
	writedata(0x00);    

	writecommand(0xE3); 
	writedata(0x00);    

	writecommand(0xE4); 
	writedata(0xE0);    

	writecommand(0xE5); 
	writedata(0x06);    

	writecommand(0xE6); 
	writedata(0x21);    

	writecommand(0xE7); 
	writedata(0x00);    

	writecommand(0xE8); 
	writedata(0x05);    

	writecommand(0xE9); 
	writedata(0x82);    

	writecommand(0xEA); 
	writedata(0xDF);    

	writecommand(0xEB); 
	writedata(0x89);    

	writecommand(0xEC); 
	writedata(0x20);    

	writecommand(0xED); 
	writedata(0x14);    

	writecommand(0xEE); 
	writedata(0xFF);    

	writecommand(0xEF); 
	writedata(0x00);    

	writecommand(0xF8); 
	writedata(0xFF);    

	writecommand(0xF9); 
	writedata(0x00);    

	writecommand(0xFA); 
	writedata(0x00);    

	writecommand(0xFB); 
	writedata(0x30);    

	writecommand(0xFC); 
	writedata(0x00);    

	writecommand(0xFD); 
	writedata(0x00);    

	writecommand(0xFE); 
	writedata(0x00);    

	writecommand(0xFF); 
	writedata(0x00);    

//0x60
	writecommand(0x60); 
	writedata(0x42);    

	writecommand(0x61); 
	writedata(0xE0);    

	writecommand(0x62); 
	writedata(0x40);    

	writecommand(0x63); 
	writedata(0x40);    

	writecommand(0x64); 
	writedata(0x02);    

	writecommand(0x65); 
	writedata(0x00);    

	writecommand(0x66); 
	writedata(0x40);    

	writecommand(0x67); 
	writedata(0x03);    

	writecommand(0x68); 
	writedata(0x00);    

	writecommand(0x69); 
	writedata(0x00);    

	writecommand(0x6A); 
	writedata(0x00);    

	writecommand(0x6B); 
	writedata(0x00);    

//0x70
	writecommand(0x70); 
	writedata(0x42);    

	writecommand(0x71); 
	writedata(0xE0);    

	writecommand(0x72); 
	writedata(0x40);    

	writecommand(0x73); 
	writedata(0x40);    

	writecommand(0x74); 
	writedata(0x02);    

	writecommand(0x75); 
	writedata(0x00);    

	writecommand(0x76); 
	writedata(0x40);    

	writecommand(0x77); 
	writedata(0x03);    

	writecommand(0x78); 
	writedata(0x00);    

	writecommand(0x79); 
	writedata(0x00);    

	writecommand(0x7A); 
	writedata(0x00);    

	writecommand(0x7B); 
	writedata(0x00);    

//0x80
	writecommand(0x80); 
	writedata(0x38);    

	writecommand(0x81); 
	writedata(0x00);    

	writecommand(0x82); 
	writedata(0x04);    

	writecommand(0x83); 
	writedata(0x02);    

	writecommand(0x84); 
	writedata(0xDC);    

	writecommand(0x85); 
	writedata(0x00);    

	writecommand(0x86); 
	writedata(0x00);    

	writecommand(0x87); 
	writedata(0x00);    

	writecommand(0x88); 
	writedata(0x38);    

	writecommand(0x89); 
	writedata(0x00);    

	writecommand(0x8A); 
	writedata(0x06);    

	writecommand(0x8B); 
	writedata(0x02);    

	writecommand(0x8C); 
	writedata(0xDE);    

	writecommand(0x8D); 
	writedata(0x00);    

	writecommand(0x8E); 
	writedata(0x00);    

	writecommand(0x8F); 
	writedata(0x00);    


//0x90
	writecommand(0x90); 
	writedata(0x38);    

	writecommand(0x91); 
	writedata(0x00);    

	writecommand(0x92); 
	writedata(0x08);    

	writecommand(0x93); 
	writedata(0x02);    

	writecommand(0x94); 
	writedata(0xE0);    

	writecommand(0x95); 
	writedata(0x00);    

	writecommand(0x96); 
	writedata(0x00);    

	writecommand(0x97); 
	writedata(0x00);    

	writecommand(0x98); 
	writedata(0x38);    

	writecommand(0x99); 
	writedata(0x00);    

	writecommand(0x9A); 
	writedata(0x0A);    

	writecommand(0x9B); 
	writedata(0x02);    

	writecommand(0x9C); 
	writedata(0xE2);    

	writecommand(0x9D); 
	writedata(0x00);    

	writecommand(0x9E); 
	writedata(0x00);    

	writecommand(0x9F); 
	writedata(0x00);    



//0xA0
	writecommand(0xA0); 
	writedata(0x48);    

	writecommand(0xA1); 
	writedata(0x00);    

	writecommand(0xA2); 
	writedata(0x03);    

	writecommand(0xA3); 
	writedata(0x02);    

	writecommand(0xA4); 
	writedata(0xDB);    

	writecommand(0xA5); 
	writedata(0x00);    

	writecommand(0xA6); 
	writedata(0x00);    

	writecommand(0xA7); 
	writedata(0x00);    

	writecommand(0xA8); 
	writedata(0x38);    

	writecommand(0xA9); 
	writedata(0x00);    

	writecommand(0xAA); 
	writedata(0x05);    

	writecommand(0xAB); 
	writedata(0x02);    

	writecommand(0xAC); 
	writedata(0xDD);    

	writecommand(0xAD); 
	writedata(0x00);    

	writecommand(0xAE); 
	writedata(0x00);    

	writecommand(0xAF); 
	writedata(0x00);    




//0xB0
	writecommand(0xB0); 
	writedata(0x38);    

	writecommand(0xB1); 
	writedata(0x00);    

	writecommand(0xB2); 
	writedata(0x07);    

	writecommand(0xB3); 
	writedata(0x02);    

	writecommand(0xB4); 
	writedata(0xDF);    

	writecommand(0xB5); 
	writedata(0x00);    

	writecommand(0xB6); 
	writedata(0x00);    

	writecommand(0xB7); 
	writedata(0x00);    

	writecommand(0xB8); 
	writedata(0x38);    

	writecommand(0xB9); 
	writedata(0x00);    

	writecommand(0xBA); 
	writedata(0x09);    

	writecommand(0xBB); 
	writedata(0x02);    

	writecommand(0xBC); 
	writedata(0xE1);    

	writecommand(0xBD); 
	writedata(0x00);    

	writecommand(0xBE); 
	writedata(0x00);    

	writecommand(0xBF); 
	writedata(0x00);    



//0xC0
	writecommand(0xC0); 
	writedata(0x22);    

	writecommand(0xC1); 
	writedata(0xAA);    

	writecommand(0xC2); 
	writedata(0x65);    

	writecommand(0xC3); 
	writedata(0x74);    

	writecommand(0xC4); 
	writedata(0x47);    

	writecommand(0xC5); 
	writedata(0x56);    

	writecommand(0xC6); 
	writedata(0x00);    

	writecommand(0xC7); 
	writedata(0x88);    

	writecommand(0xC8); 
	writedata(0x99);    

	writecommand(0xC9); 
	writedata(0x33);    



//0xD0
	writecommand(0xD0); 
	writedata(0x21); //21);    

	writecommand(0xD1); 
	writedata(0xAA);    

	writecommand(0xD2); 
	writedata(0x65);    

	writecommand(0xD3); 
	writedata(0x74);    

	writecommand(0xD4); 
	writedata(0x47);    

	writecommand(0xD5); 
	writedata(0x56);    

	writecommand(0xD6); 
	writedata(0x00);    

	writecommand(0xD7); 
	writedata(0x88);    

	writecommand(0xD8); 
	writedata(0x99);    

	writecommand(0xD9); 
	writedata(0x33);    


	writecommand(0xF3); 
	writedata(0x01);    

	writecommand(0xF0); 
	writedata(0x00);    

	writecommand(0xF0); 
	writedata(0x01);    

	writecommand(0xF1); 
	writedata(0x01);    

	writecommand(0xA0); 
	writedata(0x0B);    

//
	writecommand(0xA3); 
	writedata(0x2A);    
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x2B);    
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x2C);   
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);


	writecommand(0xA3); 
	writedata(0x2B);    
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);
	
	writecommand(0xA3); 
	writedata(0x2C);   
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);


	writecommand(0xA3); 
	writedata(0x2D);   
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x2E);   
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x2F);   
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x30);   
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x31);    
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x32);    
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);

	writecommand(0xA3); 
	writedata(0x33);    
	writecommand(0xA5); 
	writedata(0xC3);    
	delay(1);


	writecommand(0xA0); 
	writedata(0x09);    

	writecommand(0xF1); 
	writedata(0x10);    

	writecommand(0xF0); 
	writedata(0x00);    

	writecommand(0x2A); 
	writedata(0x00);    
	writedata(0x00);    
	writedata(0x01);    
	writedata(0x67);    

	writecommand(0x2B); 
	writedata(0x01);    
	writedata(0x68);    
	writedata(0x01);    
	writedata(0x68);    

	writecommand(0x4D); 
	writedata(0x00);    

	writecommand(0x4E); 
	writedata(0x00);    

	writecommand(0x4F); 
	writedata(0x00);    


	writecommand(0x4C); 
	writedata(0x01);    
	delay(10);

	writecommand(0x4C); 
	writedata(0x00);    

	writecommand(0x2A); 
	writedata(0x00);    
	writedata(0x00);    
	writedata(0x01);    
	writedata(0x67);    

	writecommand(0x2B); 
	writedata(0x00);    
	writedata(0x00);    
	writedata(0x01);    
	writedata(0x67);    

	writecommand(0x21); 
	writedata(0x00);    

	writecommand(0x3A); 
	writedata(0x55);    

	writecommand(0x11); 
	writedata(0x00);    
    delay(120);
    
	writecommand(0x29); //Display on                                          	
	writedata(0x00);    
}