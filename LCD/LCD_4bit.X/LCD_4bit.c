/*
 * File:   LCD_4bit.c
 * Author: ASUS
 *
 * Created on December 26, 2021, 10:11 PM
 */

// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "lcd_4bit.h"
#define _XTAL_FREQ 20000000

#define lcd_port  'B'
/*Pins and Port. Choose PortB connect to LCD*/
#if lcd_port=='B'
    #define RS                  RB1
    #define RW                  RB2
    #define EN                  RB3
    #define LCD_Port            PORTB
#elif lcd_port=='C'
    #define RS                  RC1
    #define RW                  RC2
    #define EN                  RC3
    #define LCD_Port            PORTC
#elif lcd_port=='D'
    #define RS                  RD1
    #define RW                  RD2
    #define EN                  RD3
    #define LCD_Port            PORTD
#endif
/*Working with LCD*/
#define ReadCommand         {RS=0; RW=1;}
#define WriteCommand        {RS=0; RW=0;}
#define ReadData            {RS=1; RW=1;}
#define WriteData           {RS=1; RW=0;}
/*Pulse allows data to pass*/
#define PulseENABLE         {EN=1;  __delay_us(1); EN=0;  __delay_us(40);}
/*Commands in LCD*/
#define LCD_Clear           0x01  //Clear display
#define LCD_Home            0x02  //Return cursor and LCD to home position
#define LCD_MoveCursor      0x06  //Move cursor to the end of the last character
#define LCD_Off             0x08  //Turn off display and cursor
#define LCD_DisplayOn       0x0C  //Turn on display and turn off cursor
#define LCD_CursorOn        0x0E  //Turn on display and cursor
#define LCD_CursorLine1     0x80  //Move cursor to the first position of line 1
#define LCD_CursorLine2     0xC0  //Move cursor to the first position of line 2
#define LCD_CursorLine3     0x90  //Move cursor to the first position of line 3
#define LCD_CursorLine4     0xD0  //Move cursor to the first position of line 4
#define LCD_MDRight         0x1C  //Move display to the right
#define LCD_MDLeft          0x18  //Move display to the left
/*LCD 4bit or LCD 8bit?*/
#define LCD_4bit            0x28
#define LCD_8bit            0x38

//unsigned char xedoc1[8]={0x00, 0x00, 0x04, 0x15, 0x1F, 0x1F, 0x1F, 0x11};
//unsigned char xedoc2[8]={0x00, 0x04, 0x15, 0x1F, 0x1F, 0x1F, 0x11, 0x00};
unsigned char xengang1[8]={0x00, 0x00, 0x1F, 0x0E, 0x0F, 0x0E, 0x1F, 0x00};
unsigned char xengang2[8]={0x00, 0x00, 0x0F, 0x07, 0x07, 0x07, 0x0F, 0x00};

unsigned char xengang4[8]={0x00, 0x00, 0x07, 0x03, 0x03, 0x03, 0x07, 0x00};
unsigned char xengang6[8]={0x00, 0x00, 0x03, 0x01, 0x01, 0x01, 0x03, 0x00};
unsigned char xengang8[8]={0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00};

unsigned char xengang3[8]={0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
unsigned char xengang5[8]={0x00, 0x00, 0x10, 0x00, 0x18, 0x00, 0x10, 0x00};
unsigned char xengang7[8]={0x00, 0x00, 0x18, 0x10, 0x1C, 0x10, 0x18, 0x00};
unsigned char xengang9[8]={0x00, 0x00, 0x1C, 0x18, 0x1E, 0x18, 0x1C, 0x00};
unsigned char xengang10[8]={0x00, 0x00, 0x1E, 0x1C, 0x1F, 0x1C, 0x1E, 0x00};

unsigned char xedoc3[8]={0x00, 0x11, 0x1F, 0x1F, 0x1F, 0x15, 0x04, 0x00};
unsigned char xedoc4[8]={0x00, 0x00, 0x11, 0x1F, 0x1F, 0x1F, 0x15, 0x04};
unsigned char xedoc5[8]={0x00, 0x00, 0x00, 0x11, 0x1F, 0x1F, 0x1F, 0x15};

unsigned char xedoc6[8]={0x00, 0x00, 0x00, 0x00, 0x11, 0x1F, 0x1F, 0x1F};
unsigned char xedoc8[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x1F, 0x1F};
unsigned char xedoc10[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x1F};
unsigned char xedoc12[8]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11};

