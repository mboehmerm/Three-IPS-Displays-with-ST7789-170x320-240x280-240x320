//----------------------------------------------------------------------
/*EASTRISING TECHNOLOGY CO,.LTD.*/
// Module       : ER-TFT020-4    240RGB*296
// Lanuage      : C51 Code
// Create       : JAVEN
// Date         : 2025.12.10
// LCM Drive IC : JD9853   
// INTERFACE    : 4 WIRE SPI  
// MCU          : STC12LE5A60S2
// VDD          : 3.0V                 
//*************************************************************************** 

#include <STC12C5A.H>
#include <stdio.h>
#include <intrins.h>
#include<lcd.h>
#include<word.h>

//=============================================================
//Define sd card PIN IO 
sbit SD_CLK = P1^7;
sbit SD_DI  = P1^5;
sbit SD_DO  = P1^6;
sbit SD_CS  = P1^4;
//===========================================================

//===========================================================
//BUFFER
unsigned char xdata DATA[512];

//===========================================================
//write one byte to SD
void SdWrite(unsigned char n)
{unsigned char i;

	for(i=8;i;i--)
	{
		SD_CLK=0;
		SD_DI=(n&0x80);
		n<<=1;
		SD_CLK=1;
	}

	SD_DI=1; 
} 
//===========================================================
////read one byte
unsigned char SdRead()
{unsigned char n,i;
	for(i=8;i;i--)
	{
	SD_CLK=0;
	SD_CLK=1;
	n<<=1;
	if(SD_DO) n|=1;

	}

	return n;
}
//============================================================
//Detecting the response of the SD card
unsigned char SdResponse()
{unsigned char i=0,response;

	while(i<=8)
	{
	response = SdRead();
	if(response==0x00)
	break;
	if(response==0x01)
	break;
	i++;
	}

	return response;
} 
//================================================================
//Send commands to the SD card
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{
	SdWrite(command|0x40);
	SdWrite(((unsigned char *)&argument)[0]);
	SdWrite(((unsigned char *)&argument)[1]);
	SdWrite(((unsigned char *)&argument)[2]);
	SdWrite(((unsigned char *)&argument)[3]);
	SdWrite(CRC);
}
//================================================================
//Initialize the SD card
unsigned char SdInit(void)
{	int delay=0, trials=0;
 	unsigned char i;
	unsigned char response=0x01;

	SD_CS=1;
	for(i=0;i<=9;i++)
	SdWrite(0xff);
	SD_CS=0;
	
	//Send Command 0 to put MMC in SPI mode
	SdCommand(0x00,0,0x95);
	
	
	response=SdResponse();
	
	if(response!=0x01)
	{
	return 0;
	} 
	
	while(response==0x01)
	{
	SD_CS=1;
	SdWrite(0xff);
	SD_CS=0;
	SdCommand(0x01,0x00ffc000,0xff);
	response=SdResponse();
	} 
	
	SD_CS=1;
	SdWrite(0xff);
	return 1; 
}
//================================================================
//Specify the address to write  the SD card data, a maximum of 512 bytes
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{	unsigned int count;
	unsigned char dataResp;
	//Block size is 512 bytes exactly
	//First Lower SS

	SD_CS=0;
	//Then send write command
	SdCommand(0x18,address,0xff);
	
	if(SdResponse()==00)
	{
		SdWrite(0xff);
		SdWrite(0xff);
		SdWrite(0xff);
		//command was a success - now send data
		//start with DATA TOKEN = 0xFE
		SdWrite(0xfe);
		//now send data
		for(count=0;count<len;count++) SdWrite(*Block++);
		
		for(;count<512;count++) SdWrite(0);
		//data block sent - now send checksum
		SdWrite(0xff); 
		SdWrite(0xff);
		//Now read in the DATA RESPONSE token
		dataResp=SdRead();
		//Following the DATA RESPONSE token
		//are a number of BUSY bytes
		//a zero byte indicates the MMC is busy
		
		while(SdRead()==0);
		
		dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
		SD_CS=1;
		SdWrite(0xff);
		if(dataResp==0x0b)
			{
			//printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
			return 0;
			}
		if(dataResp==0x05)
		return 1;
		
		//printf("Invalid data Response token.\n");
		return 0;
	}
	//printf("Command 0x18 (Write) was not received by the MMC.\n");
	return 0;
}

//=======================================================================
//Specified address from the SD card to read data, a maximum of 512 bytes
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{	unsigned int count;
	//Block size is 512 bytes exactly
	//First Lower SS

 	//printf("MMC_read_block\n");

	SD_CS=0;
	//Then send write command
	SdCommand(0x11,address,0xff);
	
	if(SdResponse()==00)
		{
		//command was a success - now send data
		//start with DATA TOKEN = 0xFE
		while(SdRead()!=0xfe);
		
		for(count=0;count<len;count++) *Block++=SdRead(); 
		
		for(;count<512;count++) SdRead();
		
		//data block sent - now send checksum
		SdRead();
		SdRead();
		//Now read in the DATA RESPONSE token
		SD_CS=1;
		SdRead();
		return 1;
		}
	 //printf("Command 0x11 (Read) was not received by the MMC.\n");
	return 0;
}

//============================================================	
main()
{	unsigned long j;  
  	unsigned int f,i,ya=0;
  	unsigned long AddTemp;

  	P1M1=0x00; //Set P1 port as push-pull output
 	P1M0=0xFF;

 	P3M1=0x00; //Set P3 port as push-pull output
 	P3M0=0xFF;

 	P1=0xff;
 	P2=0xff;
 	P3=0xff;
 	P0=0xff;

	delayms(50);

	lcd_Initial();//LCD Initial
	

	while(1)
	{  	delayms(50);
	
		// TPTEST();			   	 		
 		ClearScreen(0x00);	//balck
 		showzifustr(84,50,"ER-TFT020-4",0x001f,0x0);
		showzifustr(66,70,"www.buydisplay.com",0x00f800,0xffff);	
	   showzifustr(20,90,"drawing Line,Rectangle,Circle",0x00ffe0,0x00);
	   LCD_DrawLine(20,120,220,120,0x07e0);
	   LCD_DrawRectangle(40, 140, 200,180,0xf81f);
 	   LCD_DrawRectangle(0, 0, 239,295,0xf800);
	   Draw_Circle(120,240,40,0xf800);
  
	  	while(KEY);

		ClearScreen(0xf800);
		 while(KEY);	    
		ClearScreen(0x07e0);
		 while(KEY);	 	
		ClearScreen(0x001f);
		 while(KEY);
/*		ClearScreen(0x07ff);
	     while(KEY); 
		ClearScreen(0xffe0);
	     while(KEY); 
		ClearScreen(0xf81f);
	     while(KEY);
*/	  
		ClearScreen(0x0000);	//balck
		 while(KEY);
	
		ClearScreen(0xffff);	//white
		 while(KEY);
	
		DispRGBGray();
	     while(KEY); 
	
		show_picture();        //in MCU
	     while(KEY); 
		 
	
		SdInit();         //SD Card Initial
		f=8;
		AddTemp=389632;
	
		 while(f--)
		 {
		  	   LCD_SetPos(0,0,239,295); 
		 for(j=0;j<278;j++)   
		    {
		    SdReadBlock(DATA,AddTemp+(j*512),512);
		    for(i=0;i<256;i++)                    
			   {   
		
		   	   Write_Data(DATA[2*i],DATA[2*i+1]);	 
		
		       }
		     } 
		     AddTemp = AddTemp+((j+10)*512);          
		     while(KEY);                         
			 
		 } 	
	
	}
}

