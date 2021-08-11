/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.6
        Device            :  PIC18F46K80
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include <stdio.h> /* for sprintf */
#include "OLED.h"
#include "INA260.h"

char voltage_str[6];
char current_str[6];
char power_str[6];
float Voltage;
float Current;
float Power;

static const unsigned char IPL[] = {
// 'estg_v_branco', 64x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xfc, 0x3f, 0xc0, 0x03, 0xf3, 0xcf, 0xc0,
0x03, 0xef, 0xf3, 0xc0, 0x03, 0xdf, 0xfb, 0xc0, 0x03, 0xdb, 0xfb, 0xc0, 0x03, 0xcf, 0xf3, 0xc0,
0x03, 0xe7, 0xe7, 0xc0, 0x03, 0xf8, 0x1f, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0,
0x03, 0xff, 0xff, 0xc0, 0x00, 0x01, 0x00, 0x00, 0x03, 0xef, 0xff, 0xc0, 0x02, 0xff, 0xff, 0xc0,
0x00, 0x3f, 0xe0, 0x00, 0x03, 0xbf, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe3, 0xfc, 0x00,
0x03, 0xff, 0x7c, 0x00, 0x02, 0xff, 0xe0, 0x00, 0x00, 0x08, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    I2C_Initialize();
    
    Oled_Init();
    Oled_Clear();
    Oled_SelectPage(0); 
    Oled_WriteString("      IPL-ESTG ");
     __delay_ms(2000);
    Oled_SelectPage(1);
    Oled_WriteString("  Conducao autonoma ");
     __delay_ms(2000);
    Oled_Clear();
    //OLEDWriteLargeCharacter("IPL");
    //OLED_Image(IPL);

    
    I2C_Master_Init(400000);
    while (1)
    {
       Voltage = Voltage_ReadRegister();
       Current = Current_ReadRegister();
       Power = Power_ReadRegister();
       sprintf(voltage_str, "%.2f",Voltage);
       sprintf(current_str, "%.2f",Current);
       sprintf(power_str, "%.2f",Power);
       //__delay_ms(600);
       Oled_SelectPage(0);
       Oled_WriteString("    Bateria 7.4V ");
       __delay_ms(600);
       Oled_SelectPage(1);
       Oled_WriteString("Tensao: ");
       Oled_WriteString(voltage_str);
       Oled_WriteString(" V");
       __delay_ms(600);
       Oled_SelectPage(2);
       Oled_WriteString("Corrente: ");
       Oled_WriteString(current_str); 
       Oled_WriteString(" A");
       __delay_ms(600);
       Oled_SelectPage(3);
       Oled_WriteString("Potencia: ");
       Oled_WriteString(power_str);
       Oled_WriteString(" W");
       __delay_ms(5000);
       Oled_Clear();
       I2C_Master_Init(400000);
    }
}
/**
 End of File
*/