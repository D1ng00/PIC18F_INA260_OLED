/*
 * Project Name: OLED_I2C
 * File: OLED.c
 * Description: OLED 128X32 SSD1306  I2C library c file
 * Author: Gavin Lyons.
 * Complier: xc8 v2.00 compiler
 * PIC: PIC16F1619 
 * IDE:  MPLAB X v5.00
 * Development board: Microchip Curiosity Board, PIC16F1619
 * Created March 2019
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/pic_16F1619_projects
 */

#include "mcc_generated_files/mcc.h"
#include "OLED.h"         
#include "font.h"

#define SSD1306_BUFFER_SIZE   SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / 8
static uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];
static uint8_t buffer[SSD1306_BUFFER_SIZE];

void msDelay(uint8_t ms) {
    uint8_t k;

    for (k = 0; k < ms; k++) {
        __delay_ms(1);
    }
}

//Function to Send command byte to OLED, passed byte

void Oled_Command(uint8_t command) {
    uint8_t data_I2C[2];

    data_I2C[0] = SSD1306_COMMAND;
    data_I2C[1] = command;


    I2C_WriteNBytes(SSD1306_ADDR, data_I2C, 2);

}

//Function to Send data byte to OLED, passed byte

void Oled_Data(uint8_t value) {
    uint8_t data_I2C[2];

    data_I2C[0] = SSD1306_DATA_CONTINUE;
    data_I2C[1] = value;


    I2C_WriteNBytes(SSD1306_ADDR, data_I2C, 2);

}

//Function to init OLED

void Oled_Init(void) {

    Oled_Command(SSD1306_DISPLAY_OFF);
    Oled_Command(SSD1306_SET_DISPLAY_CLOCK_DIV_RATIO);
    Oled_Command(0x80);
    Oled_Command(SSD1306_SET_MULTIPLEX_RATIO);
    Oled_Command(SSD1306_LCDHEIGHT - 1);
    Oled_Command(SSD1306_SET_DISPLAY_OFFSET);
    Oled_Command(0x00);
    Oled_Command(SSD1306_SET_START_LINE | 0x00); // Line: 0
    Oled_Command(SSD1306_CHARGE_PUMP);
    Oled_Command(0x14);
    Oled_Command(SSD1306_MEMORY_ADDR_MODE);
    Oled_Command(0x00); //Hor Addressing Mode is Used (02 is Ver)
    Oled_Command(SSD1306_SET_SEGMENT_REMAP | 0x01);
    Oled_Command(SSD1306_COM_SCAN_DIR_DEC);
    Oled_Command(SSD1306_SET_COM_PINS);
    Oled_Command(0x02);
    Oled_Command(SSD1306_SET_CONTRAST_CONTROL);
    Oled_Command(0x8F);
    Oled_Command(SSD1306_SET_PRECHARGE_PERIOD);
    Oled_Command(0xF1);
    Oled_Command(SSD1306_SET_VCOM_DESELECT);
    Oled_Command(0x40);
    Oled_Command(SSD1306_DISPLAY_ALL_ON_RESUME);
    Oled_Command(SSD1306_NORMAL_DISPLAY);
    Oled_Command(SSD1306_DEACTIVATE_SCROLL);
    Oled_Command(SSD1306_DISPLAY_ON);
}

//function to write a string to OLED, passed string

void Oled_WriteString(char *characters) {
    while (*characters) Oled_WriteCharacter(*characters++);
}

//function to write a character, passed character

void Oled_WriteCharacter(char character) {
    for (int i = 0; i < 5; i++) Oled_Data((ASCII[character - 0x20][i]));
    Oled_Data(0x00);
}

void Oled_SetContrast(uint8_t contrast) {
    Oled_Command(SSD1306_SET_CONTRAST_CONTROL);
    Oled_Command(contrast);
}

//function to clear OLED by writing to it.

void Oled_Clear(void) {
    int i = 0;
    // OLED 128* 32 pixels = total pixels -> (4096 / 1 byte) = 512 passes.
    // SSD1306_CLEAR_SIZE  = 512 for 128* 32 or 1024  for 128*64
    for (i; i < SSD1306_CLEAR_SIZE; i++) {
        Oled_Data(0x00); // clear oled screen  
    }
}

//function to clear OLED  line passed page number

void Oled_ClearLine(uint8_t page_num) {
    Oled_SelectPage(page_num);
    int i = 0;
    // Clear line of 128 pixels of current page
    for (i; i < 128; i++) {
        Oled_Data(0x00); // clear oled screen  
    }
}

