/*
 * File:   INA260.c
 * Author: D1NG0
 *
 * Created on July 29, 2021, 12:38 PM
 */


#include "INA260.h"
#include "mcc_generated_files/i2c_master.h"
#include "OLED.h"
#include <stdlib.h>


#define INA260_ADDRESS 0x40
#define INA260_REG_BUSVOLTAGE 0x02
#define INA260_REG_POWER  0x03
#define INA_RESET_DEVICE 0x8000
#define INA260_CURRENT_REGISTER 0x01
#define I2C_DELAY 0x10
#define _XTAL_FREQ 8000000

float voltage=0.0;
float current=0.0;
float power=0.0;
char oi[80];



float Voltage_ReadRegister(){
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(0x80);
    I2C_Master_Write(INA260_REG_BUSVOLTAGE);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0x81);
    char temp_a = I2C_Master_Read(1);
    unsigned int temp_b = temp_a<<8 | I2C_Master_Read(0);
    voltage = temp_b *1.25;
    I2C_Master_Stop();//Stop condition
    voltage = voltage/1000;
    __delay_ms(300);
    __delay_ms(600);
    return voltage;
}

float Current_ReadRegister(){
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(0x80);
    I2C_Master_Write(INA260_CURRENT_REGISTER);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0x81);
    char temp_c = I2C_Master_Read(1);
    unsigned int temp_d = temp_c<<8 | I2C_Master_Read(0);
    current = temp_d *1.25;
    I2C_Master_Stop();          //Stop condition
    current = current/1000;
    __delay_ms(300);
    __delay_ms(600);
    return current;
}

float Power_ReadRegister(){
    I2C_Master_Start();         //Start condition
    I2C_Master_Write(0x80);
    I2C_Master_Write(INA260_REG_POWER);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0x81);
    char temp_e = I2C_Master_Read(1);
    unsigned int temp_f = temp_e<<8 | I2C_Master_Read(0);
    power = temp_f *10;
    I2C_Master_Stop();          //Stop condition
    power = power/1000;
    __delay_ms(300);
    __delay_ms(600);
    return power;
}
void I2C_Master_Wait(void)
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(void)
{
  I2C_Master_Wait();
  SEN = 1;
}

void I2C_Master_RepeatedStart(void)
{
  I2C_Master_Wait();
  RSEN = 1;
}

void I2C_Master_Stop(void)
{
  I2C_Master_Wait();
  PEN = 1;
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;
}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;
  I2C_Master_Wait();
  ACKDT = (a)?0:1;
  ACKEN = 1;
  return temp;
}
void I2C_Master_Init(const unsigned long c)
{
  SSPCON1 = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (8000000/(4*c))-1;
  SSPSTAT = 0;
  TRISC3 = 1;        //Setting as input as given in datasheet
  TRISC4 = 1;        //Setting as input as given in datasheet
}


