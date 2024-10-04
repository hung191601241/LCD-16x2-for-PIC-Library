
/*Pins and Port. Choose PortB connect to LCD*/
#define RS                  RB1
#define RW                  RB2
#define EN                  RB3
#define LCD_Port            PORTB
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
#define LCD_MDRight         0x1C  //Move display to the right
#define LCD_MDLeft          0x18  //Move display to the left
/*LCD 4bit or LCD 8bit?*/
#define LCD_4bit            0x28
#define LCD_8bit            0x38

/*Function*/
void command(unsigned char a);
void lcd_command(unsigned char command);
void lcd_putc(unsigned char data);
void lcd_puts(char *str);
void lcd_init();
void lcd_int(unsigned int value);
void lcd_double(double value, int n);
void lcd_gotoxy(int x, int y);