// Function to select [page] i.e. row or line number
// 128*32 has page 0-3
// 128* 64 has page 0-7
// Passed page num / byte
// page 0   8 
// page 1   16
// page 2   24
// page 3   32
// page 4   40
// page 5   48
// page 6   56
// page 7   64

void Oled_SelectPage(uint8_t page_num) {
    uint8_t Result = 0xB0 | page_num; // Mask or with zero lets everything thru
    Oled_Command(Result);
    Oled_Command(SSD1306_SET_LOWER_COLUMN);
    Oled_Command(SSD1306_SET_HIGHER_COLUMN);
}


/*void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg)
{

    Oled_Clear();		//Clear the screen

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	//Height
	for (int16_t j = 0; j < h; j++, y++)
	{
		//Width
		for (int16_t i = 0; i < w; i++)
		{
				byte = bitmap[j * byteWidth + i / 8];		// Row/Col of byte

				for (int8_t cnt = 0; cnt < 8; cnt ++)
				{
					//Shift left to right through byte to get each bit if the return val > 0, there is a 1 in that space.
					if ( (byte & (0x80 >> cnt)) == 0)
					{
						//  If 0, Background
						OLED_DrawPixel( x + i + cnt, y, bg);
					}
					else
					{
						//if not 0, Pixel Color
						OLED_DrawPixel( x + i + cnt, y, color );
					}
				}
				// Increment x-pos by 1 byte
				i = i + 7;
			}
		}

	  ssd1306_UpdateScreen();
	  return;

}*/


void OLEDWriteLargeCharacter( char character){
    unsigned char i;                                                 //Set character position
    for(i=0; i<11; i++){
    //LcdWriteData(&(Terminal11x16[z-0x20][2*i]));  //Read font array from PROGMEM and display top half of character
        Oled_Data(character - 32);
    //LcdWriteData((Terminal11x16[z-0x20][2*i]));  //Read font array from PROGMEM and display top half of character
    }Oled_Data(0x00);
                                            //Set position with y position set lower to display bottom half of character
    for(i=0; i<11; i++){
    //LcdWriteData(&(Terminal11x16[z-0x20][(2*i)+1])); //Read font array from PROGMEM and display bottom half of character
        Oled_Data(character - 32); //Read font array from PROGMEM and display bottom half of character
    }Oled_Data(0x00);
}

void OLEDWriteLargeString(char *characters){     
    while(*characters){      
        OLEDWriteLargeCharacter(*characters++);     //Advance location for the next character by character width amount plus padding
    }  
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color) {
    if(x >= SSD1306_LCDWIDTH || y >= SSD1306_LCDHEIGHT) {
        // Don't write outside the buffer
        return;
    }
    
    // Check if pixel should be inverted
    /*if(SSD1306.Inverted) {
        color = (SSD1306_COLOR)!color;
    }*/
    
    // Draw in the right color
    if(color == White) {
        SSD1306_Buffer[x + (y / 8) * SSD1306_LCDWIDTH] |= 1 << (y % 8);
    } else { 
        SSD1306_Buffer[x + (y / 8) * SSD1306_LCDWIDTH] &= ~(1 << (y % 8));
    }
}

void ssd1306_UpdateScreen(void) {
    // Write data to each page of RAM. Number of pages
    // depends on the screen height:
    //
    //  * 32px   ==  4 pages
    //  * 64px   ==  8 pages
    //  * 128px  ==  16 pages
    for(uint8_t i = 0; i < SSD1306_LCDHEIGHT/8; i++) {
        Oled_Command(0xB0 + i); // Set the current RAM page address.
        Oled_Command(0x00);
        Oled_Command(0x10);
        Oled_Data(&SSD1306_Buffer[SSD1306_LCDWIDTH*i]);
    }
}

void OLED_DrawPixel( int16_t x, int16_t y, uint8_t color)
{
  
   // if ( (x < 0) || (x >= SSD1306_LCDWIDTH()) || (y < 0) || (y >= SSD1306_LCDHEIGHT()))
    //return;

  switch (color)
  {
    case 1:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] |=  (1 << (y&7)); break;
    case 2:   buffer[x+ (y/8)*SSD1306_LCDWIDTH] &= ~(1 << (y&7)); break;
    case 3: buffer[x+ (y/8)*SSD1306_LCDWIDTH] ^=  (1 << (y&7)); break;
  }
}

void OLED_Image( const uint8_t *image)
{
  uint16_t i;
  for( i=0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8); i++)
  {
    buffer[i] = *(image+i);
  }
}