unsigned char xedoc7[8]={0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char xedoc9[8]={0x15, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char xedoc11[8]={0x1F, 0x15, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char xedoc13[8]={0x1F, 0x1F, 0x15, 0x04, 0x00, 0x00, 0x00, 0x00};

unsigned char xedoc14[8]={0x1F, 0x1F, 0x1F, 0x15, 0x04, 0x00, 0x00, 0x00};

unsigned char xengang11[8]={0x00, 0x00, 0x1F, 0x0E, 0x1E, 0x0E, 0x1F, 0x00};
unsigned char xengang12[8]={0x00, 0x00, 0x1E, 0x1C, 0x1C, 0x1C, 0x1E, 0x00};

unsigned char xengang14[8]={0x00, 0x00, 0x1C, 0x18, 0x18, 0x18, 0x1C, 0x00};
unsigned char xengang16[8]={0x00, 0x00, 0x18, 0x10, 0x10, 0x10, 0x18, 0x00};
unsigned char xengang18[8]={0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00};

unsigned char xengang13[8]={0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
unsigned char xengang15[8]={0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00};
unsigned char xengang17[8]={0x00, 0x00, 0x03, 0x01, 0x07, 0x01, 0x03, 0x00};
unsigned char xengang19[8]={0x00, 0x00, 0x07, 0x03, 0x0F, 0x03, 0x07, 0x00};

unsigned char dan1[8]={0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};
unsigned char dan2[8]={0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
unsigned char dan3[8]={0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00};
unsigned char dan4[8]={0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00};
unsigned char dan5[8]={0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};

unsigned char bomb1[8]={0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x00};
unsigned char bomb2[8]={0x00, 0x00, 0x00, 0x0C, 0x0D, 0x00, 0x00, 0x00};
unsigned char bomb3[8]={0x00, 0x00, 0x00, 0x0C, 0x0E, 0x00, 0x00, 0x00};
unsigned char bomb4[8]={0x00, 0x08, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00};
unsigned char bomb5[8]={0x02, 0x08, 0x01, 0x14, 0x01, 0x08, 0x01, 0x04};
/*First command*/
void command(unsigned char a)
{
    WriteCommand
    LCD_Port=a;
    PulseENABLE
}

/*Send command*/
void lcd_command(unsigned char command)
{
    WriteCommand
    /*Send 4 high bit*/
    LCD_Port=(LCD_Port & 0x0F)|(command & 0xF0);
    PulseENABLE
    /*Send 4 low bit*/
    LCD_Port=(LCD_Port & 0x0F)|(command << 4);
    PulseENABLE
}

/*Send data 1 character*/
void lcd_putc(unsigned char data)
{
    WriteData
    /*Send 4 high bit*/
    LCD_Port=(LCD_Port & 0x0F)|(data & 0xF0);
    PulseENABLE
    /*Send 4 high bit*/
    LCD_Port=(LCD_Port & 0x0F)|(data << 4);
    PulseENABLE
}

/*Send data 1 character string*/
void lcd_puts(char *str)
{
    int i;
    for(i=0; str[i]!='\0'; i++)
    {
        lcd_putc(str[i]);
    }
}

/*Initiating LCD*/
void lcd_init()
{
    command(LCD_4bit);
    lcd_command(LCD_4bit);

    lcd_command(LCD_DisplayOn);
    lcd_command(LCD_Home);
    __delay_ms(5);
    lcd_command(LCD_Home);
    __delay_ms(20);
    lcd_command(LCD_Home);
    __delay_ms(20);

    lcd_command(LCD_CursorLine1);
    LCD_Port=0x00;
    lcd_command(LCD_CursorLine2);
    LCD_Port=0x00;
    lcd_command(LCD_DisplayOn);
}

/*Send the integer to LCD*/
/*In ASCII, 48=0*/
//void lcd_int(unsigned int value)
//{
//    int dem=0, a1, a2;
//    /*If it is a negative number*/
//    if(value<0)
//    {
//        lcd_putc('-');
//        value*=-1;
//    }
//    if(value>=0 && value<10)
//    {
//        lcd_putc((char)(value+48));
//    }
//    else if(value>=10 && value <100)
//        dem=2;
//    else if(value>=100 && value<1000)
//        dem=3;
//    else if(value>=1000 && value<10000)
//        dem=4;
//    else if(value>=10000 && value<32767)
//        dem=5;
//    
//    if(dem>=2)
//    {
//        do
//        {
//            a1=pow(10, dem-1);
//            a2=value/a1;
//            lcd_putc((char)(a2+48));
//            value = value % a1;
//            dem--;
//        }while(dem!=0);
//    }
//}
//
///*Send the decimal to LCD*/
///*void lcd_double(vale, number of decimal places)*/
//void lcd_double(double value, int n)
//{
//    int a1, a2, i;
//    double a3;
//    /*If it is a negative number*/
//    if(value<0)
//    {
//        lcd_putc('-');
//        value*=-1;
//    }
//    /*floor function: rounded down*/
//    a1=floor(value);
//    lcd_int(a1);
//    lcd_putc('.');
//    /*Decimals*/
//    a3=value-a1;
//    for(i=0; i<n; i++)
//    {
//        a3*=10;
//        a2=floor(a3);
//        lcd_int(a2);
//        a3=a3-a2;        
//    }
//}

/*Move the cursor to the set position*/
void lcd_gotoxy(int x, int y)
{
    switch(x)
    {
        case 1:
            lcd_command(LCD_CursorLine1 + (y-1));
            break;
        case 2:
            lcd_command(LCD_CursorLine2 + (y-1));
            break;
        case 3:
            lcd_command(LCD_CursorLine3 + (y-1));
            break;
        default:
            lcd_command(LCD_CursorLine4 + (y-1));
    }
}

void lcd_CGRAM()
{
    int c, d, e, f, g, h;
    /*Go to CGRAM address*/
//    lcd_command(0x40);
//    for(a=0; a<8; a++)        lcd_putc(xedoc1[a]);
//    
//    lcd_command(0x48);
//    for(b=0; b<8; b++)        lcd_putc(xedoc2[b]);
    
    lcd_command(0x50);
    for(c=0; c<8; c++)        lcd_putc(xengang1[c]);

    lcd_command(0x58);
    for(d=0; d<8; d++)        lcd_putc(xengang2[d]);
    
    lcd_command(0x60);
    for(e=0; e<8; e++)        lcd_putc(xengang3[e]);
    
    lcd_command(0x68);
    for(f=0; f<8; f++)        lcd_putc(xengang4[f]);
    
    lcd_command(0x70);
    for(g=0; g<8; g++)        lcd_putc(xengang5[g]);
    
    lcd_command(0x78);
    for(h=0; h<8; h++)        lcd_putc(xengang6[h]);
}

void lcd_CGRAM2()
{
    int a, b, c, d, e, f, g, h;
    /*Go to CGRAM address*/
    lcd_command(0x40);
    for(a=0; a<8; a++)        lcd_putc(xengang7[a]);
    
    lcd_command(0x48);
    for(b=0; b<8; b++)        lcd_putc(xengang8[b]);
    
    lcd_command(0x50);
    for(c=0; c<8; c++)        lcd_putc(xengang9[c]);

    lcd_command(0x58);
    for(d=0; d<8; d++)        lcd_putc(xengang10[d]);
    
    lcd_command(0x60);
    for(e=0; e<8; e++)        lcd_putc(xedoc3[e]);
    
    lcd_command(0x68);
    for(f=0; f<8; f++)        lcd_putc(xedoc4[f]);
    
    lcd_command(0x70);
    for(g=0; g<8; g++)        lcd_putc(xedoc5[g]);
//    
//    lcd_command(0x78);
//    for(h=0; h<8; h++)        lcd_putc(xedoc6[h]);
}

void lcd_CGRAM3()
{
    int a, b, c, d, e, f, g, h;
    /*Go to CGRAM address*/
    lcd_command(0x40);
    for(a=0; a<8; a++)        lcd_putc(xedoc6[a]);
    
    lcd_command(0x48);
    for(b=0; b<8; b++)        lcd_putc(xedoc7[b]);
    
    lcd_command(0x50);
    for(c=0; c<8; c++)        lcd_putc(xedoc8[c]);

    lcd_command(0x58);
    for(d=0; d<8; d++)        lcd_putc(xedoc9[d]);
    
    lcd_command(0x60);
    for(e=0; e<8; e++)        lcd_putc(xedoc10[e]);
    
    lcd_command(0x68);
    for(f=0; f<8; f++)        lcd_putc(xedoc11[f]);
    
    lcd_command(0x70);
    for(g=0; g<8; g++)        lcd_putc(xedoc12[g]);
    
    lcd_command(0x78);
    for(h=0; h<8; h++)        lcd_putc(xedoc13[h]);
}

void lcd_CGRAM4()
{
    int a, b, c, d, e, f, g, h;
    /*Go to CGRAM address*/
    lcd_command(0x40);
    for(a=0; a<8; a++)        lcd_putc(xedoc14[a]);
    
    lcd_command(0x48);
    for(b=0; b<8; b++)        lcd_putc(xengang11[b]);
    
    lcd_command(0x50);
    for(c=0; c<8; c++)        lcd_putc(xengang12[c]);

    lcd_command(0x58);
    for(d=0; d<8; d++)        lcd_putc(xengang13[d]);
    
    lcd_command(0x60);
    for(e=0; e<8; e++)        lcd_putc(xengang14[e]);
    
    lcd_command(0x68);
    for(f=0; f<8; f++)        lcd_putc(xengang15[f]);
    
    lcd_command(0x70);
    for(g=0; g<8; g++)        lcd_putc(xengang16[g]);
    
    lcd_command(0x78);
    for(h=0; h<8; h++)        lcd_putc(xengang17[h]);
}

void lcd_CGRAM5()
{
    int a, b, c, d, e, f, g, h;
    /*Go to CGRAM address*/
    lcd_command(0x40);
    for(a=0; a<8; a++)        lcd_putc(xengang18[a]);
    
    lcd_command(0x48);
    for(b=0; b<8; b++)        lcd_putc(xengang19[b]);
    
    lcd_command(0x50);
    for(c=0; c<8; c++)        lcd_putc(bomb1[c]);

    lcd_command(0x58);
    for(d=0; d<8; d++)        lcd_putc(dan1[d]);
    
    lcd_command(0x60);
    for(e=0; e<8; e++)        lcd_putc(dan2[e]);
    
    lcd_command(0x68);
    for(f=0; f<8; f++)        lcd_putc(dan3[f]);
    
    lcd_command(0x70);
    for(g=0; g<8; g++)        lcd_putc(dan4[g]);
    
    lcd_command(0x78);
    for(h=0; h<8; h++)        lcd_putc(dan5[h]);
}

void lcd_CGRAM6()
{
    int a, b, c, d, e, f, g, h;
    /*Go to CGRAM address*/
//    lcd_command(0x40);
//    for(a=0; a<8; a++)        lcd_putc(bomb1[a]);
//    
//    lcd_command(0x48);
//    for(b=0; b<8; b++)        lcd_putc(bomb2[b]);
//    
//    lcd_command(0x50);
//    for(c=0; c<8; c++)        lcd_putc(bomb3[c]);
//
//    lcd_command(0x58);
//    for(d=0; d<8; d++)        lcd_putc(bomb4[d]);
    
    lcd_command(0x60);
    for(e=0; e<8; e++)        lcd_putc(bomb2[e]);
    
    lcd_command(0x68);
    for(f=0; f<8; f++)        lcd_putc(bomb3[f]);
    
    lcd_command(0x70);
    for(g=0; g<8; g++)        lcd_putc(bomb4[g]);
    
    lcd_command(0x78);
    for(h=0; h<8; h++)        lcd_putc(bomb5[h]);
}


void main(void) 
{
    TRISB=0;  PORTB=0;
    int i;
    lcd_init(); 
    lcd_CGRAM();
    for(i=1; i<16; i++)
    {
        lcd_CGRAM();
        lcd_gotoxy(1,i);            
        lcd_putc(2);
        __delay_ms(200);

        lcd_gotoxy(1,i);
        lcd_putc(3);
        lcd_gotoxy(1,i+1);
        lcd_putc(4);
        __delay_ms(200);

        lcd_gotoxy(1,i);
        lcd_putc(5);
        lcd_gotoxy(1,i+1);
        lcd_putc(6);
        __delay_ms(200);

        lcd_gotoxy(1,i);
        lcd_putc(7);

        lcd_CGRAM2();
        lcd_gotoxy(1,i+1);
        lcd_putc(0);
        __delay_ms(200);

        lcd_gotoxy(1,i);
        lcd_putc(1);
        lcd_gotoxy(1,i+1);
        lcd_putc(2);
        __delay_ms(200);

        lcd_gotoxy(1,i);
        lcd_putc(' ');
        lcd_gotoxy(1,i+1);
        lcd_putc(3);
        __delay_ms(200);
    }
    lcd_CGRAM();
    lcd_gotoxy(1,16);            
    lcd_putc(2);
    __delay_ms(200);
    lcd_CGRAM2();
    lcd_gotoxy(1,16);
    lcd_putc(4);
    __delay_ms(200);
    lcd_gotoxy(1,16);
    lcd_putc(5);
    __delay_ms(200);
    lcd_gotoxy(1,16);
    lcd_putc(6);
    __delay_ms(200);

    lcd_CGRAM3();
    lcd_gotoxy(1,16);
    lcd_putc(0);
    lcd_gotoxy(2,16);
    lcd_putc(1);
    __delay_ms(200);

    lcd_gotoxy(1,16);
    lcd_putc(2);
    lcd_gotoxy(2,16);
    lcd_putc(3);
    __delay_ms(200);

    lcd_gotoxy(1,16);
    lcd_putc(4);
    lcd_gotoxy(2,16);
    lcd_putc(5);
    __delay_ms(200);

    lcd_gotoxy(1,16);
    lcd_putc(6);
    lcd_gotoxy(2,16);
    lcd_putc(7);
    __delay_ms(200);

    lcd_CGRAM4();
    lcd_gotoxy(1,16);
    lcd_putc(' ');
    lcd_gotoxy(2,16);
    lcd_putc(0);
    __delay_ms(200);


    lcd_CGRAM4();
    lcd_gotoxy(2,16);            
    lcd_putc(1);
    __delay_ms(200);

    lcd_gotoxy(2,16);
    lcd_putc(2);
    lcd_gotoxy(2,15);
    lcd_putc(3);
    __delay_ms(200);

    lcd_gotoxy(2,16);
    lcd_putc(4);
    lcd_gotoxy(2,15);
    lcd_putc(5);
    __delay_ms(200);

    lcd_gotoxy(2,16);
    lcd_putc(6);
    lcd_gotoxy(2, 15);
    lcd_putc(7);
    __delay_ms(200);

    lcd_CGRAM5();  
    lcd_gotoxy(2,16);
    lcd_putc(0);
    lcd_gotoxy(2,15);
    lcd_putc(1);
    __delay_ms(200);

    lcd_gotoxy(2, 10);
    lcd_putc(2);
    __delay_ms(200);   
    
    lcd_gotoxy(2,16);
    lcd_putc(0);
    lcd_gotoxy(2,15);
    lcd_putc(1);
    __delay_ms(50);
   
    
    for(i=14; i>10; i--)
    {
        lcd_CGRAM5();
        lcd_gotoxy(2,i);
        lcd_putc(3);
        __delay_ms(50);
        lcd_gotoxy(2,i);
        lcd_putc(4);
        __delay_ms(50);
        lcd_gotoxy(2,i);
        lcd_putc(5);
        __delay_ms(50);
        lcd_gotoxy(2,i);
        lcd_putc(6);
        __delay_ms(50);
        lcd_gotoxy(2,i);
        lcd_putc(7);
        __delay_ms(50);
        lcd_gotoxy(2,i);
        lcd_putc(' ');
    }
    
    lcd_CGRAM6();
    lcd_gotoxy(2,10);
    lcd_putc(4);
    __delay_ms(50);
    lcd_gotoxy(2,10);
    lcd_putc(5);
    __delay_ms(50);
    lcd_gotoxy(2,10);
    lcd_putc(6);
    __delay_ms(50);
    lcd_gotoxy(2,10);
    lcd_putc(7);
    
    while(1)
    {
        
    }
}
